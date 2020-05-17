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
	server_sockaddr.sin_port = htons(PORT);//port��Χ0~65535
	server_sockaddr.sin_addr.s_addr = inet_addr(SERVER_IP);//inet_addr ���ַ�����ʽ��IP��ַ -> �����ֽ�˳�� ������ֵ;htonl(INADDR_ANY)��ָ����IP
	memset(&(server_sockaddr.sin_zero),0,sizeof(server_sockaddr.sin_zero));

	int i=1;/*�����ظ�ʹ�ñ��ص�ַ���׽��ֽ��а�*/
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));

	/*�󶨺���bind()*/
	if(bind(listenfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
	{
		perror("bind");
		exit(1);
	}
	
	/*��������listen()*/
	if(listen(listenfd,MAX_QUE_CONN_NM)==-1)
	{
		perror("listen");
		exit(1);
	}
	
	/*����accpet()���ȴ��ͻ�������*/
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
	memcpy(buff2,content,strlen(content)+1);//sizeof(content)=8,��ʾ����ָ��Ĵ�С�Ǵ�����
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


