#include "grpc.h"
#include "userserver.h"


string g_GrpcIp = GRPC_IP ;
string g_GrpcPort= GRPC_PORT ;
string g_ConfigFile="../conf/grpc_config.ini";

log4cpp::Category* logger ;
string initFileName = "../conf/log4cpp.properties";

void RunServer() {
  string server_address(g_GrpcIp+":"+g_GrpcPort);
  UserInfoServiceImpl service;
  if(service.readRedisConfig()){
     logger->error("read redis config failed");
     return ;
  }
  if(! service.Connect()){
    return;  
  }

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  logger->info("Server listening on %s" ,  server_address.c_str() );

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
    log4cpp::PropertyConfigurator::configure(initFileName);
    logger = &(log4cpp::Category::getInstance(string("server")));
    
    logger->info("read config file");
    readConfig();
    
    logger->info("Run server ...");

    RunServer();

    return 0;
}
