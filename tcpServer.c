//SERVER TCP

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>

#define BUFLEN 512 

int main(int argc, char* argv[]){
  if(argc < 2)
  {
    printf("Se debe espeficifar el puerto como argumento\n");
    return 1;
  }
  int conexionServer, conexionClient, puerto; 
  socklen_t longc; 
  struct sockaddr_in server, cliente;
  char buffer[BUFLEN]; 
  puerto = atoi(argv[1]); 
  //Se crea socket TCP
  conexionServer = socket(AF_INET, SOCK_STREAM, 0); 
  //se llena la estructura con 0s
  bzero((char *)&server, sizeof(server)); 
  server.sin_family = AF_INET; //adress family IPv4
  server.sin_port = htons(puerto);
  server.sin_addr.s_addr = INADDR_ANY; //macro que asigna la direccion propia
  
  //Bindear socket al puerto
  if(bind(conexionServer, (struct sockaddr *)&server, sizeof(server)) < 0)
  { 
    printf("Error al asociar el puerto a la conexion\n");
    close(conexionServer);
    return 1;
  }
  //Se comienza a escuchar en el puerto
  listen(conexionServer, 3); 
  printf("Escuchando puerto %d\n", ntohs(server.sin_port));
  longc = sizeof(cliente);
  //Se espera la conexion
  conexionClient = accept(conexionServer, (struct sockaddr *)&cliente, &longc); 
  if(conexionClient<0)
  {
    printf("Error al aceptar trafico\n");
    close(conexionServer);
    return 1;
  }
  printf("Conectando con %s:%d\n", inet_ntoa(cliente.sin_addr),htons(cliente.sin_port)); 
  while(1){
    printf("Esperando datos...\n");
    fflush(stdout);
    
    if(recv(conexionClient, buffer, 100, 0) < 0)
    { //Se reciben los datos del cliente
      printf("Error al recibir los datos\n");
      close(conexionServer);
      return 1;
    }
    else
    {
      printf("Se reciben datos desde %s:%d\n", inet_ntoa(cliente.sin_addr), ntohs(cliente.sin_port));
      printf("Datos: %s\n", buffer);
      bzero((char *)&buffer, sizeof(buffer));
      send(conexionClient, "Recibido\n", 13, 0);
    }
  }
  close(conexionServer);
  return 0;
}