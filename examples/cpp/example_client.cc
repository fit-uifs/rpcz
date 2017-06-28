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

//#ifdef _WIN32 || WIN32
//#define GLOG_NO_ABBREVIATED_SEVERITIES
////#include <windows.h>
//#endif

#include "rpcz/rpcz.hpp"
#include "cpp/search.pb.h"
#include "cpp/search.rpcz.h"
#include "cpp/dqlib.pb.h"
#include "cpp/dqlib.rpcz.h"
#include "rpcz/zmq.hpp"

#include <thread>
#include <mutex>
#include<numeric>
#include "inirw.h"
#include<iostream>
#include<iomanip>
#include <iostream>
#include <boost/thread/thread.hpp>

//#include <glog/logging.h>
#include "rpcz/callback.hpp"
#include "rpcz/connection_manager.hpp"
#include "rpcz/rpc_channel.hpp"
#include "rpcz/rpc.hpp"
#include "rpcz/server.hpp"
#include "rpcz/sync_event.hpp"

using namespace std;

std::vector<bool> result_list;
std::vector<int64_t> time_list;
//int64_t time_list[10000];
//bool result_list[10000];
mutex mt;

char* host = new char[128];
int thread_cnt = 50;
int cycle_cnt = 10;
void search_test( rpcz::application& application, const examples::MvoAssetAllocationInput& dqrequest, int ncycle) {
    for (int j = 0; j < ncycle; j++) {
        rpcz::rpc rpc;
        rpc.set_deadline_ms(10000);
        rpcz::sync_event event;

//        dq_stub.CLAService(dqrequest, &dqresponse,   &rpc, rpcz::new_callback(
//                &event, &rpcz::sync_event::signal));
        examples::SearchService_Stub search_stub(application.create_rpc_channel(
                host), true);
        examples::SearchRequest request;
        examples::SearchResponse response;
        request.set_query("gold");

        auto start_time = chrono::high_resolution_clock::now();
        try {
            search_stub.Search(request, &response, &rpc, rpcz::new_callback(
                &event, &rpcz::sync_event::signal));
            //cout << response.DebugString() << endl;
        } catch (rpcz::rpc_error &e) {
            cout << "Error: " << e.what() << endl;;
        }
        event.wait();

        //ASSERT_EQ(rpc_response_header::DEADLINE_EXCEEDED, rpc.get_status());
        int sz = response.results_size();
        bool ret =  rpc.ok();
        ret &= response.results_size()==2;
        auto end_time = chrono::high_resolution_clock::now();

        int64_t cnt = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count() ;
        mt.lock();
        result_list.push_back(ret);
        time_list.push_back(cnt);
        mt.unlock();
    }
}
void cla_test( rpcz::application& application, const examples::MvoAssetAllocationInput& dqrequest, int ncycle)
{
//    struct  timeval    tvstart,tvend;
//    memset(&tvstart,0,sizeof(struct timeval));
//    memset(&tvend,0,sizeof(struct timeval));

    for (int j = 0; j < ncycle; j++)
    {
        //gettimeofday(&tvstart,NULL);
        auto start_time = chrono::high_resolution_clock::now();

        examples::DQService_Stub dq_stub(application.create_rpc_channel(host), true);
        examples::MvoAssetAllocationOutput dqresponse;


            rpcz::rpc rpc;
            //rpc.set_deadline_ms(10000);
            rpcz::sync_event event;
            try {
                    dq_stub.CLAService(dqrequest, &dqresponse,   &rpc, rpcz::new_callback(
                    &event, &rpcz::sync_event::signal));
            event.wait();

            //ASSERT_EQ(rpc_response_header::DEADLINE_EXCEEDED, rpc.get_status());
//            ASSERT_TRUE(rpc.ok());
//            ASSERT_EQ(2, response.results_size());
//            ASSERT_EQ("The search for happiness", response.results(0));
            //cout << dqresponse.DebugString() << dqrequest.target_value() <<endl;
            bool ret =  rpc.ok() && std::abs(dqresponse.target_value() - 0.00277) < 0.00001;

            auto end_time = chrono::high_resolution_clock::now();
//            gettimeofday(&tvend,NULL);
//            int cnt = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);
            int64_t cnt = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count() ;
            mt.lock();
            result_list.push_back(ret);
            time_list.push_back(cnt);
            mt.unlock();
        }
        catch (rpcz::rpc_error &e) {
            cout << "Error: " << e.what() << endl;
        }
        catch (zmq::error_t& e) {
            std::cerr << "ZMQ EXCEPTION : " << e.what() << std::endl;
        } catch (std::exception& e) {
            std::cerr << "STD EXCEPTION : " << e.what() << std::endl;
        } catch (...) {
            std::cerr << " UNTRAPPED EXCEPTION " << std::endl;
        }
    }
}


rpcz::application* application=NULL;
examples::MvoAssetAllocationInput dqrequest;
int main() {
    std::cout << "Start testing: \n" ;
    iniFileLoad("testConfig.ini");

    iniGetString("test","host",host,64,"");
    int total = iniGetInt("test","total_cnt",0);
    thread_cnt = iniGetInt("test","thread_cnt",0);
    cycle_cnt = iniGetInt("test","total_cnt",0)/thread_cnt;

    std::cout << "Host " << host << "\ntotal_cnt: " << total << "\nthread_cnt:  " << thread_cnt << "\ncycle_cnt: " << cycle_cnt << endl ;

    zmq::context_t * context = new zmq::context_t(10,20480);
    rpcz::application::options opt;
    opt.connection_manager_threads =30;
    opt.zeromq_context = context;
    application =  new rpcz::application(opt);



  dqrequest.set_target_type("MINVAR");
  dqrequest.set_strategy("MVOSTRANDARD");
  examples::Matrix* matrix = new examples::Matrix;
  matrix->set_cols(2);
  matrix->set_rows(2);
  matrix->add_data(0.00215);
  matrix->add_data(-0.00062);
  matrix->add_data(-0.00062);
  matrix->add_data(0.00069);
  matrix->set_storage_order(::examples::Matrix_StorageOrder_RowMajor);
  dqrequest.set_allocated_asset_return(matrix) ;//.set_asset_return.rows=2;
  examples::Vector* lb = new examples::Vector;
  lb->add_data(0);
  lb->add_data(0);
  dqrequest.set_allocated_lower_bound(lb);

  examples::Vector* ub = new examples::Vector;
  ub->add_data(1.0);
  ub->add_data(1.0);
  dqrequest.set_allocated_upper_bound(ub);

  dqrequest.set_target_value(0) ;
  dqrequest.set_num_returns_for_mean (2);
  dqrequest.set_annualization_scale (2);
  dqrequest.set_num_ef_points (10);


//    struct  timeval    tvstart,tvend;
//    memset(&tvstart,0,sizeof(struct timeval));
//    memset(&tvend,0,sizeof(struct timeval));
    //gettimeofday(&tvstart,NULL);

    auto start_time = chrono::high_resolution_clock::now();

    std::vector<std::thread> cla_threads;
    cla_threads.clear();

    for (int j = 0; j<thread_cnt;j++)
    {
        thread t(search_test, std::ref(*application), std::ref(dqrequest), cycle_cnt);
        cla_threads.push_back(std::move(t));
    }

    for (auto& t : cla_threads)
        t.join();

    auto end_time = chrono::high_resolution_clock::now();
    int64_t cnt = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count() ;
    //gettimeofday(&tvend,NULL);
    //int cnt = (tvend.tv_sec-tvstart.tv_sec)*1000000+(tvend.tv_usec-tvstart.tv_usec);

    int succ = 0;
    int fail = 0;
    for (int i = 0; i < result_list.size(); i++)
    {
        if (result_list.at(i))
            succ++;
        else
            fail++;
    }

    std::cout << "Total tests:" << thread_cnt*cycle_cnt << " times,used time  " << fixed << setprecision(0)<< cnt/1000000000 << " s. " << endl;
    std::sort(time_list.begin(),time_list.end());
    //unsigned int sum = std::accumulate(time_list.begin(),time_list.end(),0);
    double mean = 0;
    for(auto i:time_list)
        mean+=i/1.0/succ/1000000;

    //std::cout<< time_list[succ-1] <<" " << time_list[succ-2] << " "<< time_list[succ-3] <<" " << time_list[succ-4] << endl;
    //std::cout << "Total time: " << fixed << setprecision(0) << sum/1000.0 << " ms "<<" cnt: " << time_list.size() << endl;
    if(succ<total)
        std::cout << "Fail: " << total - succ << endl ;
    std::cout<< "Throughput: " << std::round(thread_cnt*cycle_cnt/(cnt/1000000000.0))  <<"\n Mean : " << fixed << setprecision(0) << mean <<" ms\n Median: " <<time_list[succ/2]/1000000.0<< " ms\n Max:"<< time_list[succ-1]/1000000.0 <<" ms" << endl<< endl;

    delete host;
    //delete context;
    delete application;
}


