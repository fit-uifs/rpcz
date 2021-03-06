// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: dqlib.proto

#ifndef RPCZ_dqlib_2eproto__INCLUDED
#define RPCZ_dqlib_2eproto__INCLUDED

#include <string>
#include <rpcz/service.hpp>

namespace google {
namespace protobuf {
class ServiceDescriptor;
class MethodDescriptor;
}  // namespace protobuf
}  // namespace google
namespace rpcz {
class rpc;
class closure;
class rpc_channel;
}  //namesacpe rpcz
#include "dqlib.pb.h"

namespace examples {
void rpcz_protobuf_AssignDesc_dqlib_2eproto();
void rpcz_protobuf_ShutdownFile_dqlib_2eproto();

class DQService_Stub;

class DQService : public rpcz::service {
 protected:
  // This class should be treated as an abstract interface.
  inline DQService() {};
 public:
  virtual ~DQService();

  typedef DQService_Stub Stub;

  static const ::google::protobuf::ServiceDescriptor* descriptor();

  virtual void CLAService(const ::examples::MvoAssetAllocationInput& request,
                       ::rpcz::reply< ::examples::MvoAssetAllocationOutput> response);

  // implements Service ----------------------------------------------

  const ::google::protobuf::ServiceDescriptor* GetDescriptor();
  void call_method(const ::google::protobuf::MethodDescriptor* method,
                  const ::google::protobuf::Message& request,
                  ::rpcz::server_channel* channel);
  const ::google::protobuf::Message& GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const;
  const ::google::protobuf::Message& GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(DQService);
};

class DQService_Stub {
 public:
  DQService_Stub(::rpcz::rpc_channel* channel, 
                   bool owns_channel=false);
  DQService_Stub(::rpcz::rpc_channel* channel, 
                   const ::std::string& service_name,
                   bool owns_channel=false);
  ~DQService_Stub();

  inline ::rpcz::rpc_channel* channel() { return channel_; }


  void CLAService(const ::examples::MvoAssetAllocationInput& request,
                       ::examples::MvoAssetAllocationOutput* response,
                       ::rpcz::rpc* rpc,                     ::rpcz::closure* done);
  void CLAService(const ::examples::MvoAssetAllocationInput& request,
                       ::examples::MvoAssetAllocationOutput* response,
                       long deadline_ms = -1);
 private:
  ::rpcz::rpc_channel* channel_;
  ::std::string service_name_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(DQService_Stub);
};


}  // namespace examples
#endif  // RPCZ_dqlib_2eproto__INCLUDED
