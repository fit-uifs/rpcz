import httplib  
import threading
import random
import time
import datetime
from grpc_client import grpc_cla_test
import dqlib_rpcz
import search_rpcz


ret_data = []
# lock = thread.allocate_lock()
lock = threading.Lock()


def oneSearch():
    # a = datetime.datetime.now()
    # rt=random.randint(1,10)
    # if rsleep==1:
    # 	time.sleep(rt)
    ret = search_rpcz.run()
    # b = datetime.datetime.now()
    # retstr = "get once "

    lock.acquire()
    if (len(ret) >1):
        # return True# retstr += "successfully! "
        ret_data.append(True)
    else:
        # return False# retstr += "failed! "
        ret_data.append(False)
    lock.release()

def oneRpcz():
	# a = datetime.datetime.now()
	# rt=random.randint(1,10)
	# if rsleep==1:
	# 	time.sleep(rt)
	ret = dqlib_rpcz.run()
	# b = datetime.datetime.now()
	# retstr = "get once "
	
	lock.acquire()
	if(abs(ret-0.00277)<0.00001):
		# return True# retstr += "successfully! "
		ret_data.append(True)
	else:
		# return False# retstr += "failed! "
		ret_data.append(False)
	lock.release()

rsleep=10
def oneGrpc():
	# a = datetime.datetime.now()
	# rt=random.randint(1,10)
	# if rsleep==1:
	# 	time.sleep(rt)
	ret = grpc_cla_test.run()
	# b = datetime.datetime.now()
	# retstr = "get once "
	
	lock.acquire()
	if(abs(ret-0.00277)<0.00001):
		# return True# retstr += "successfully! "
		ret_data.append(True)
	else:
		# return False# retstr += "failed! "
		ret_data.append(False)
	lock.release()
	# print retstr +"time used %d seconds."%((b-a).seconds)


def bunch_test(nthreads,ncycles,func):
	global ret_data
	ret_data = []

	a = datetime.datetime.now()
	# nthreads=50
	# ncycles=100
	for x in xrange(ncycles):
		threads = []
		for i in range(nthreads):
			t1 = threading.Thread(target=func)
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

if __name__ == '__main__':
	# oneGrpc()
	# oneRpcz()

	print "Test rpcz:"
	bunch_test(10,10,oneRpcz)

	print "Test Grpc:"
	bunch_test(10,10,oneGrpc)








