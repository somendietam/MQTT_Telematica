## Introducción
En el presente trabajo se busca la implementación del protocolo MQTT 3.1.1 en lenguaje C, un protocolo que opera mediante un modelo de publicación/suscripción, permitiendo a los clientes publicar mensajes en temas específicos y suscribirse a temas de interés. Para la realización del proyecto se usa la API Sockets de Berkeley con el fin de establecer una conexión entre el cliente y el servidor (broker). Con este enfoque, se facilita una comunicación eficiente y escalable entre dispositivos conectados, siendo ideal para aplicaciones IoT donde la eficiencia y la confiabilidad son fundamentales.

## Desarrollo
Fundamentalmente se establecen los archivos cliente y servidor para establecer la conexión haciendo uso de la API de Sockets de Berkeley. Además, se inicializan las estructuras y variables necesarias para el llamado de las funciones que permitirán el intercambio de mensajes entre el cliente y el broker. Con fines de una estructura detallada, entendible y escalable, se crea un archivo para cada tipo de mensaje con su respectivo archivo encabezado. Respecto a la experiencia implementando el protocolo, se presentaron diversas dificultades con el entendimiento de la documentación del protocolo y de su respectiva implementación en el lenguaje C. La complejidad inherente al protocolo y las particularidades de su implementación han requerido un análisis detallado y un enfoque metódico para garantizar un desarrollo correcto y funcional del sistema de comunicación MQTT; gracias a esto se obtuvo un gran aprendizaje relacionado a la programación en red, trabajo en equipo y sobre todo el manejo del lenguaje C.

###Diagrama de flujo 
![Diagrama de flujo](https://github.com/somendietam/tele/blob/main/DiagramaFlujoMQTT.png)

## Aspectos Logrados
De todos los mensajes con los que cuenta el protocolo MQTT, se logró la implementación del CONNECT, CONNACK, PUBLISH, PUBACK y DISCONNECT. De manera más detallada se logró una conexión exitosa entre los sockets cliente y servidor, pudiendo intercambiar mensajes CONNECT y CONNACK, para posteriormente poder darle la opción al cliente de hacer un PUBLISH o un DISCONNECT. En el caso de que un cliente envíe un PUBLISH, este se envía exitosamente al broker, el cual lo recibe; sin embargo, la devolución del archivo PUBACK por parte del servidor no se logra de manera exitosa. Por otro lado, en el caso del archivo del DISCONNECT, este cumple su función de manera correcta cerrando la conexión entre el cliente y el servidor.

## Aspectos No Logrados
No se cumplió con la implementación de los mensajes SUBSCRIBE y SUBACK, por cuestiones de mala gestión del tiempo, dificultades con el aprendizaje del protocolo y manejo del lenguaje, además de múltiples fallas técnicas con los equipos de cómputo en los cuales se llevó a cabo la implementación. Adicional a esto, el desarrollo de la concurrencia no fue posible.

## Conclusiones
Finalmente, a pesar de que no se pudo desarrollar el 100% del protocolo como se requería, hubo un trabajo arduo por parte de todos los integrantes del equipo, dejando grandes aprendizajes y enseñanzas como programación en C, programación en red, implementación y diseño de algoritmos, lectura y entendimiento de documentación compleja, el funcionamiento de protocolos para las telecomunicaciones y la mensajería. Además de todos los aprendizajes técnicos que dejó este proyecto, se fortalecieron las habilidades blandas y competencias transversales gracias al trabajo en equipo y el entendimiento de lo que es una buena planeación y gestión del tiempo a la hora de abarcar un proyecto.

## Referencias
[Protocolo MQTT 3.1.1](https://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718037)

