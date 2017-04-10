# CMake generated Testfile for 
# Source directory: Y:/Documents/work-ubuntu/rpcz-master/test
# Build directory: Y:/Documents/work-ubuntu/rpcz-master/build-win/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(callback_test "callback_test")
add_test(connection_manager_test "connection_manager_test")
add_test(client_server_test "client_server_test")
add_test(application_test "application_test")
subdirs(proto)
