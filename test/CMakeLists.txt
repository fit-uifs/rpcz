add_subdirectory(proto)
set(CTEST_OUTPUT_ON_FAILURE 1)

#gtest
include_directories("/media/psf/Home/Documents/work-ubuntu/googletest-release-1.8.0/googletest/include/")
#glog
include_directories("/media/psf/Home/Documents/work-ubuntu/glog-0.3.4/src/")
include_directories("/media/psf/Home/Documents/work-ubuntu/rpcz-master-linux-ok/include/rpcz")

include_directories(${PROJECT_BINARY_DIR}/test)
include_directories(${PROJECT_SOURCE_DIR}/src)
add_library(rpcz_gtest_main rpcz_gtest_main.cc)

include(build_deps)
#BuildDependency(
#    GLog
#    http://google-glog.googlecode.com/files/glog-0.3.1-1.tar.gz
#    GLOG
#    libglog)

ExternalProject_Add(
    GTest
    URL http://googletest.googlecode.com/files/gtest-1.6.0.zip
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(GTest binary_dir)
ExternalProject_Get_Property(GTest source_dir)
include_directories(${source_dir}/include)    
link_directories(${binary_dir})

function(rpcz_test TESTNAME)
  CMAKE_PARSE_ARGUMENTS(OPTIONS "" "" "SRCS;LIBS" ${ARGN})
  add_executable(${TESTNAME} ${OPTIONS_SRCS})
  set(LIBS rpcz rpcz_gtest_main
          /media/psf/Home/Documents/work-ubuntu/googletest-release-1.8.0/cmake-build-debug/googlemock/gtest/libgtest.a
          /usr/local/lib/libglog.so
          pthread#${GLOG_LIBRARIES}
  )
  list(APPEND LIBS ${OPTIONS_LIBS})
  target_link_libraries(${TESTNAME} ${LIBS})
  add_test(${TESTNAME} ${TESTNAME})
endfunction()

rpcz_test(callback_test SRCS callback_test.cc)
rpcz_test(connection_manager_test SRCS connection_manager_test.cc)
rpcz_test(client_server_test SRCS client_server_test.cc LIBS search_pb)
rpcz_test(application_test SRCS application_test.cc LIBS search_pb)
