#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

//Server file


void* listening_funct(void * argptr){
	char buffer[1024];
	int socket_no = *((int*)argptr);
	while (1){
		recv(socket_no,buffer,sizeof(buffer),0);
		printf("Client : %s",buffer);
	}
	return;
}

int main(int argc,char* arg[]){
	
	if (argc<2){
		printf("Usage : Program [Port]\n");
		return 0;
	}
	int port_no = atoi(arg[1]);
	int server_socket=0,new_socket=0;
	
	server_socket = socket(AF_INET, SOCK_STREAM,0);
	if (server_socket==-1){
		perror("Failed to create socket");
		return 0;
	}

	/*Attach server address to socket*/
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port_no);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /*Read upon localhost*/
	memset(serverAddr.sin_zero,'\0', sizeof(serverAddr.sin_zero));
	char buffer[1024];
	memset(buffer,0,sizeof(buffer));
	/*Note :- type of &serverAddr is typecasted to (struct sockaddr)* */
	if (bind(server_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))==-1){
		perror("Failed to bind address to socket");
		return 0;
	}
	if (listen(server_socket,5) != 0){
		perror("Error in listening");
		return 0;
	}
	struct sockaddr_storage client_addr;
	int sz;
	sz = sizeof(client_addr);
	new_socket = accept(server_socket,(struct sockaddr *)&client_addr, &sz);
	printf("Client connected.\n");
	strcpy(buffer,"Welcome to hop world!\n");
	send(new_socket,buffer,strlen(buffer)+1,0);
	char * inp = NULL;
	pthread_t thread;
	int succ = pthread_create(&thread,NULL,listening_funct,&new_socket);
	while (1) {
		getline(&inp,&sz,stdin);
		send(new_socket,inp,strlen(inp)+1,0);
		//printf("Server : %s",inp);
	}
	return 0;
}
