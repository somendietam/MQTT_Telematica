#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "connect.h"
#include "disconnect.h"
#include "publish.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 1024


int main() {
    //inicialización estructuras connect
    struct connect_fixed_header connect_fixed_header;
    struct connect_variable_header connect_variable_header;
    struct connect_payload connect_payload;

    //inicialización estructuras publish
    struct publish_fixed_header publish_fixed_header;
    struct publish_variable_header publish_variable_header;
    struct publish_payload publish_payload;

    // Crear socket TCP/IP
    char buffer[BUFFER_SIZE];

    //creación variables connect
    char *connect_message = NULL;
    char *disconnect_message = NULL;
    size_t connect_length = 0;

    //creación variables publish
    char *publish_message = NULL;
    size_t publish_length = 0;

    // Crear socket TCP/IP
    int cliente_socket;

    if ((cliente_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
    struct sockaddr_in servidor_addr;
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &servidor_addr.sin_addr) <= 0) {
        perror("Dirección del servidor inválida");
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(cliente_socket, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1) {
        perror("Error al conectar con el servidor");
        exit(EXIT_FAILURE);
    }

    printf("Conectado al servidor MQTT\n");

    connect_message = build_connect_packet(connect_message, &connect_length, &connect_fixed_header, &connect_variable_header, &connect_payload);

    // Enviar el paquete CONNECT al servidor
    if (send(cliente_socket, connect_message, connect_length, 0) == -1) {
        perror("Error al enviar el paquete CONNECT al servidor");
        exit(EXIT_FAILURE);
    }
    printf("Paquete CONNECT enviado al servidor.\n");    
    
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytes_recibidos = recv(cliente_socket, buffer, BUFFER_SIZE, 0);

    if (bytes_recibidos <= 0) {
        printf("Error al recibir mensaje del servidor\n");
    }

    // Mostrar mensaje recibido
    printf("\n");
    printf("Paquete CONNACK recibido del servidor.\n"); 
    printbuffer(buffer, 4);

    int option = 0;

    while(1){
        printf("\nIngrese el numero del mensaje: \n");
        scanf("%i", &option);

        if(option == 14){
            disconnect_message = build_disconnect(disconnect_message);

            printf("\nMENSAJE DISCONNECT: \n");
            printbuffer(disconnect_message, 2);

            // Enviar el paquete DISCONNECT al servidor
            if (send(cliente_socket, disconnect_message, 2, 0) == -1) {
                perror("Error al enviar el paquete DISCONNECT al servidor");
                exit(EXIT_FAILURE);
            }
            printf("Paquete DISCONNECT enviado al servidor.\n");
            close(cliente_socket);
            break;
        }

        if(option == 3){
            publish_message = build_publish_packet(publish_message, &publish_length, &publish_fixed_header, &publish_variable_header, &publish_payload);
            if (send(cliente_socket, publish_message, publish_length, 0) == -1) {
                perror("Error al enviar el paquete PUBLISH al servidor");
                exit(EXIT_FAILURE);
            }
            printf("Paquete PUBLISH enviado al servidor.\n");

        }
    }
    return 0;
    free(connect_message);
    free(disconnect_message);
    free(publish_message);
}