#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include "msg.h"

class UDPPingClient {
    int port;
    char hostName[1024];
    sockaddr_in servaddr;
    bool validaddr;
    
public:
    UDPPingClient(const char *host, int port_){
        strcpy(hostName,host);
        port = port_;
        addrinfo *res;
        bzero(&servaddr,sizeof(servaddr));
        validaddr=false;
        
        int rv = getaddrinfo(host,NULL,NULL,&res);
        if(rv){
            printf("Could not get host address %s: %s\n",
                   host,gai_strerror(rv));
        } else {
            servaddr = *(sockaddr_in*)res->ai_addr;
            servaddr.sin_port = htons(port);
            validaddr=true;
        }
    }
    
    bool ping(){
        if(!validaddr)return false;
        int fd = socket(AF_INET,SOCK_DGRAM,0);
        
        if(fd<0){
            perror("cannot open socket");
            return false;
        }
        
        struct pingmsg msg;
        if (sendto(fd, &msg, sizeof(msg), 0,
                   (sockaddr*)&servaddr, sizeof(servaddr)) < 0){
            perror("cannot send message");
            close(fd);
            return false;
        }
        close(fd);
        return true;
    }
};
