add_subdirectory(plugin)

protobuf_generate_cpp(RPCZ_PB_SRCS RPCZ_PB_HDRS proto/rpcz.proto)
set(PROTO_SOURCES ${RPCZ_PB_SRCS} ${RPCZ_PB_HDRS})

find_package(ZeroMQ REQUIRED)

include_directories(${ZeroMQ_INCLUDE_DIRS})
include_directories("../../include/rpcz")
include_directories(${PROJECT_BINARY_DIR})

set(RPCZ_SOURCES
    application.cc clock.cc connection_manager.cc
    reactor.cc rpc.cc rpc_channel_impl.cc server.cc
    sync_event.cc zmq_utils.cc
    ${PROTO_SOURCES})
set(RPCZ_LIB_DEPS ${ZeroMQ_LIBRARIES}
                  ${PROTOBUF_LIBRARIES}
                  ${Boost_LIBRARIES})

if (RPCZ_ENABLE_IPV6)
	set_source_files_properties(
		connection_manager.cc
		PROPERTIES COMPILE_FLAGS -DRPCZ_ENABLE_IPV6=1
	)
endif()

if(rpcz_build_static)
    add_library(rpcz STATIC ${RPCZ_SOURCES})
else()
    add_library(rpcz SHARED ${RPCZ_SOURCES})
endif()

target_link_libraries(rpcz ${RPCZ_LIB_DEPS})
set(RPCZ_TARGET_LIBS rpcz)

if(rpcz_build_python_support OR rpcz_install_python_support)
    add_custom_command(
      TARGET rpcz
      POST_BUILD
      COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/../../python/setup.py" build
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../python/"
      COMMENT "Building python support..."
      VERBATIM
    )
    if(rpcz_install_python_support)
        add_custom_command(
          TARGET rpcz
          POST_BUILD
          COMMAND pip install -e . 
          WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../python/"
          COMMENT "Installing python support..."
          VERBATIM
        )
    endif(rpcz_install_python_support)
endif(rpcz_build_python_support OR rpcz_install_python_support)

add_executable(zsendrpc zsendrpc.cc)
target_link_libraries(zsendrpc rpcz ${Boost_PROGRAM_OPTIONS_LIBRARIES})

install(TARGETS ${RPCZ_TARGET_LIBS} zsendrpc
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install( FILES ${RPCZ_PB_HDRS} DESTINATION include/rpcz )

