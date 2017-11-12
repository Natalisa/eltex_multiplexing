#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#define PORT 3490
#define SERVER "127.0.0.1"

int main(){
    char msg1[] = "Hello!";
    int sock;
    struct sockaddr_in addr,server;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
        perror("Ошибка socket()");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sendto(sock, msg1, sizeof(msg1), 0,
           (struct sockaddr *)&addr, sizeof(addr));
    char buf[1024];
    int serlen = sizeof(server);
    recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&server, &serlen); // прием ответа от сервера
    printf("%s\n",buf);
    close(sock);
    return 0;
}
