import grpc
import grpc.framework.common.cardinality
import grpc.framework.interfaces.face.utilities as face_utilities

import dqmvoassetallocationservice_pb2 as dqmvoassetallocationservice__pb2
import dqmvoassetallocationservice_pb2 as dqmvoassetallocationservice__pb2


class CLAServiceStub(object):

  def __init__(self, channel):
    """Constructor.

    Args:
      channel: A grpc.Channel.
    """
    self.CLA = channel.unary_unary(
        '/echopb.CLAService/CLA',
        request_serializer=dqmvoassetallocationservice__pb2.MvoAssetAllocationInput.SerializeToString,
        response_deserializer=dqmvoassetallocationservice__pb2.MvoAssetAllocationOutput.FromString,
        )


class CLAServiceServicer(object):

  def CLA(self, request, context):
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')


def add_CLAServiceServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'CLA': grpc.unary_unary_rpc_method_handler(
          servicer.CLA,
          request_deserializer=dqmvoassetallocationservice__pb2.MvoAssetAllocationInput.FromString,
          response_serializer=dqmvoassetallocationservice__pb2.MvoAssetAllocationOutput.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'echopb.CLAService', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))
