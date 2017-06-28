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

using namespace std;

int main() {
  rpcz::application application;

  examples::SearchService_Stub search_stub(application.create_rpc_channel(
          "tcp://222.190.119.204:10000"), true);
  examples::SearchRequest request;
  examples::SearchResponse response;
  request.set_query("gold");

  cout << "Sending request." << endl;
  try {
    search_stub.Search(request, &response, 1000);
    cout << response.DebugString() << endl;
  } catch (rpcz::rpc_error &e) {
    cout << "Error: " << e.what() << endl;;
  }

  examples::DQService_Stub dq_stub(application.create_rpc_channel(
	  "tcp://222.190.119.204:10000"), true);
  examples::MvoAssetAllocationInput dqrequest;
  examples::MvoAssetAllocationOutput dqresponse;
  dqrequest.set_target_type("test");

  cout << "Sending dqlib request." << endl;
  try {
	  dq_stub.CLAService(dqrequest, &dqresponse, 1000);
	  cout << dqresponse.DebugString() << dqrequest.target_value() <<endl;
  }
  catch (rpcz::rpc_error &e) {
	  cout << "Error: " << e.what() << endl;;
  }
}
