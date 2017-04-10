// Copyright 2011 Google Inc. All Rights Reserved.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: nadavs@google.com <Nadav Samet>

#include <iostream>
#include "rpcz/rpcz.hpp"
#include "cpp/search.pb.h"
#include "cpp/search.rpcz.h"
#include "cpp/dqlib.pb.h"
#include "cpp/dqlib.rpcz.h"
#include <dlfcn.h>
//#include <limits.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include <dlfcn.h>
//#include <string.h>
#include "inirw.h"
#include "rpcz/zmq.hpp"
#include <thread>
#include <mutex>
#include<numeric>
#include "inirw.h"
#include<iostream>
#include<iomanip>

using namespace std;
//typedef void(*ProcessRequest)(char* req_type, unsigned char* req_arg_msg, int length, unsigned char* out, int* out_length);
//
//ProcessRequest pr = NULL;

void* handle=NULL;
typedef void (*FPTR)(char* req_type, unsigned char* req_arg_msg, int length, unsigned char* out, int* out_length, char** err);
FPTR fptr = NULL;
char err_buf[256] = "\0";
int out_len = 5120;
unsigned char* out = new unsigned char[out_len];
unsigned char* in = new unsigned char[out_len];
char* host = new char[64];

void init()
{
    iniFileLoad("testConfig.ini");
    iniGetString("test","server_host",host,128,"");

	if (handle == NULL){
		handle = dlopen("./libdqlibgo.so.1.2", RTLD_LAZY);// RTLD_GLOBALRTLD_NOW
		printf("dll init loaded\n");
	}
	if (handle != NULL){
		fptr = (FPTR)dlsym(handle, "ProcessRequest");
		printf("function init loaded\n");
	}
}


void PRequest(char* req_type, unsigned char* req_arg_msg,int length, unsigned char* out, int* out_length, char** error)
{
	if (handle == NULL) {
		handle = dlopen("./libdqlibgo.so.1.2", RTLD_LAZY);// RTLD_GLOBALRTLD_NOW
		printf("dll reloaded\n");
		if (handle == NULL) {
			strcpy(err_buf,"dlopen ");
			strcat(err_buf,(char*)dlerror());
			*error = err_buf;
			return;
		}
	}

	if (fptr == NULL) {
		fptr = (FPTR)dlsym(handle, "ProcessRequest");
		printf("function reloaded\n");
		if (fptr == NULL) {
			strcpy(err_buf,"dlsym  ");
			strcat(err_buf,(char*)dlerror());
			*error = err_buf;
			return;
		}
	}

	(*fptr)(req_type,req_arg_msg,length,out,out_length,error);
	return;

}

namespace examples {

class SearchServiceImpl : public SearchService {

  virtual void Search(

      const SearchRequest& request,
      rpcz::reply<SearchResponse> reply) {
    //cout << "Got request for '" << request.query() << "'" << endl;

    SearchResponse response;
    response.add_results("result1 for " + request.query());
    response.add_results("this is result2");
    reply.send(response);
  }

};

class DQServiceImpl : public DQService {

	virtual void CLAService(
		const ::examples::MvoAssetAllocationInput& request,
		::rpcz::reply< ::examples::MvoAssetAllocationOutput> reply) {
		//cout << "Got request for '"<<" "<< request.strategy()<<" " << request.target_type() << "'" << endl;

		int in_len = request.ByteSize();
		request.SerializeToArray(in, in_len);
		//cout << "loadlibrary start. " <<  endl;
		try
		{
				if (fptr != NULL)
				{
					char* error=NULL;
					//cout << "ProcessRequest start. " << endl;
					PRequest("MVOASSETALLOCATION", in, in_len, out, &out_len,&error);
					if(error!=NULL)
					{
						cout << "error occurs:" << error;
						return;
					}

					MvoAssetAllocationOutput response;
					//cout << "parse response start. " << endl;
					response.ParseFromArray(out, out_len);
					//cout << "reply start. " << endl;
					reply.send(response);
					//cout << "reply ok. " << endl;

				}
				else{
					init();
					cout << "Reload library";
				}


		}
		catch (const std::exception& e)
		{
			cout << e.what();
		}
	}
};
}  // namespace examples

int main() {
	init();

    zmq::context_t context(10,2048);
    rpcz::application::options opt;
	opt.connection_manager_threads = 30;
    opt.zeromq_context = &context;
  rpcz::application application(opt);
  rpcz::server server(application);

  examples::SearchServiceImpl search_service;
  server.register_service(&search_service);
  examples::DQServiceImpl dq_service;
  server.register_service(&dq_service);

  cout << "Serving requests on "<< host << endl;
  server.bind(host);//"tcp://*:5555"
  application.run();
}
