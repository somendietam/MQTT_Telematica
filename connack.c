#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "connack.h"

#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

void printbuffer(char arr[], size_t size) {
    printf("\n");
    for (int i = 0; i<size; ++i) {
        char c = arr[i];
        for (int j = 7; j >= 0; --j) {
            putchar((c & (1 << j)) ? '1' : '0');
        }
        putchar(' ');
    }
    putchar('\n');
}


int search_flag_connect(char buffer[]){
   int contador = 0;
    
    for (int i = 1; i <= 4; i++) {
        // Comprobamos el primer bit del byte actual
        if (buffer[i] & 0x80) { // 0x80 es 10000000 en binario
            // El primer bit es 1
            contador++;
        } else {
            // El primer bit es 0, salimos del ciclo
            break;
        }
    }

    int pos_flag_connect = (contador + 10)-1; // Sumamos 2 al contador ya que tenemos el byte de fixed header y el ultimo Byte del remaining length que era el que comenzaba en 1
    //y sumamos 8 que son los siguientes bytes de protocolo mqtt, en total 2+8=10
    
    char bin_flag_connect = buffer[pos_flag_connect];

    printf("El valor de posicion_flag es: %d\n", pos_flag_connect);

    int flag_clean_ssn = 0;

    // Desplazar el bit 1 posición a la derecha para acceder al bit deseado
    char bit = (bin_flag_connect >> 1) & 1;

    // Imprimir el resultado
    if (bit == 1)
        flag_clean_ssn = 1;

    printf("el flag clean session es: %d\n", flag_clean_ssn);
    //printf("El carácter es: %c\n", bin_flag_connect);
    // Iterar sobre cada bit del byte (char)
    for (int i = 7; i >= 0; i--) {

        // Verificar si el bit en la posición i está encendido
        if (bin_flag_connect & (1 << i))
            printf("1");
        else
            printf("0");
    }
    printf("\n");

    return flag_clean_ssn;
}


char *build_connack(char *connack_message, struct fixed_header *fixed_header, struct connack_variable_header *variable_header, int flag_clean_ssn){
    connack_message = malloc(sizeof(char)*4);
    fixed_header->control_packet_type = 0x20;
    fixed_header->remaining_length= 0x02;
    
    if (flag_clean_ssn == 1){
        variable_header->flags = 0x00;
    }else{
        variable_header->flags = 0x01;
    }

    variable_header->return_code = 0x00;

    connack_message[0] = fixed_header->control_packet_type;
    connack_message[1] = fixed_header->remaining_length;
    connack_message[2] = variable_header->flags;
    connack_message[3] = variable_header->return_code;

    printf("\n");
    printf("Este es el CONNACK: \n");
    printbuffer(connack_message, 4);

    return connack_message;
    
}