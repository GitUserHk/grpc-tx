#include "grpc.h"
#include "userclient.h"

string g_GrpcIp= GRPC_IP ;
string g_GrpcPort= GRPC_PORT ;
string g_ConfigFile="../conf/grpc_config.ini";

log4cpp::Category* logger ;
string initFileName = "../conf/log4cpp.properties";

int main(int argc, char** argv) {

  log4cpp::PropertyConfigurator::configure(initFileName);
  logger = &(log4cpp::Category::getInstance(std::string("client")));

  logger->info("read config file");
  if(readConfig())
    logger->info("use default value");

  string server_address(g_GrpcIp+":"+g_GrpcPort);
  UserClient userClient(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

  int running = 1;
  while(running)
  {
    switch(userClient.GetMenuChoose())
    {
      case '0':
        running = 0;
        break;
      case '1':
        userClient.UserLogin();
        break;
      case '2':
        userClient.RegisterUser();
        break;
      default:
        running = 0;
    }
    getc(stdin);
  }

  return 0;
}
