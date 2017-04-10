include(rpcz_functions)

include_directories("/media/psf/Home/Documents/work-ubuntu/rpcz-master/build/examples")
#include_directories("/media/psf/Home/Documents/work-ubuntu/rpcz-master/build/examples/cpp/")
include_directories("/media/psf/Home/Documents/work-ubuntu/rpcz-master-linux-ok/include/rpcz")

find_package(ProtobufPlugin REQUIRED)
PROTOBUF_GENERATE_CPP(SEARCH_PB_SRCS SEARCH_PB_HDRS ../common/search.proto)
PROTOBUF_GENERATE_RPCZ(SEARCH_RPCZ_SRCS SEARCH_RPCZ_HDRS ../common/search.proto)

#SET (SEARCH_PB_SRCS
#        search.pb.cc
#        )
#SET (SEARCH_PB_HDRS
#        search.pb.h
#        )
#SET (SEARCH_RPCZ_SRCS
#        search.rpcz.cc
#        )
#SET (SEARCH_RPCZ_HDRS
#        search.rpcz.h
#        )
SET (SEARCH_PB_SRC
        search.pb.h
        search.pb.cc
        search.rpcz.h
        search.rpcz.cc
        dqlib.pb.h
        dqlib.pb.cc
        dqlib.rpcz.h
        dqlib.rpcz.cc
        inirw.c
        inirw.h

        )

#add_library(example_pb ${SEARCH_PB_SRCS} ${SEARCH_PB_HDRS} ${SEARCH_RPCZ_SRCS}
#                      ${SEARCH_RPCZ_HDRS})
add_library(example_pb ${SEARCH_PB_SRC} )
target_link_libraries(example_pb  rt ${PROTOBUF_LIBRARY})
include_directories(${PROJECT_BINARY_DIR}/examples)



add_executable(example_server example_server.cc)
target_link_libraries(example_server rpcz example_pb rt dl)


add_executable(example_client example_client.cc)
target_link_libraries(example_client rpcz example_pb rt pthread ${PROJECT_BINARY_DIR}/examples/cpp/libbenchmark.a)