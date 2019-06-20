#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "client.hpp"

int main(int argc,char *argv[]){
    UDPPingClient c("localhost",33102);
    c.ping();
}
