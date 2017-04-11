include(rpcz_functions)

if(WIN32)
set(Protobuf_INCLUDE_DIR Y:/Documents/work-ubuntu/dqlib_dev/external/protobuf-3.1.0/include)
set(Protobuf_LIBRARY_DIR Y:/Documents/work-ubuntu/dqlib_dev/external/protobuf-3.1.0/libs/x64/debug)
else()
endif()
find_package(Protobuf REQUIRED)

#find_package(ProtobufPlugin REQUIRED)
#PROTOBUF_GENERATE_CPP(SEARCH_PB_SRCS SEARCH_PB_HDRS ../common/search.proto)
#PROTOBUF_GENERATE_RPCZ(SEARCH_RPCZ_SRCS SEARCH_RPCZ_HDRS ../common/search.proto)
INCLUDE_DIRECTORIES(${Protobuf_INCLUDE_DIR})
INCLUDE_DIRECTORIES("Y:/Documents/work-ubuntu/rpcz/src/rpcz")
INCLUDE_DIRECTORIES("Y:/Documents/work-ubuntu/rpcz/include/rpcz")

SET (SEARCH_PB_SRC
        search.pb.h
        search.pb.cc
        search.rpcz.h
        search.rpcz.cc
        dqlib.pb.h
        dqlib.pb.cc
        dqlib.rpcz.h
        dqlib.rpcz.cc

        )
add_library(example_pb ${SEARCH_PB_SRC} )
target_link_libraries(example_pb  rt ${PROTOBUF_LIBRARY})
include_directories(${PROJECT_BINARY_DIR}/examples)

add_executable(example_client example_client.cc)
target_link_libraries(example_client rpcz example_pb rt Y:/Documents/work-ubuntu/libzmq/bin/x64/Debug/v140/static/libzmq.lib)

add_executable(example_server example_server.cc)


    target_link_libraries(example_server
        ${Boost_LIBRARIES}
        ${PROTOBUF_LIBRARY}

        rpcz
        example_pb

        rt
        Y:/Documents/work-ubuntu/libzmq/bin/x64/Debug/v140/static/libzmq.lib

    )

