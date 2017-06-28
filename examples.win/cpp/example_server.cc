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

#include <windows.h>
#include <iostream>
#include "rpcz/rpcz.hpp"
#include "cpp/search.pb.h"
#include "cpp/search.rpcz.h"
#include "cpp/dqlib.pb.h"
#include "cpp/dqlib.rpcz.h"
//#include "cpp/dqlibgo.h"


//#pragma comment(lib, "../debug/dqlibgo.lib")
//extern "C"
//#ifdef _WIN32 || WIN32
//_declspec(dllexport)
//#endif
//void ProcessRequest(char* req_type, unsigned char* req_arg_msg, int length, unsigned char* out, int* out_length);
using namespace std;
typedef void(*ProcessRequest)(char* req_type, unsigned char* req_arg_msg, int length, unsigned char* out, int* out_length);
HMODULE hDll = NULL;
ProcessRequest pr = NULL;

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

int out_len = 512;
unsigned char* out = new unsigned char[out_len];
unsigned char* in = new unsigned char[out_len];

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
				if (hDll == NULL)
					hDll = LoadLibrary("dqlibgo.dll");
				if (hDll != NULL)
				{
					//cout << "GetProcAddress start. " << endl;
					if(pr == NULL)
						pr = (ProcessRequest)GetProcAddress(hDll, "ProcessRequest");
					if (pr != NULL)
					{
						//cout << "ProcessRequest start. " << endl;
						pr("MVOASSETALLOCATION", in, in_len, out, &out_len);

						MvoAssetAllocationOutput response;
						//cout << "parse response start. " << endl;
						response.ParseFromArray(out, out_len);
						//cout << "reply start. " << endl;
						reply.send(response);
						//cout << "reply ok. " << endl;
					}
					
				}
				else
					cout << "loadlibrary fail: " << GetLastError() <<endl;

		}
		catch (const std::exception& e)
		{
			int i = 0;
		}
	}
};
}  // namespace examples


int main() {


  rpcz::application application;
  rpcz::server server(application);

  examples::SearchServiceImpl search_service;
  server.register_service(&search_service);

  hDll = LoadLibrary("dqlibgo.dll");
  pr = (ProcessRequest)GetProcAddress(hDll, "ProcessRequest");
  examples::DQServiceImpl dq_service;
  server.register_service(&dq_service);

  cout << "Serving requests on port 10000." << endl;
  server.bind("tcp://*:10000");
  application.run();

  FreeLibrary(hDll);
}
