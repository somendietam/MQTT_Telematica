#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

// Estructura para el encabezado fijo (Fixed Header) del paquete CONNECT
struct fixed_header {
    char control_packet_type;  // Tipo de paquete de control (CONNECT)
    char *remaining_length;     // Longitud restante del paquete
} connect_fixed_header;

// Estructura para el encabezado variable (Variable Header) del paquete CONNECT
struct connect_variable_header {
    char protocol_name_length[2];
    char protocol_name[4];  // Nombre del protocolo (MQTT)
    char protocol_version;  // Versión del protocolo (3.1.1)
    char connect_flags;     // Bandera de conexión limpia, etc.
    char keep_alive[2];       // Tiempo de vida de la sesión (Keep Alive)
} connect_variable_header;

// Estructura para el payload del paquete CONNECT
struct connect_payload {
    char client_id_length[2];     // Longitud del ID del cliente
    char *clientID;            // Identificador de cliente (Client ID)
    char username_length[2];      // Longitud del nombre de usuario
    char *username;             // Nombre de usuario (Username)
    char password_length[2];      // Longitud de la contraseña
    char *password;             // Contraseña (Password)
} connect_payload;

// Prototipos de funciones
void encode_remaining_length(int length, char result[]);
char *build_connect_packet(char *connect_message, size_t *connect_length, struct fixed_header *fixed_header, struct connect_variable_header *variable_header, struct connect_payload *connect_payload);
void printbinario(char arr[]);
void printbuffer(char arr[], size_t size);
void printbinario2(char arr[]);
char *build_disconnect(char *disconnect_message);


int main() {
    // Crear socket TCP/IP
    char buffer[BUFFER_SIZE];
    char *connect_message = NULL;
    char *disconnect_message = NULL;
    disconnect_message = malloc(sizeof(char)*2);
    size_t connect_length = 0;
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
    }
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


// Función para construir el paquete CONNECT
char *build_connect_packet(char *connect_message, size_t *connect_length, struct fixed_header *fixed_header, struct connect_variable_header *variable_header, struct connect_payload *connect_payload) {

    printf("Ingrese Client ID: ");
    getline(&connect_payload->clientID, connect_length, stdin);
    connect_payload->clientID[strcspn(connect_payload->clientID, "\n")] = '\0'; // Eliminar el carácter de nueva línea

    printf("Ingrese nombre de usuario: ");
    getline(&connect_payload->username, connect_length, stdin);
    connect_payload->username[strcspn(connect_payload->username, "\n")] = '\0'; // Eliminar el carácter de nueva línea

    printf("Ingrese contraseña: ");
    getline(&connect_payload->password, connect_length, stdin);
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

    printf("Este es el connect flags: %d\n", connect_variable_header.connect_flags);

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
    return connect_message;
    
}

char *build_disconnect(char * disconnect_message){

    disconnect_message[0] = 0xE0;
    disconnect_message[1] = 0x00;

    return disconnect_message;
}
