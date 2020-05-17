/*server.c*/
#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#define SERVER_IP "192.168.192.128"
#define PORT 8100
#define BUFFER_SIZE 1024
#define MAX_QUE_CONN_NM 5

int main()
{
	struct sockaddr_in server_sockaddr,client_sockaddr;
	int sin_size,recvbytes;
	int listenfd,connfd;
	char buff1[BUFFER_SIZE];
	char buff2[BUFFER_SIZE];
	char *content="hello,boy~";
	int transnum = 5;

	if((listenfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}

	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);//port范围0~65535
	server_sockaddr.sin_addr.s_addr = inet_addr(SERVER_IP);//inet_addr 将字符串形式的IP地址 -> 网络字节顺序 的整型值;htonl(INADDR_ANY)泛指本机IP
	memset(&(server_sockaddr.sin_zero),0,sizeof(server_sockaddr.sin_zero));

	int i=1;/*允许重复使用本地地址与套接字进行绑定*/
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));

	/*绑定函数bind()*/
	if(bind(listenfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
	{
		perror("bind");
		exit(1);
	}
	
	/*监听函数listen()*/
	if(listen(listenfd,MAX_QUE_CONN_NM)==-1)
	{
		perror("listen");
		exit(1);
	}
	
	/*调用accpet()，等待客户端连接*/
	sin_size = sizeof(client_sockaddr);
	if((connfd = accept(listenfd,(struct sockaddr *)&client_sockaddr,&sin_size))==-1)
	{
		perror("accept");
		exit(1);
	}
while(transnum--)
{
	memset(buff1,0,sizeof(buff1));
	if((recvbytes=recv(connfd,buff1,BUFFER_SIZE,0))==-1)
	{
		perror("recv");
		exit(1);
	}
	printf("recv mesg:%s\n",buff1);

	memset(buff2,0,sizeof(buff2));
	memcpy(buff2,content,strlen(content)+1);//sizeof(content)=8,显示的是指针的大小非串长度
	if(send(connfd,buff2,strlen(content)+1,0)==-1)
	{
		perror("send");
		exit(1);
	}
	sleep(5);
}
	close(listenfd);	
	exit(0);
}


