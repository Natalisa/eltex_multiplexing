#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netdb.h>

#define PORT 3490
#define SERVER "127.0.0.1"

int main(){
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_addr(SERVER);
  int sock;
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("Ошибка create()");
    return -1;
  }
  if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
    perror("Ошибка connect()");
    return -1;
  }
  send(sock, "Hello", 6, 0);
  char buf[255];
  if(recv(sock, buf, sizeof(buf), 0) < 0){
    perror("Ошибка recv()");
    return -1;
  }
  printf("Сервер: %s\n",buf);
  close(sock);
  return 0;
}
