#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>

#define PORT 3490

int main(){
  /* TCP */
  struct sockaddr_in server, client;
  int sockTCP;
  if((sockTCP = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("Ошибка create()");
    return -1;
  }
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);
  if(bind(sockTCP, (struct sockaddr*)&server, sizeof(server)) == -1){
    perror("Ошибка bind()");
    return -1;
  }
  if(listen(sockTCP,5)==-1){
    perror("Ошибка listen()");
    return -1;
  }

  /* UDP */
  int sockUDP;
  struct sockaddr_in addr, cliaddr;
  if((sockUDP = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("Ошибка socket UDP()");
    return -1;
  }
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(PORT);
  if(bind(sockUDP, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    perror("Ошибка bind UDP()");
    return -1;
  }

  int newsock, clnlen;
  int max = sockTCP > sockUDP ? sockTCP : sockUDP;
  while(1){
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sockTCP, &fds);
    FD_SET(sockUDP, &fds);
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int ret = select(max + 1, &fds, NULL, NULL, &tv);
    if(ret == -1){
      perror("Ошибка select()");
      return -1;
    }
    if(FD_ISSET(sockTCP, &fds)){
      if((newsock = accept(sockTCP, (struct sockaddr*)&client, &clnlen)) == -1){ // подключаем нового пользователя
        perror("Ошибка accept()");
        return -1;
      }
      int size;
      char buf[255];
      size = recv(newsock, buf, sizeof(buf), 0);
      send(newsock, buf, sizeof(buf), 0);
      printf("TCP>>>%s\n",buf);
  	  close(newsock);
    } else if(FD_ISSET(sockUDP, &fds)){
      int clilen = sizeof(cliaddr);
      char buf[255];
      recvfrom(sockUDP,buf,sizeof(buf),0,(struct sockaddr*)&cliaddr,&clilen);
      printf("UDP>>>%s\n",buf);
      sendto(sockUDP,buf,sizeof(buf),0,(struct sockaddr*)&cliaddr,clilen);
    }
  }
  close(sockTCP);
  close(sockUDP);
  return 0;
}
