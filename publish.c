#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <time.h>
#include "publish.h"
#include "connect.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 1024

char *build_publish_packet(char *publish_message, size_t *publish_length, struct publish_fixed_header *fixed_header, struct publish_variable_header *variable_header, struct publish_payload *payload){
    

    fixed_header->control_packet_type = 0b00110000;
    int dup_client;
    int qos_value_client;

    // Pregunta si es el primer intento de envío
    printf("¿Es el primer intento de envío? (0/1): ");
    scanf("%d", &dup_client);

    if (dup_client == 1){
        fixed_header->control_packet_type = fixed_header->control_packet_type | 0b00001000;
    } //asignamos el dup = 1 en el fixed header, de lo contrario se quedara en 0 por defecto


    // Pregunta por el valor de QoS
    printf("Ingrese el valor de QoS (0/1/2): ");
    scanf("%d", &qos_value_client);

    //si el valor es 0 lo dejamos asi por defecto (no operamos nada), de lo contrario:

    if(qos_value_client == 1){
        fixed_header->control_packet_type = fixed_header->control_packet_type | 0b00000010;
    }else if (qos_value_client == 2){
        fixed_header->control_packet_type = fixed_header->control_packet_type | 0b00000100;
    }

    //bit de retain
    int retain = 0;
    
    if (qos_value_client != 0){
        fixed_header->control_packet_type = fixed_header->control_packet_type | 0b00000001;
        
    }

    //TOPIC Y PAYLOAD
    printf("Por favor, introduce el topic: ");
    getline(&variable_header->topic, publish_length, stdin);
    while(getchar() != '\n');
    variable_header->topic[strcspn(variable_header->topic, "\n")] = '\0';
    

    printf("Por favor, introduce el texto: ");
    getline(&payload->message, publish_length, stdin);
    while(getchar() != '\n');
    payload->message[strcspn(payload->message, "\n")] = '\0';

    int topiclength = strlen(variable_header->topic);
    variable_header->topic_length[0] = (topiclength >> 8) & 0xFF;
    variable_header->topic_length[1] = topiclength & 0xFF;

    printf("\nEste es el TOPIC: ");
    printbuffer(variable_header->topic, topiclength);

    int messagelength = strlen(payload->message);
    payload->message_length[0] = (messagelength >> 8) & 0xFF;
    payload->message_length[1] = messagelength & 0xFF;

    printf("\nEste es el MESSAGE: ");
    printbuffer(payload->message, messagelength);

    srand(time(NULL));    
    int numero_aleatorio = rand() % 65536; //Generar un número aleatorio entre 0 y 65535 (2^16 - 1)
    variable_header->packet_id[0] = (numero_aleatorio >> 8) & 0xFF;   // Almacenar el número aleatorio en un array de dos bytes
    variable_header->packet_id[1] = numero_aleatorio & 0xFF;  


    //REMAINING LENGTH

    int length_vh = 2 + strlen(variable_header->topic) + 2;
    printf("\nESte s el LENGTH_VH: %i", length_vh);
    int length_pl = 2 + strlen(payload->message);
    printf("\nESte s el LENGTH_PL: %i", length_pl);
    int length = length_vh + length_pl;
    printf("\nESte s el LENGTH: %i", length);

    char *remaining_length = NULL;
    remaining_length = malloc(sizeof(char)*4);
    encode_remaining_length(length, remaining_length);

    //CONSTRUCCIÓN PAQUETE PUBLISH

    *publish_length = 1 + strlen(remaining_length) + length;
    publish_message = malloc(sizeof(char)**publish_length);

    int offset = 0;

    //Inclusión control packet type
    publish_message[offset++] = fixed_header->control_packet_type;

    //Inclusión remaining length
    for(int i=0; i<strlen(remaining_length); i++){
        publish_message[offset++] = remaining_length[i];
    }

    //Inclusión variable header
    for(int i=0; i<2; i++){
        publish_message[offset++] = variable_header->topic_length[i];
    }

    for(int i=0; i<topiclength; i++){
        publish_message[offset++] = variable_header->topic[i];
    }

    for(int i=0; i<2; i++){
        publish_message[offset++] = variable_header->packet_id[i];
    }

    //Inclusión payload
    for(int i=0; i<2; i++){
        publish_message[offset++] = payload->message_length[i];
    }

    for(int i=0; i<messagelength; i++){
        publish_message[offset++] = payload->message[i];
    }

    printf("\n");
    printf("Este es el mensaje PUBLISH: \n");
    printbuffer(publish_message, *publish_length);

    free(remaining_length);

    return publish_message;
}
