// Estructura para el encabezado fijo (Fixed Header) del paquete CONNECT
struct publish_fixed_header {
    char control_packet_type;  // Tipo de paquete de control (CONNECT)
    char *remaining_length;     // Longitud restante del paquete
};

// Estructura para el encabezado variable (Variable Header) del paquete CONNECT
struct publish_variable_header {
    char topic_length[2];
    char *topic;            // Nombre del topic
    char packet_id[2];  // paquete identifier
}; 

// Estructura para el payload del paquete CONNECT
struct publish_payload {
    char message_length[2];     // Longitud del ID del cliente
    char *message;            // Identificador de cliente (Client ID)
};

char *build_publish_packet(char *publish_message, size_t *publish_length, struct publish_fixed_header *fixed_header, struct publish_variable_header *variable_header, struct publish_payload *payload);
