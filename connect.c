#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "connect.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

// Función para construir el paquete CONNECT
char *build_connect_packet(char *connect_message, size_t *connect_length, struct connect_fixed_header *fixed_header, struct connect_variable_header *variable_header, struct connect_payload *connect_payload) {

    connect_payload->clientID = malloc(BUFFER_SIZE);
    connect_payload->username = malloc(BUFFER_SIZE);
    connect_payload->password = malloc(BUFFER_SIZE);

    printf("Ingrese Client ID: ");
    getline(&connect_payload->clientID, connect_length, stdin);
    while(getchar() != '\n');
    connect_payload->clientID[strcspn(connect_payload->clientID, "\n")] = '\0'; // Eliminar el carácter de nueva línea

    printf("Ingrese nombre de usuario: ");
    getline(&connect_payload->username, connect_length, stdin);
    while(getchar() != '\n');
    connect_payload->username[strcspn(connect_payload->username, "\n")] = '\0'; // Eliminar el carácter de nueva línea

    printf("Ingrese contraseña: ");
    getline(&connect_payload->password, connect_length, stdin);
    while(getchar() != '\n');
    connect_payload->password[strcspn(connect_payload->password, "\n")] = '\0'; // Eliminar el carácter de nueva línea

    // Construir el encabezado fijo (Fixed Header)
    fixed_header->control_packet_type = 0x10; // CONNECT
    printf("Este es el control packet type: %d\n", fixed_header->control_packet_type);

    variable_header->protocol_name[0] = 'M';
    variable_header->protocol_name[1] = 'Q';
    variable_header->protocol_name[2] = 'T';
    variable_header->protocol_name[3] = 'T';

    variable_header->protocol_name_length[0] = 0;
    variable_header->protocol_name_length[1] = 4;
    variable_header->protocol_name_length[strcspn(variable_header->protocol_name_length, "\n")] = '\0'; // Eliminar el carácter de nueva línea

    printf("Este es el protocol name en binario");
    printbinario(variable_header->protocol_name);

    printf("Este es el protocol name length %d, %d\n", variable_header->protocol_name_length[0], variable_header->protocol_name_length[1]);
    printf("Este es el protocol name length en binario");
    printbinario2(variable_header->protocol_name_length);

    variable_header->protocol_version = 0x04; // Versión 3.1.1
    printf("Este es el protocol version: %d\n", variable_header->protocol_version);
    // Inicialmente, todos los bits a 0
    variable_header->connect_flags = 0xC2;

    printf("Este es el connect flags: %d\n", variable_header->connect_flags);

    variable_header->keep_alive[0] = 0; // Keep Alive de 60 segundos
    variable_header->keep_alive[1] = 60;

    int clientlength = strlen(connect_payload->clientID);
    connect_payload->client_id_length[0] = (clientlength >> 8) & 0xFF; // Byte más significativo
    connect_payload->client_id_length[1] = clientlength & 0xFF;

    printf("Este es el client ID: %s\n", connect_payload->clientID);
    printf("Este es el client id en binario");
    printbinario(connect_payload->clientID);

    printf("Este es el client id length en binario");
    printbinario2(connect_payload->client_id_length);

    int userlength = strlen(connect_payload->username);
    connect_payload->username_length[0] = (userlength >> 8) & 0xFF; // Byte más significativo
    connect_payload->username_length[1] = userlength & 0xFF;

    printf("Este es el username: %s\n", connect_payload->username);
    printf("Este es el username length en binario");
    printbinario2(connect_payload->username_length);

    int passlength = strlen(connect_payload->password);
    connect_payload->password_length[0] = (passlength >> 8) & 0xFF; // Byte más significativo
    connect_payload->password_length[1] = passlength & 0xFF;

    printf("Este es el password: %s\n", connect_payload->password);
    printf("Este es el passlength en binario");
    printbinario2(connect_payload->password_length);

    // Codificar la longitud restante (byte 2 del buffer que puede extenderse hasta el byte 4)
    int length_pl = (2 + strlen(connect_payload->clientID) + 2+ strlen(connect_payload->username) + 2 + strlen(connect_payload->password));
    int length_vh = (6 + 1 + 1 + 2);
    int length = length_pl + length_vh;
    printf("Length: %i\n", length);
    
    printf("client id strlen: %zu\n", strlen(connect_payload->clientID));
 
    printf("username strlen: %zu\n", strlen(connect_payload->username));

    printf("password strlen: %zu\n", strlen(connect_payload->password));
 
    char *result;
    result = malloc(sizeof(char)*4);
    encode_remaining_length(length, result);

    //construir paquete
    *connect_length= 1 + strlen(result) + length;
    connect_message = malloc(sizeof(char)**connect_length);
    int offset = 0;
    connect_message[offset++] = fixed_header->control_packet_type;

    for (int i = 0; i < strlen(result); i++) {
        connect_message[offset++] = result[i];
    }

    for (int i = 0; i < 2; i++) {
        connect_message[offset++] = variable_header->protocol_name_length[i];
    }

    for (int i = 0; i < 4; i++) {
        connect_message[offset++] = variable_header->protocol_name[i];
    }

    connect_message[offset++] = variable_header->protocol_version;
    connect_message[offset++] = variable_header->connect_flags;

    for (int i = 0; i < 2; i++) {
        connect_message[offset++] = variable_header->keep_alive[i];
    }

    for (int i = 0; i < 2; i++) {
        connect_message[offset++] = connect_payload->client_id_length[i];
    }

    for (int i = 0; i < clientlength; i++) {
        connect_message[offset++] = connect_payload->clientID[i];
    }

    for (int i = 0; i < 2; i++) {
        connect_message[offset++] = connect_payload->username_length[i];
    }

    for (int i = 0; i < userlength; i++) {
        connect_message[offset++] = connect_payload->username[i];
    }

    for (int i = 0; i < 2; i++) {
        connect_message[offset++] = connect_payload->password_length[i];
    }

    for (int i = 0; i < passlength; i++) {
        connect_message[offset++] = connect_payload->password[i];
    }

    printf("\n");
    printf("Este es el mensaje connect: \n");
    printbuffer(connect_message,*connect_length);
    free(result);
    free(connect_payload->clientID);
    free(connect_payload->username);
    free(connect_payload->password);
    return connect_message;
    
}

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

void encode_remaining_length(int length, char *result) {
    char encoded_byte;
    int i = 0;

    do {
        encoded_byte = length % 128;
        length /= 128;

        if (length > 0) {
            encoded_byte |= 128;
        }
        result[i] = encoded_byte;
        i++;
    } while (length > 0);
}

void printbinario(char arr[]) {
    printf("\n");
    for (int i = 0; arr[i] != '\0'; ++i) {
        char c = arr[i];
        for (int j = 7; j >= 0; --j) {
            putchar((c & (1 << j)) ? '1' : '0');
        }
        putchar(' ');
    }
    putchar('\n');
}

void printbinario2(char arr[]) {
    printf("\n");
    for (int i = 0; i<2; ++i) {
        char c = arr[i];
        for (int j = 7; j >= 0; --j) {
            putchar((c & (1 << j)) ? '1' : '0');
        }
        putchar(' ');
    }
    putchar('\n');
}