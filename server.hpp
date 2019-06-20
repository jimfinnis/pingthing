#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <strings.h>
#include <errno.h>

#include "msg.h"

class UDPPingServer {
    int port;
    int fd;
    timespec lastTime;
    
    static inline double time_diff(timespec start, timespec end)
    {
        timespec temp;
        if ((end.tv_nsec-start.tv_nsec)<0) {
            temp.tv_sec = end.tv_sec-start.tv_sec-1;
            temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
        } else {
            temp.tv_sec = end.tv_sec-start.tv_sec;
            temp.tv_nsec = end.tv_nsec-start.tv_nsec;
        }
        
        double t = temp.tv_sec;
        double ns = temp.tv_nsec;
        t += ns*1e-9;
        return t;
    }
    
public:
    UDPPingServer(int port_){
        port = port_;
        fd=-1;
    }
    
    bool init(){
        if(fd>=0){
            perror("port already open");
            return false;
        }
        
        clock_gettime(CLOCK_MONOTONIC,&lastTime);
        
        sockaddr_in addr;
        fd = socket(AF_INET,SOCK_DGRAM,0);
        if(fd<0){
            perror("cannot open socket");
            return false;
        }
        
        bzero(&addr,sizeof(addr));
        addr.sin_family=AF_INET;
        addr.sin_addr.s_addr=htonl(INADDR_ANY);
        addr.sin_port=htons(port);
        if(bind(fd,(sockaddr *)&addr,sizeof(addr))){
            perror("cannot bind socket");
            close(fd);
            fd=-1;
            return false;
        }
        return true;
    }
    
    void shutdown(){
        if(fd>=0)
            close(fd);
        fd=-1;
    }
    
    
    void poll(){
        struct pingmsg msg;
        sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(fd,&msg,sizeof(msg),
                         MSG_DONTWAIT,
                         (struct sockaddr *)&cliaddr,&len); 
        if(n>0){
            clock_gettime(CLOCK_MONOTONIC,&lastTime);
        }
    }
    
    double getTime(){
        timespec t;
        clock_gettime(CLOCK_MONOTONIC,&t);
        return time_diff(lastTime,t);
    }
    
};
