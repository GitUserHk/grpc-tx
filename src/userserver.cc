#include "userserver.h"


Status UserInfoServiceImpl::sendMessage(ServerContext* context, const UserRequest* request,
                  UserReply* reply) {

    string t_account = request->account();
    string t_password = request->password();
    int t_operate = request->operate();
    int flag = 0;
    string t_value ;

    logger->info("account : %s" ,  t_account.c_str() );
    logger->info("password : %s" ,  t_password.c_str() );
    if(t_operate == 1){
        userMapIterator = userMap.find(t_account) ;
        if ( userMapIterator != userMap.end()){
            logger->info("map password %s",userMapIterator->second.c_str());
            if (t_password != userMapIterator->second) {
                flag = -2 ;   // password error
                goto handleOver;     
            }
            goto handleOver;
        }
        if( ! getString (t_account , t_value) )  
        {
            flag = -1 ;   //account not exsit
            goto handleOver;            
        }
        logger->info("t_value %s",t_value.c_str());
        if (t_value != t_password) {
            flag = -2;  // password error
            goto handleOver;     
        }
        logger->info("flag %d",flag);
        userMap.insert(pair<string,string>(t_account , t_password ) );
    }
    else if(t_operate == 2){
        userMapIterator = userMap.find(t_account) ;
        if ( userMapIterator != userMap.end()){
            flag = -3;  // account repeat
            goto handleOver; 
        }
        if(! getString (t_account , t_value) )  
        {
            if(! setString(t_account , t_password) ){
                flag = -4;   // update redis failed
                goto handleOver; 
            }
            userMap.insert(pair<string,string>(t_account , t_password) );
            flag = 1;  // register success
            goto handleOver; 
        }
    }
    else{
        flag = -5;   // operate type except
        goto handleOver; 
    }
handleOver:
    reply->set_flag(flag); 
    return Status::OK;
}

int UserInfoServiceImpl::readRedisConfig(){
    if( access( g_ConfigFile.c_str() , 0 ) == -1 ){
       logger->error( "File %s is not exists " , g_ConfigFile.c_str() );
       return -1;
    }
    /* Check for write permission */
    if( access( g_ConfigFile.c_str() , 2 ) == -1 ){
       logger->error( "File %s no write permission " , g_ConfigFile.c_str() );
       return -1;
    }

    INIReader reader(g_ConfigFile);

    if (reader.ParseError() < 0) {
        logger->error("Can't load %s" , g_ConfigFile.c_str());
        return -1;
    }
    redisIp = reader.Get("redis", "ip", REDIS_IP );
    redisPort = reader.GetInteger("redis", "port", REDIS_PORT );
    redisPassword = reader.Get("redis", "password", REDIS_PASSWORD );

    return 0;
}

bool UserInfoServiceImpl::Connect()
{
    _context = redisConnect(redisIp.c_str(), redisPort);
    if(_context && _context->err)
    {
        logger->error("connect redis error");
        return false;    
    }
    string command = "AUTH " + redisPassword;  
    setString(command);
    logger->info("redis Connect success");
    return true;
}

void UserInfoServiceImpl::disConnect()
{
    ::redisFree(_context);
    logger->info("redis disConnect success");
}

bool UserInfoServiceImpl::setString(const string & data)
{
    _reply = (redisReply*)::redisCommand(_context, data.c_str());
    if(!isError())
    {
        if (!(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK") == 0))
        {
            logger->info("Failed to execute SET(%s)",data.c_str());
            return false;
        }    
    }
    return true;
}


bool UserInfoServiceImpl::setString(const string & key , const string & value)
{
    string data = "SET " + key + " " + value;

    logger->info("set date %s",data.c_str());
    _reply = (redisReply*)::redisCommand(_context, data.c_str());
    if(!isError())
    {
        if (!(_reply->type == REDIS_REPLY_STATUS && strcasecmp(_reply->str,"OK") == 0))
        {
            logger->error("Failed to execute SET(%s)",data.c_str());
            return false;
        }    
    }
    return true;
}

bool UserInfoServiceImpl::getString(const string & key , string & value)
{
    _reply = (redisReply*)::redisCommand(_context, "GET %s", key.c_str());
    if(!isError() && _reply->type == REDIS_REPLY_STRING)
    {
        value = _reply->str;
        logger->info("get value %s",value.c_str());
        return true;
    }
    return false;
}

bool UserInfoServiceImpl::isError()
{
    if(NULL == _reply)
    {
        disConnect();
        Connect();
        return true;
    }
    return false;
}