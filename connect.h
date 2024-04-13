
// Estructura para el encabezado fijo (Fixed Header) del paquete CONNECT
struct connect_fixed_header {
    char control_packet_type;  // Tipo de paquete de control (CONNECT)
    char *remaining_length;     // Longitud restante del paquete
};

// Estructura para el encabezado variable (Variable Header) del paquete CONNECT
struct connect_variable_header {
    char protocol_name_length[2];
    char protocol_name[4];  // Nombre del protocolo (MQTT)
    char protocol_version;  // Versión del protocolo (3.1.1)
    char connect_flags;     // Bandera de conexión limpia, etc.
    char keep_alive[2];       // Tiempo de vida de la sesión (Keep Alive)
}; 

// Estructura para el payload del paquete CONNECT
struct connect_payload {
    char client_id_length[2];     // Longitud del ID del cliente
    char *clientID;            // Identificador de cliente (Client ID)
    char username_length[2];      // Longitud del nombre de usuario
    char *username;             // Nombre de usuario (Username)
    char password_length[2];      // Longitud de la contraseña
    char *password;             // Contraseña (Password)
};

void encode_remaining_length(int length, char result[]);
void printbinario(char arr[]);
void printbuffer(char arr[], size_t size);
void printbinario2(char arr[]);
char *build_connect_packet(char *connect_message, size_t *connect_length, struct connect_fixed_header *fixed_header, struct connect_variable_header *variable_header, struct connect_payload *connect_payload);
