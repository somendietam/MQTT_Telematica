#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

struct connack_fixed_header{
    char control_packet_type;
    char remaining_length;
}connack_fixed_header;

struct connack_variable_header{
    char flags;
    char return_code;
}connack_variable_header;

//Definir funciones
void printbuffer(char arr[], size_t size);
char *build_connack(char *connack_message, struct connack_fixed_header *fixed_header, struct connack_variable_header *variable_header, int flag_clean_ssn);
int search_flag_connect(char buffer[]);


int main() {
    int servidor_socket, cliente_socket;
    struct sockaddr_in servidor_addr, cliente_addr;
    socklen_t cliente_len;
    char *connack_message = NULL;
    connack_message = malloc(sizeof(char)*4);
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
        if (bytes_recibidos <= 0) {
            perror("Error al recibir mensaje del cliente");
            break;
        }

        if(buffer[0] == -32){
            printbuffer(buffer,2);
            printf("\nHa llegado un DISCONNECT, cerrando conexión...\n");
            break;
        }

}
    printf("\nFIN\n");

    return 0;
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


char *build_connack(char *connack_message, struct connack_fixed_header *fixed_header, struct connack_variable_header *variable_header, int flag_clean_ssn){

    connack_fixed_header.control_packet_type = 0x20;
    connack_fixed_header.remaining_length = 0x02;
    
    if (flag_clean_ssn == 1){
        connack_variable_header.flags = 0x00;
    }else{
        connack_variable_header.flags = 0x01;
    }

    connack_variable_header.return_code = 0x00;

    connack_message[0] = connack_fixed_header.control_packet_type;
    connack_message[1] = connack_fixed_header.remaining_length;
    connack_message[2] = connack_variable_header.flags;
    connack_message[3] = connack_variable_header.return_code;

    printf("\n");
    printf("Este es el CONNACK: \n");
    printbuffer(connack_message, 4);

    return connack_message;
    
}
