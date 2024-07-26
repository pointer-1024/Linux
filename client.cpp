#include<iostream>
#include<cstring>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
  //第一步:创建客户端的socket
  int sockfd = socket(AF_INET,SOCK_STREAM,0);

  //第二步:向服务器发起连接请求
  struct hostent *h;
  if((h = gethostbyname("192.168.146.89")) == 0){
    perror("gethostbyname");
    close(sockfd);
    return -1;
  }

  struct sockaddr_in seraddr;
  memset(&seraddr,0,sizeof(seraddr));
  seraddr.sin_family = AF_INET;
  seraddr.sin_port = htons(8008);
  memcpy(&seraddr.sin_addr,h->h_addr,h->h_length);
  if(connect(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr)) != 0){
    perror("connect");
    close(sockfd);
    return -1;
  }

  char buffer[1024];

  while(1){
    memset(buffer,0,sizeof(buffer));
    std::cin >> buffer;
    if(send(sockfd,buffer,sizeof(buffer),0) <= 0) break;
    std::cout << "发送: " << buffer << std::endl;
    
    memset(buffer,0,sizeof(buffer));
    if(recv(sockfd,buffer,sizeof(buffer),0) <= 0) break;
    std::cout << "接收：" << buffer << std::endl;
  }

  close(sockfd);

  return 0;
}
