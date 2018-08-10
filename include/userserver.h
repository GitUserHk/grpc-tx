#ifndef __HEAD_USERSERVER__
#define __HEAD_USERSERVER__

#include "grpc.h"
#include <hiredis/hiredis.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

#define REDIS_IP        "127.0.0.1"
#define REDIS_PORT	    6379
#define REDIS_PASSWORD  "123456"

// Logic and data behind the server's behavior.
class UserInfoServiceImpl final : public SendMessage::Service {
public:
  Status sendMessage(ServerContext* context, const UserRequest* request , UserReply* reply) ;

public:
  int readRedisConfig();
public:
  bool Connect();
  void disConnect();
  bool setString(const string & data);
  bool setString(const string & key , const string & value);
  bool getString(const string & key , string & value );
private:
  bool isError();
private:
  redisContext * _context;
  redisReply * _reply;

private:
  string redisIp;
  long redisPort;
  string redisPassword;
  map<string , string> userMap;
  map<string,string>::iterator userMapIterator;
};

#endif