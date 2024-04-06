#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "54.89.62.171"
#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

int main() {
    int cliente_socket;
    struct sockaddr_in servidor_addr;

    // Crear socket TCP/IP
    if ((cliente_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
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

    // Envía mensajes al servidor en tiempo real
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Escribe un mensaje para enviar al servidor (o 'salir' para terminar): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Verificar si el mensaje es igual a 'salir'
        if (strcmp(buffer, "salir\n") == 0) {
            send(cliente_socket, buffer, strlen(buffer), 0);
            printf("Saliendo del cliente...\n");
            break; // Salir del bucle
        }

        // Enviar el mensaje al servidor
        send(cliente_socket, buffer, strlen(buffer), 0);
    }

    // Cerrar el socket cuando hayas terminado
    close(cliente_socket);

    return 0;
}
