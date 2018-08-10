#ifndef __HEAD_USERCLIENT__
#define __HEAD_USERCLIENT__

#include "grpc.h"

using grpc::Channel;
using grpc::ClientContext;

class UserClient {
 public:
  UserClient(shared_ptr<Channel> channel);

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int sendMessage(const string& account , const string& password , const int operate) ;
  int mygetch();
  void ShowMenu(); 
  int GetMenuChoose();
  int CreateUserInfo(int flag);
  void RegisterUser();
  void UserLogin();
  void MessagePackage( int operate);
 
 private:
   unique_ptr<SendMessage::Stub> stub_;
   USERINFO UserInfo;
};

#endif