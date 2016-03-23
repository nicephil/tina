#define TAG "smartlinkd-demo"
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <tina_log.h>
#include "connect.h"

int debug_enable = 1;

int onRead(char* buf,int length)
{
    if(length == THREAD_INIT){
        
    }
    else if(length == -1){
        
    }else if(length == 0){
        TLOGD("server close the connection...\n");
        exit(0);
        return THREAD_EXIT;

    }else {
        TLOGD("lenght: %d\n",length);
        //printf_info((struct _cmd *)buf);
        struct _cmd* c = (struct _cmd *)buf;
        if(c->cmd == FAILED){
            TLOGD("response failed\n");
            return THREAD_CONTINUE;
        }
        if(c->info.protocol == PROTO_FAIL){
            TLOGD("proto scan fail");
            return THREAD_CONTINUE;
        }
        TLOGD("cmd: %d\n",c->cmd);
        TLOGD("pcol: %d\n",c->info.protocol);
        TLOGD("ssid: %s\n",c->info.base_info.ssid);
        TLOGD("pasd: %s\n",c->info.base_info.password);
        TLOGD("security: %d\n",c->info.base_info.security);

        if(c->info.protocol == PROTO_AKISS)
            TLOGD("radm: %d\n",c->info.airkiss_random);
        if(c->info.protocol == PROTO_COOEE){
            TLOGD("ip: %s\n",c->info.ip_info.ip);
            TLOGD("port: %d\n",c->info.ip_info.port);
        }
    }
    return THREAD_CONTINUE;
}

int main(int argc, char* argv[])  
{  
    int proto = 0;
    if(argc > 1){
        proto = atoi(argv[1]);
    }

    aw_smartlinkd_prepare();
    if(aw_smartlinkd_init(0,onRead) == 0){
        if(proto == 0){
            TLOGD("start airkiss\n");
            aw_startairkiss();
        }
        else if(proto == 1){
            TLOGD("start cooee\n");
            aw_startcooee();
        }
    }
    sleep(1);
    aw_stopcooee();
    sleep(2);
    aw_startcooee();
    sleep(1);
    aw_stopcooee();
    sleep(1);
    aw_startcooee();
    sleep(1);
    aw_startairkiss();
    sleep(1);
    aw_stopairkiss();
    sleep(1);
    aw_startairkiss();
    
    while(1);

    return 0;  
}
