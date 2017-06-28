cmake_minimum_required(VERSION 3.6)
project(python)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    rpcz/__init__.py
    rpcz/pywraprpcz.so
    rpcz/rpc.py
    rpcz/service.py
    rpcz/service_reflection.py
    tests/__init__.py
    compiler.py
    example_client.py
    example_server.py
    librpcz.so
    setup.py)

add_executable(python ${SOURCE_FILES})