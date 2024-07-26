#include<iostream>
#include<cstring>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main()
{
  //第一步:创建服务端的socket
  int listenfd = socket(AF_INET,SOCK_STREAM,0);

  //第二步:把服务端用于通信的地址和端口绑定到socket上
  struct sockaddr_in servaddr;
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(8008);
  if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr)) != 0){
    perror("bind");
    close(listenfd);
    return -1;
  }
  //第三步:把socket设置为监听模式
  if(listen(listenfd,5) != 0){
    perror("listen");
    close(listenfd);
    return -1;
  }
  //第四步:接收客户端的连接
  int clientfd;
  int socklen = sizeof(struct sockaddr_in);
  struct sockaddr_in clientaddr;
  clientfd = accept(listenfd,(struct sockaddr*)&clientaddr,(socklen_t *)&socklen);
  std::cout << "客户端 " << inet_ntoa(clientaddr.sin_addr) << " 已连接" << std::endl;
  //第五步:与客户端通信,接收客户端发过来的报文后,回复处理结果
  char buffer[1024];

  while(1){
    memset(buffer,0,sizeof(buffer));

    if(recv(clientfd,buffer,sizeof(buffer),0) <= 0) break;
    std::cout << "接收：" << buffer << std::endl;

    strcpy(buffer,"ok");
    if(send(clientfd,buffer,strlen(buffer),0) <= 0) break;
    std::cout << "发送：" << buffer << std::endl;
  }

  //第六步:不断重复第五步,直到客户端断开连接
  //第七步:关闭socket,释放资源
  close(listenfd);
  close(clientfd);


  return 0;
}
