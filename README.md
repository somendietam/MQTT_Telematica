INTRODUCCION

En el presente trabajo se busca la implementacion del protocolo MQTT 3.1.1 en lenguaje C, un protocolo que opera mediante un modelo de publicacion/suscripcion, permitiendo a los clientes publicar mensajes en temas especificos y suscribirse a temas de interes. Para la realizacion del proyecto se usa la API SOCKETS DE BERKELEY con el fin de establecer una conexion entra el cliente y el servidor (broker).
Con este enfoque, se facilita una comunicación eficiente y escalable entre dispositivos conectados, siendo ideal para aplicaciones IoT donde la eficiencia y la confiabilidad son fundamentales.

DESARROLLO

Fundamentalmente se establecen los archivos cliente y servidor para establecer la conexion haciendo uso de la APi de Sockets de Berkeley, ademas se inicializan las estructuras y variables necesarias para el llamado de las funciones que permitiran el intercambio de mensajes entre el cliente y el broker. Con fines de una estructura detallada, entendible y escalable, se crea un archivo para cada tipo de mensaje con su respectivo archivo encabezado. 
Respecto a la experiencia implementando el protocolo, se presentaron diversas dificultades con el entendimiento de la documentacion del protocolo y de su respectiva implementacion en el lenguaje C, la complejidad inherente al protocolo y las particularidades de su implementación han requerido un análisis detallado y un enfoque metódico para garantizar un desarrollo correcto y funcional del sistema de comunicación MQTT; gracias a esto se obtuvo un gran aprendizaje relacionado a la programacion en red, trabajo en equipo y sobre todo el manejo del lenguaje C. 

ASPECTOS LOGRADOS
De todos los mensajes con los que cuenta el protocolo MQTT, se logro la implementacion del CONNECT, CONNACK, PUBLISH, PUBACK Y DISCONNECT. De manera mas detallada se logro una conexion exitosa entre los sockets cliente y servidor, pudiendo 
intercambiar
