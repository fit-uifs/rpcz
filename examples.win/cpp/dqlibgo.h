#pragma once




//typedef unsigned char Byte;

extern "C"  int add(int a, int b);

//extern "C" _declspec(dllexport) std::string ProcessRequest(const std::string& req_type, const std::string& req_arg_msg);

extern "C"  
#ifdef _WIN32 || WIN32
_declspec(dllexport)
#endif
void ProcessRequest(char* req_type, unsigned char* req_arg_msg,int length, unsigned char* out, int* out_length);

extern "C" void for_regist();