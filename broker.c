#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "connack.h"

#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

int main() {
    int servidor_socket, cliente_socket;
    struct sockaddr_in servidor_addr, cliente_addr;
    struct fixed_header connack_fixed_header;
    struct connack_variable_header connack_variable_header;
    socklen_t cliente_len;
    char *connack_message = NULL;
    char buffer[BUFFER_SIZE];

    // Crear socket TCP/IP
    if ((servidor_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_addr.s_addr = INADDR_ANY;
    servidor_addr.sin_port = htons(SERVER_PORT);

    // Enlazar el socket a la dirección y puerto del servidor
    if (bind(servidor_socket, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Error al enlazar el socket");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(servidor_socket, 10) == -1) {
        perror("Error al escuchar conexiones entrantes");
        exit(EXIT_FAILURE);
    }

    printf("Servidor MQTT esperando conexiones entrantes...\n");

    // Aceptar conexión entrante
    cliente_len = sizeof(cliente_addr);
    cliente_socket = accept(servidor_socket, (struct sockaddr *)&cliente_addr, &cliente_len);
    if (cliente_socket == -1) {
        perror("Error al aceptar conexión entrante");
        exit(EXIT_FAILURE);
    }

    printf("Conexión establecida con el cliente\n");

    // Bucle para recibir mensajes del cliente
    memset(buffer, 0, BUFFER_SIZE); // Limpiar el buffer

    // Recibir mensaje del cliente
    ssize_t bytes_recibidos = recv(cliente_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_recibidos <= 0) {
        perror("Error al recibir mensaje del cliente");
    }

    // Mostrar mensaje recibido
    printbuffer(buffer, strlen(buffer));
    printf("\n");

    int flag = search_flag_connect(buffer);

    connack_message = build_connack(connack_message, &connack_fixed_header, &connack_variable_header, flag);

    if (send(cliente_socket, connack_message, 4, 0) == -1) {
            perror("Error al enviar el paquete CONNACK al servidor");
            exit(EXIT_FAILURE);
        }
    printf("Paquete CONNACK enviado al cliente.\n");       

    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // Limpiar el buffer

        // Recibir mensaje del cliente
        ssize_t bytes_recibidos = recv(cliente_socket, buffer, BUFFER_SIZE, 0);
        unsigned char input = (buffer[0] >> 4) & 0x0F;
        if (bytes_recibidos < 0) {
            perror("Error al recibir mensaje del cliente");
            break;
        }

        if(input == 14){
            printbuffer(buffer,2);
            printf("\nHa llegado un DISCONNECT, cerrando conexión...\n");
            
        }

        if(input == 3){
            printbuffer(buffer,BUFFER_SIZE);
            printf("\nHa llegado un PUBLISH!");
        }

}
    printf("\nFIN\n");
    free(connack_message);
    return 0;
}
