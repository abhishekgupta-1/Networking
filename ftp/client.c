#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>
#include <errno.h>


//Client Program

int main(int argc,char* arg[]){
	int control_socket = socket(AF_INET,SOCK_STREAM,0);
	if (control_socket==-1){
		perror("Failed to open socket!");
	}
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(21);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));
	connect(control_socket,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
	char buffer[1024];
	char filename[20];
	int sz, cmd, count, port_no;
	char command[2], port[4];
	printf("Enter your command\n");
	scanf("%d",&cmd);
	sprintf(command,"%d",cmd);
	if (cmd == 0){
		send(control_socket,command,2,0);
		printf("Enter port no : ");
		scanf("%d",&port_no);
		sprintf(port,"%d",port_no);
		send(control_socket,port,4,0);
		printf("Enter filename : ");
		scanf("%s",filename);
		send(control_socket,filename,strlen(filename)+1,0);
		//Create a listening connection on client
		int passive_socket = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in listeningaddr;
		listeningaddr.sin_family = AF_INET;
		listeningaddr.sin_port = htons(port_no);
		listeningaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		memset(&(listeningaddr.sin_zero), '\0', sizeof(listeningaddr.sin_zero));
		if (bind(passive_socket,(struct sockaddr*)&listeningaddr,sizeof(listeningaddr))<0){
			perror("Failed to bind passive socket : ");
			return 1;
		}
		if (listen(passive_socket,5)<0){
			perror("Failed to listen on passive socket : ");
			return 1;
		}
		int download_socket = accept(passive_socket,(struct sockaddr *)NULL,NULL); //no need to store the client (actually server) address
		
		printf("Downloading......\n");
		FILE * fileptr = fopen("temp.txt","ab");
		while (1){
			count = read(download_socket,buffer,sizeof(buffer));
			if (count>0){
				fwrite(buffer,1,count,fileptr);
				printf("Read %d bytes\n",count);
			}
			else break;
		}
		fclose(fileptr);
	}

	return 0;
}
	
