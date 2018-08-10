#include "grpc.h"

int readConfig()
{
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
    g_GrpcIp = reader.Get("server", "ip", GRPC_IP );
    g_GrpcPort = reader.Get("server", "port", GRPC_PORT );
    return 0;
}


