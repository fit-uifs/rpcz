# Install script for directory: Y:/Documents/work-ubuntu/rpcz-master

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/rpcz")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rpcz" TYPE FILE FILES
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/application.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/callback.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/connection_manager.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/macros.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/rpc.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/rpc_channel.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/rpcz.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/server.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/service.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/sync_event.hpp"
    "Y:/Documents/work-ubuntu/rpcz-master/include/rpcz/zmq_utils.hpp"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/rpcz" TYPE FILE FILES "Y:/Documents/work-ubuntu/rpcz-master/build-win/src/rpcz/rpcz.pb.h")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("Y:/Documents/work-ubuntu/rpcz-master/build-win/src/cmake_install.cmake")
  include("Y:/Documents/work-ubuntu/rpcz-master/build-win/test/cmake_install.cmake")
  include("Y:/Documents/work-ubuntu/rpcz-master/build-win/examples/cpp/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "Y:/Documents/work-ubuntu/rpcz-master/build-win/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
