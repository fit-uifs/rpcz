import os
import compiler
import shutil
from distutils.core import Command
from distutils.command import build as build_module
from distutils.extension import Extension
from distutils.core import setup

BUILD_DIR='../build'

def _build_rpcz_proto():
    compiler.generate_proto('../src/rpcz/proto/rpcz.proto', 'rpcz')


def _build_test_protos():
    compiler.generate_proto('../test/proto/search.proto', 'tests')
    compiler.generate_proto(
            '../test/proto/search.proto', 'tests',
            with_plugin='python_rpcz', suffix='_rpcz.py',
            plugin_binary=
                BUILD_DIR + '/src/rpcz/plugin/python/protoc-gen-python_rpcz')


class build(build_module.build):
    def run(self):
        _build_rpcz_proto()
        _build_test_protos()
        shutil.copy('compiler.py', 'rpcz')
        build_module.build.run(self)


class gen_pyext(Command):
    user_options = []
    def initialize_options(self):
        pass
    def finalize_options(self):
        pass
    def run(self):
        if os.system('cython --cplus cython/pywraprpcz.pyx') != 0:
            raise IOError("Running cython failed.")


setup(
    name = "rpcz",
    version = "0.9",
    author = "Nadav Samet",
    author_email = "nadavs@google.com",
    description = "An RPC implementation for Protocol Buffer based on ZeroMQ",
    license = "Apache 2.0",
    keywords = "protocol-buffers rpc zeromq 0mq",
    packages=['rpcz', 'tests'],
    url='http://code.google.com/p/rpcz/',
    long_description='',
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Topic :: Software Development :: Libraries :: Python Modules",
        "License :: OSI Approved :: Apache Software License",
    ],
    cmdclass = {
        'build': build,
        'gen_pyext': gen_pyext,
    },
    ext_modules=[
        Extension("rpcz.pywraprpcz", ["cython/pywraprpcz.cpp"],
                  libraries=["crypt32","Advapi32","rpcz","example_pb","libsodium","iphlpapi","ws2_32","libzmq",'libboost_thread-vc140-mt-1_59','libboost_program_options-vc140-mt-1_59','libboost_system-vc140-mt-1_59','libboost_date_time-vc140-mt-1_59','libboost_chrono-vc140-mt-1_59','libboost_atomic-vc140-mt-1_59','libprotobuf'],
                  include_dirs=['../include','../src','Y:/Documents/work-ubuntu/dqlib_dev/external/boost_1_59_0','Y:/Documents/work-ubuntu/dqlib_dev/external/protobuf-3.1.0/include', BUILD_DIR + '/src'],
                  library_dirs=[ BUILD_DIR + '/deps/lib', BUILD_DIR + '/src/rpcz','C:/boost_1_59_0_32/lib32-msvc-14.0','Y:/Documents/work-ubuntu/dqlib_dev/external/protobuf-3.1.0/vsprojects/release','Y:/Documents/work-ubuntu/rpcz/build-win/examples/cpp/Release','Y:/Documents/work-ubuntu/libzmq/bin/Win32/Release/v140/static','Y:/Documents/work-ubuntu/libsodium/Build/Release/Win32'],
                  extra_compile_args=[ '/Zc:wchar_t','/fp:precise','/D', '\"_WIN32\"', '/EHsc'],
                  extra_link_args=[],# '/nologo','/GL', ,'/D','\"_WINDLL\"''-std=c++11','/GS',  '/analyze-', '/W4', '/Gy', '/Zi', '/Gm-', '/O2', '/sdl', '/Oy-','/Zc:inline','/D','\"_MBCS\"', '/D','\"WIN32\"', '/WX-', '/Zc:forScope', '/Gd' ,'/Oy-', '/Oi', '/MD'  ,
                  language='c++')
    ],
)
