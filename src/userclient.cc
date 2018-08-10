#include "userclient.h"


UserClient::UserClient(shared_ptr<Channel> channel)
    : stub_(SendMessage::NewStub(channel)) {

}

// Assembles the client's payload, sends it and presents the response back
// from the server.
int UserClient::sendMessage(const string& account , const string& password , const int operate) {
  // Data we are sending to the server.
  UserRequest request;
  request.set_account(account);
  request.set_password(password);
  request.set_operate(operate);

  int flag = 0 ; 
  // Container for the data we expect from the server.
  UserReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  ClientContext context;

  // The actual RPC.
  Status status = stub_->sendMessage(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    flag = reply.flag();
    switch (flag){
      case 0:
        printf("登录成功！\n");
        break;
      case 1:
        printf("注册成功！\n");
        break;
      case -1:
        printf("账号不存在！\n");
        break;
      case -2:
        printf("密码错误!\n");
        break;
      case -3:
        printf("账号已存在！\n");
        break;
      case -4:
        printf("更新数据库失败！\n");
        break;
      default:
        printf("其他错误！\n");
        break;       
    }
    return flag;
  } else {
    logger->error("error_code: %d , error_message : %s ",status.error_code() , 
      status.error_message().c_str());
    logger->error("reply flag : %d",reply.flag());
    return -1;
  }
}

void UserClient::ShowMenu()
{
  system("clear");
  printf("-----GRPC登录/注册----\n");
  printf("\n\t1. 登录   \n");
  printf("\t2. 注册   \n");
  printf("\t0. 退出系统\n");
  printf("\n--------------------\n");
}
 
int UserClient::GetMenuChoose()
{
  int num;
  ShowMenu();
  printf("请选择菜单(0 ~ 2):");
        num = getchar();
  while( num < '0' || num > '2' )
  {
    ShowMenu();
    printf("选择菜单错误，请重新选择(0 ~ 2):");
    fflush(stdin);//清空输入缓冲区
    num = getchar();
  }
  return num;
}

int UserClient::mygetch()
{
  struct termios oldt,newt;
  int ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}

int UserClient::CreateUserInfo( int flag)
{
  char pass1[PASS_MAX_LEN]={} , pass2[PASS_MAX_LEN] = {};
  char temp;
  int i = 0 , j = 0;

  system("clear");
  printf("\t\t\t\t请输入账号：");
  while(i < PASS_MAX_LEN)
  {
    UserInfo.account[i] = getchar();
    if(UserInfo.account[i] == 10){
      if(j == 0){
        j++;
        continue;
      }
      break;
    }
    i++;
  }
  UserInfo.account[i] = '\0';
  printf("\n\t\t\t\t请输入密码：");
  i = 0 ;
  j = 0 ;
  while(i < PASS_MAX_LEN)
  {
    pass1[i] = mygetch();
    if(pass1[i] == 10)
    {
      break;
    }
    printf("*");
    i++;
  }
  pass1[i] = '\0';
  
  if(flag == 2) {
    i = 0;
    printf("\n\n\t\t\t\t请确认您的密码：");
    while(i < PASS_MAX_LEN)
    {
      pass2[i] = mygetch();
      if(pass2[i] == 10) break;
      printf("*");
      i++;
    }
    pass2[i] = '\0';
  
    system("clear");
    printf("\n\n\n\n\t\t\t正在注册......\n");
    sleep(2);
  
    if(strcmp(pass1, pass2) != 0)
    {
      printf("\n\n\t\t\t\t密码输入有误！请重新注册！\n");
      sleep(2);
      return -9;
    }
  }
  strncpy(UserInfo.password, pass1,PASS_MAX_LEN);
  printf("\n");
}

void UserClient::MessagePackage(int operate)
{
  memset(&UserInfo , 0 , sizeof(USERINFO));
  CreateUserInfo( operate );

  string account(UserInfo.account);
  string password(UserInfo.password);
  int reply = sendMessage(account , password , operate );
  if(reply == 0 ){
    logger->info("operate success !");
  }
}

void UserClient::RegisterUser()
{
  MessagePackage(2);
}
 
void UserClient::UserLogin()
{
  MessagePackage(1);
}
