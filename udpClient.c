//CLIENTE TCP

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

#define SERVER "127.0.0.1"

int main(int argc, char **argv){

	if(argc < 2)
	{
		printf("Se debe espeficifar el puerto como argumento\n");
		return 1;
	}


	struct sockaddr_in cliente; 
	struct hostent *server; 
	server = gethostbyname(SERVER); 

	int puerto, conexion;
	char buffer[100];
    //Se crea el socket UDP
	conexion = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
	puerto=atoi(argv[1]); //conversion del argumento
    //Se pone 0's en la estructura y se asignan los protocolos y puertos
	bzero((char *)&cliente, sizeof((char *)&cliente)); 
	cliente.sin_family = AF_INET; //asignacion del protocolo
	cliente.sin_port = htons(puerto); //asignacion del puerto

    //Se copian los datos del cliente en el server
	bcopy((char *)server->h_addr, (char *)&cliente.sin_addr.s_addr, sizeof(server->h_length));



	//se conecta con el host
	if(connect(conexion,(struct sockaddr *)&cliente, sizeof(cliente)) < 0)
	{ //conectando con el host
		printf("Error conectando con el host\n");
		close(conexion);
		return 1;
	}
	printf("Conectado con %s:%d\n",inet_ntoa(cliente.sin_addr),htons(cliente.sin_port));
	
	while(1){
		printf("Escribe un mensaje: ");
		fgets(buffer, 100, stdin);
    	send(conexion, buffer, 100, 0); //envio
    	bzero(buffer, 100);
    	recv(conexion, buffer, 100, 0); //recepción
    	printf("%s", buffer);
    }

return 0;
}