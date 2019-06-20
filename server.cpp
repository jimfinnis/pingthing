#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "server.hpp"

int main(int argc,char *argv[]){
    UDPPingServer s(33102);
    if(s.init()){
        for(;;){
            s.poll();
            sleep(1);
            std::cout << s.getTime() << std::endl;
        }
    }
    
}
