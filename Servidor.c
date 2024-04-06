#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

int main() {
    int servidor_socket, cliente_socket;
    struct sockaddr_in servidor_addr, cliente_addr;
    socklen_t cliente_len;
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
    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // Limpiar el buffer

        // Recibir mensaje del cliente
        ssize_t bytes_recibidos = recv(cliente_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_recibidos <= 0) {
            perror("Error al recibir mensaje del cliente");
            break;
        }

        // Mostrar mensaje recibido
        printf("Mensaje recibido del cliente: %s\n", buffer);

        // Verificar si el cliente quiere salir
        if (strcmp(buffer, "salir\n") == 0) {
            printf("El cliente ha solicitado salir. Cerrando conexión...\n");
            break; // Salir del bucle
        }
    }

    // Cerrar socket del cliente y del servidor cuando termines
    close(cliente_socket);
    close(servidor_socket);

    return 0;
}
