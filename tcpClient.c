//CLIENTE UDP

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

#define SERVIDOR "127.0.0.1"

int main(int argc, char* argv[]){

  if(argc < 2)
  {
    printf("Se debe espeficifar el puerto como argumento\n");
    return 1;
  }
  //Se declaran las estructuras
  struct sockaddr_in cliente; 
  struct hostent *servidor; 
  //Se asigna el servidor
  servidor = gethostbyname(SERVIDOR); 

  if(servidor == NULL)
  { //Comprobación
    printf("Host erróneo\n");
    return 1;
  }

  int puerto, conexion;
  char buffer[100];
  //Se asigna el socket TCP
  conexion = socket(AF_INET, SOCK_STREAM, 0);
  puerto=atoi(argv[1]); 

  //Se llena la estructura con 0s
  bzero((char *)&cliente, sizeof((char *)&cliente));

  //Se asignan protocolos y puerto
  cliente.sin_family = AF_INET; 
  cliente.sin_port = htons(puerto); 
  //Se copia info del servidor al cliente
  bcopy((char *)servidor->h_addr, (char *)&cliente.sin_addr.s_addr, sizeof(servidor->h_length));



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