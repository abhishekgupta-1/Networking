#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

//Client Program

void* listening_funct(void * argptr){
	char buffer[1024];
	int socket_no = *((int*)argptr);
	while (1){
		recv(socket_no,buffer,sizeof(buffer),0);
		printf("Server : %s",buffer);
	}
	return;
}

int main(int argc,char* arg[]){
	if (argc<3){
		printf("Usage : Program [IP ADDRESS] [PORT]\n");
		return 0;
	}
	int port_no = atoi(arg[2]);
	int socket_no = socket(AF_INET,SOCK_STREAM,0);
	if (socket_no==-1){
		perror("Failed to open socket!");
	}
	struct sockaddr_in serverAddr;
	int addr_sz = 0;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port_no);
	serverAddr.sin_addr.s_addr = inet_addr(arg[1]);
	memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));
	addr_sz = sizeof(serverAddr);
	connect(socket_no,(struct sockaddr *)&serverAddr,addr_sz);
	char buffer[1024];
	char * inp = NULL;
	int sz;
	pthread_t thread;
	int su = pthread_create(&thread,NULL,listening_funct,&socket_no);
	while (1){
		getline(&inp,&sz,stdin);
		send(socket_no,inp,strlen(inp)+1,0);
		//printf("Client : %s",inp);
	}
	return 0;
}
	
