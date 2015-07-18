#include <stdio.h>
#include <stdlib.h>
#include "TcpSocket.h"
#include "Type.h"
//soket header
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
static int socketfd;

enum STATUS connectToServer(char* serverIP,unsigned short serverPort,char* playerIP,unsigned short playerPort){
	int count = 0;	
	struct sockaddr_in serverAddr;
	struct sockaddr_in playerAddr;
	SOCK_BEGIN:
	if((socketfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		fprintf(mylog,"socket error:%s\n",strerror(errno));
		exit(1);
	}
	fprintf(mylog,"socket ok!\n");

	/* 设置socket选项，地址重复使用，防止程序非正常退出，下次启动时bind失败 */
	int is_reuse_addr = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&is_reuse_addr, sizeof(is_reuse_addr));

	playerAddr.sin_family = AF_INET;
	playerAddr.sin_port = htons(playerPort);
	playerAddr.sin_addr.s_addr = inet_addr(playerIP);
	//playerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(playerAddr.sin_zero),sizeof(playerAddr.sin_zero));
	if((bind(socketfd,(struct sockaddr*)&playerAddr,sizeof(struct sockaddr)))<0){
		fprintf(mylog,"bind error:%s\n",strerror(errno));
		closeSocket();
		goto SOCK_BEGIN;
		//exit(1);
	}
	fprintf(mylog,"blind ok!\n");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&(serverAddr.sin_zero),sizeof(serverAddr.sin_zero));
	while((connect(socketfd,(struct sockaddr*)&serverAddr,sizeof(struct sockaddr)))<0){
		fprintf(mylog,"connect error:%s\n",strerror(errno));
		count ++;
		sleep(1);
		if(count == 10)
			goto SOCK_BEGIN;
			//exit(1);
	}
	fprintf(mylog,"connect ok!\n");
	return TRUE;
}

int writeMessageToServer(char *message){
	//message = strcat("reg:",playerID);
	//message = strcat(message,"HelloFuture");
	//message = strcat(message,"eol");
	//printf("message:%s\n",message);
	return write(socketfd,message,50);	
}


int readMessageFromServer(char* buff){
	return recv(socketfd,buff,1024,0);
}

void closeSocket(){
	close(socketfd);
}
