INTRODUCCION

En el presente trabajo se busca la implementacion del protocolo MQTT 3.1.1 en lenguaje C, un protocolo que opera mediante un modelo de publicacion/suscripcion, permitiendo a los clientes publicar mensajes en temas especificos y suscribirse a temas de interes. Para la realizacion del proyecto se usa la API SOCKETS DE BERKELEY con el fin de establecer una conexion entra el cliente y el servidor (broker).
Con este enfoque, se facilita una comunicación eficiente y escalable entre dispositivos conectados, siendo ideal para aplicaciones IoT donde la eficiencia y la confiabilidad son fundamentales.

DESARROLLO

Fundamentalmente se establecen los archivos cliente y servidor para establecer la conexion haciendo uso de la APi de Sockets de Berkeley, ademas se inicializan las estructuras y variables necesarias para el llamado de las funciones que permitiran el intercambio de mensajes entre el cliente y el broker. Con fines de una estructura detallada, entendible y escalable, se crea un archivo para cada tipo de mensaje con su respectivo archivo encabezado. 
Respecto a la experiencia implementando el protocolo, se presentaron diversas dificultades con el entendimiento de la documentacion del protocolo y de su respectiva implementacion en el lenguaje C, la complejidad inherente al protocolo y las particularidades de su implementación han requerido un análisis detallado y un enfoque metódico para garantizar un desarrollo correcto y funcional del sistema de comunicación MQTT; gracias a esto se obtuvo un gran aprendizaje relacionado a la programacion en red, trabajo en equipo y sobre todo el manejo del lenguaje C. 

ASPECTOS LOGRADOS

De todos los mensajes con los que cuenta el protocolo MQTT, se logro la implementacion del CONNECT, CONNACK, PUBLISH, PUBACK Y DISCONNECT. De manera mas detallada se logro una conexion exitosa entre los sockets cliente y servidor, pudiendo 
intercambiar mensajes CONNECT y CONNACK, para posteriormente poder darle la opcion al cliente de hacer un PUBLISH o un DISCONNECT. En el caso de que un cliente envie un PUBLISH, este se envia exitosamente al broker, el cual lo recibe; sin embargo la devolucion del archivo PUBACK por parte del servidor no se logra de manera exitosa. Por otro lado, en el caso del archivo del DISCONNECT, este cumple su funcion de manera correcta cerrando la conexion entre el cliente y el servidor. 

ASPECTOS NO LOGRADOS

No se cumple con la implementacion de los mensajes SUSCRIBE y SUBACK, por cuestiones de mala gestion del timpo, dificultades con el aprendizaje del protocolo y manejo del lenguaje, ademas de multiples fallas tecnicas con los equipos de computo en los cuales se llevo a cabo la implementacion. Adicional a esto, el desarrollo de la concurrencia no fue posible. 

CONCLUSIONES 

Finalmente, a pesar de que no se pudo desarrollar el 100% del protocolo como se requeria, hubo un trabajo arduo por parte de todos los integrantes del equipo, dejando grandes aprendizajes y ensenanzas como programacion en C, programacion en red, implementacion y diseno de algoritmos, lectura y entendimiento de documentacion compleja, el funcionamiento de protocolos para las telecomunicaciones y la mensajeria. Ademas de todos los aprendizajes tecnicos que dejo este proyecto, se fortalecieron las habilidades blandas y competencias transversales gracias a el trabajo en equipo y el entendimiento de lo que es una una buena planeacion y gestion del timpo a la hora de abarcar un proyecto.

REFERENCIAS
https://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718037
