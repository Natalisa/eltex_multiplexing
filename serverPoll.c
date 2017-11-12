#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>

#define PORT 3490
#define TIME 5
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

  while(1){
    struct pollfd fds[2];
    fds[0].fd = sockTCP;
    fds[0].events = POLLIN;
    fds[1].fd = sockUDP;
    fds[1].events = POLLIN;
    int ret = poll(fds,2,TIME * 1000);
    if(ret == -1){
      perror("Ошибка poll()");
      return -1;
    }
    if(fds[0].revents & POLLIN) {
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
    }
    if(fds[1].revents & POLLIN){
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
