import dqmvoassetallocationservice_pb2_grpc
import dqmvoassetallocationservice_pb2
import grpc
import dqvector_pb2 as dqvector__pb2
import dqmatrix_pb2 as dqmatrix__pb2
import time


def run():
  # creds = grpc.ssl_channel_credentials(grpc.ssl);

  # creds = grpc.ssl_channel_credentials(open('py.pem').read())#,open('server.key').read(),"")
  # channel = grpc.secure_channel('localhost:10000',creds)

  #channel = grpc.insecure_channel('115.29.15.17:10000')  
  #channel = grpc.insecure_channel('10.211.55.9:10000')  
  channel = grpc.insecure_channel('222.190.119.204:10027')  
  #channel = grpc.insecure_channel('localhost:10023')
  
  stub = dqmvoassetallocationservice_pb2_grpc.CLAServiceStub(channel)
  request = dqmvoassetallocationservice_pb2.MvoAssetAllocationInput()
  request.strategy = "MVOSTRANDARD"
  request.target_type = "MINVAR"
  request.asset_return.rows=2;
  request.asset_return.cols=2;
  request.asset_return.data.extend([0.00215,    -0.00062, -0.00062,0.00069])

  request.lower_bound.data.extend([             0,             0         ])
  request.upper_bound.data.extend([             1,             1         ])

  request.target_value = 0
  request.num_returns_for_mean = 2
  request.annualization_scale = 2
  request.num_ef_points = 10

  response =  stub.CLA(request)

  # print "weights:" ,response.weights
  # print response.target_value
  return response.target_value

if __name__ == '__main__':
  run()