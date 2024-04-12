struct fixed_header{
    char control_packet_type;
    char remaining_length;
};

struct connack_variable_header{
    char flags;
    char return_code;
};

//Definir funciones
void printbuffer(char arr[], size_t size);
char *build_connack(char *connack_message, struct fixed_header *fixed_header, struct connack_variable_header *variable_header, int flag_clean_ssn);
int search_flag_connect(char buffer[]);