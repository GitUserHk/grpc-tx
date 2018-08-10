#ifndef __HEAD__GRPC__
#define __HEAD__GRPC__

#include <iostream>
#include <unistd.h>
#include <memory>
#include <string>
#include <stdio.h>
#include <grpc++/grpc++.h>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <stdlib.h>
#include <termios.h>
#include "INIReader.h"

#ifdef BAZEL_BUILD
#include "examples/protos/userinfo.grpc.pb.h"
#else
#include "userinfo.grpc.pb.h"
#endif

using namespace std;
using grpc::Status;
using userinfo::UserRequest;
using userinfo::UserReply;
using userinfo::SendMessage;

#define GRPC_IP "0.0.0.0"
#define GRPC_PORT "8080"

extern string g_GrpcIp ;
extern string g_GrpcPort ;
extern string g_ConfigFile;

extern log4cpp::Category* logger ;
extern string initFileName ;

#define PASS_MAX_LEN 100 

typedef struct UserInfo
{
	char account[PASS_MAX_LEN];	//用户账号
	char password[PASS_MAX_LEN];	//用户密码
 
}USERINFO ;

int readConfig();

#endif