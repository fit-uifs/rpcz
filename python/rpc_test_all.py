import httplib  
import threading
import random
import time
import datetime
from grpc_client import grpc_cla_test
import dqlib_rpcz
import search_rpcz
from google.protobuf import descriptor
from rpcz import Application
from rpcz import service
from rpcz import service_reflection
from google.protobuf import descriptor_pb2

ret_data = []
lock = threading.Lock()



def rpcz_test(ncycles):
	for x in xrange(ncycles):
		ret = rpcz_run()
		lock.acquire()
		ret_data.append(ret)
		lock.release()

def grpc_test(ncycles):
	for x in xrange(ncycles):
		ret = grpc_run()
		lock.acquire()
		ret_data.append(ret)
		lock.release()


def bunch_test(nthreads,ncycles,func):
	global ret_data
	ret_data = []
	a = datetime.datetime.now()
	threads = []
	for i in range(nthreads):
		t1 = threading.Thread(target=func,args=(ncycles,))
		threads.append(t1)
	for t in threads:
		t.setDaemon(True)
		t.start()
	for t in threads:
		t.join()

	b = datetime.datetime.now()
	succ = fail = 0
	for ret in ret_data:
		if ret:
			succ += 1
		else:
			fail += 1
	print "Total tests:%d times,used time  %d seconds.%d threads run %d cycles, %d successful, %d failed."%(nthreads*ncycles,(b-a).seconds, nthreads,ncycles,succ,fail)
	ret_data=[]



import dqlib_pb2


_DQSERVICE = descriptor.ServiceDescriptor(
  name='DQService',
  full_name='examples.DQService',
  file=dqlib_pb2.DESCRIPTOR,
  index=0,
  options=None,
  serialized_start=758,
  serialized_end=854,
  methods=[
  descriptor.MethodDescriptor(
    name='CLAService',
    full_name='examples.DQService.CLAService',
    index=0,
    containing_service=None,
    input_type=dqlib_pb2._MVOASSETALLOCATIONINPUT,
    output_type=dqlib_pb2._MVOASSETALLOCATIONOUTPUT,
    options=None,
  ),
])

class DQService(service.Service):
  __metaclass__ = service_reflection.GeneratedServiceType
  DESCRIPTOR = _DQSERVICE
class DQService_Stub(DQService):
  __metaclass__ = service_reflection.GeneratedServiceStubType
  DESCRIPTOR = _DQSERVICE

# app = Application()
# stub = DQService_Stub(
# 	  app.create_rpc_channel("tcp://222.190.119.204:10000"))#222.190.119.204
# request = dqlib_pb2.MvoAssetAllocationInput()
# request.strategy = "MVOSTRANDARD"
# request.target_type = "MINVAR"
# request.asset_return.rows=2;
# request.asset_return.cols=2;
# request.asset_return.data.extend([0.00215,    -0.00062, -0.00062,0.00069])
# request.lower_bound.data.extend([             0,             0         ])
# request.upper_bound.data.extend([             1,             1         ])
# request.target_value = 0
# request.num_returns_for_mean = 2
# request.annualization_scale = 2
# request.num_ef_points = 10

from google.protobuf import descriptor
import rpcz
from rpcz import service
from rpcz import service_reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)
import search_pb2

_SEARCHSERVICE = descriptor.ServiceDescriptor(
  name='SearchService',
  full_name='examples.SearchService',
  file=search_pb2.DESCRIPTOR,
  index=0,
  options=None,
  serialized_start=193,
  serialized_end=269,
  methods=[
  descriptor.MethodDescriptor(
    name='Search',
    full_name='examples.SearchService.Search',
    index=0,
    containing_service=None,
    input_type=search_pb2._SEARCHREQUEST,
    output_type=search_pb2._SEARCHRESPONSE,
    options=None,
  ),
])
import search_pb2
class SearchService(service.Service):
  __metaclass__ = service_reflection.GeneratedServiceType
  DESCRIPTOR = _SEARCHSERVICE
class SearchService_Stub(SearchService):
  __metaclass__ = service_reflection.GeneratedServiceStubType
  DESCRIPTOR = _SEARCHSERVICE
app = rpcz.Application()

stub = SearchService_Stub(
	app.create_rpc_channel("tcp://222.190.119.204:10000"))

request = search_pb2.SearchRequest()
request.query = 'gold'


def rpcz_run():
  # print "CLAService start"
  # response =  stub.CLAService (request, deadline_ms=10000)
  # print response.target_value
  response = stub.Search(request, deadline_ms=10000)
  return len(response.results) >1

from grpc_client import dqmvoassetallocationservice_pb2_grpc
from grpc_client  import dqmvoassetallocationservice_pb2
import grpc

grpc_channel = grpc.insecure_channel('222.190.119.204:10027')
grpc_stub = dqmvoassetallocationservice_pb2_grpc.CLAServiceStub(grpc_channel)
grpc_request = dqmvoassetallocationservice_pb2.MvoAssetAllocationInput()
grpc_request.strategy = "MVOSTRANDARD"
grpc_request.target_type = "MINVAR"
grpc_request.asset_return.rows = 2;
grpc_request.asset_return.cols = 2;
grpc_request.asset_return.data.extend([0.00215, -0.00062, -0.00062, 0.00069])

grpc_request.lower_bound.data.extend([0, 0])
grpc_request.upper_bound.data.extend([1, 1])

grpc_request.target_value = 0
grpc_request.num_returns_for_mean = 2
grpc_request.annualization_scale = 2
grpc_request.num_ef_points = 10
def grpc_run():
	response = grpc_stub.CLA(grpc_request)
	# print "weights:" ,response.weights
	# print response.target_value
	return abs(response.target_value-0.00277)<0.00001

if __name__ == '__main__':
	# oneGrpc(1)
	# oneRpcz()

	print "Test rpcz:"
	bunch_test(100, 20, rpcz_test)

	# print "Test Grpc:"
	# bunch_test(10, 200, grpc_test)








