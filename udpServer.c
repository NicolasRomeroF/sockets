//SERVER UDP

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
 
#define BUFLEN 512  //Tamaño maximo del buffer
 
int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Se debe espeficifar el puerto como argumento\n");
		return 1;
	}
  struct sockaddr_in server, cliente;   
  int conexionServer, i, slen = sizeof(cliente) , lenRecibido;
  char buf[BUFLEN];   
  //Se crea el socketUDP
  if ((conexionServer=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
  {
    printf("Error al crear socket\n");
    return 1;
  }   
  //Se llena la estructura con 0s
  bzero((char *)&server, sizeof(server)); 
   
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[1]));
  server.sin_addr.s_addr = htonl(INADDR_ANY);   
  //Bindear socket al puerto
  if( bind(conexionServer , (struct sockaddr*)&server, sizeof(server) ) == -1)
  {
    printf("Error al asociar el puerto a la conexion\n");
    close(conexionServer);
    return 1;
  }   
  //Se escucha en el puerto
  printf("Escuchando puerto %d\n", ntohs(server.sin_port));
  while(1)
  {
      printf("Esperando datos...\n");
      fflush(stdout);      
      //Se intenta recibir los datos
      if ((lenRecibido = recvfrom(conexionServer, buf, BUFLEN, 0, (struct sockaddr *) &cliente, &slen)) == -1)
      {
        printf("Error al recibir los datos\n");
		    close(conexionServer);
		    return 1;
      }   
      printf("Se reciben datos desde %s:%d\n", inet_ntoa(cliente.sin_addr), ntohs(cliente.sin_port));
      printf("Datos: %s\n" , buf);      
      //Se responde al cliente con un recibido
      if (sendto(conexionServer, "Recibido", lenRecibido, 0, (struct sockaddr*) &cliente, slen) == -1)
      {
        printf("Error al enviar datos\n");
		    close(conexionServer);
		    return 1;
      }
  }
  close(conexionServer);
  return 0;
}
