#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "disconnect.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

char *build_disconnect(char * disconnect_message){
    disconnect_message = malloc(sizeof(char)*2);
    disconnect_message[0] = 0xE0;
    disconnect_message[1] = 0x00;

    return disconnect_message;
}