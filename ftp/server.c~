#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

//Server file

int main(int argc,char* arg[]){

	int passive_socket=0;
	
	passive_socket = socket(AF_INET, SOCK_STREAM,0);
	if (passive_socket==-1){
		perror("Failed to create socket");
		return 0;
	}
	/*Attach server address to socket*/
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(21);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); /*Read upon localhost*/
	memset(serverAddr.sin_zero,'\0', sizeof(serverAddr.sin_zero));
	char buffer[1024];
	memset(buffer,0,sizeof(buffer));
	if (bind(passive_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))==-1){
		perror("Failed to bind address to socket");
		return 0;
	}
	if (listen(passive_socket,5) != 0){
		perror("Error in listening");
		return 0;
	}
	struct sockaddr_storage client_addr;
	int sz;
	sz = sizeof(client_addr);
	char command[2], port[4], filename[20];
	int cmd, port_no, count, control_socket, transfer_socket;
	FILE * fileptr;
	while (1){
		control_socket = accept(passive_socket,(struct sockaddr *)&client_addr, &sz);
		printf("Client connected.\nWaiting for commands\n");
		recv(control_socket,command,2,0);
		sscanf(command,"%d",&cmd);
		printf("Command recieved : %d\n",cmd);
		if (cmd==0) {
			/*Client asks for file download, requires port_no and filename */
			recv(control_socket,port,4,0);
			sscanf(port,"%d",&port_no);
			printf("Port number received : %d\n",port_no);
			count = recv(control_socket,filename,sizeof(filename),0);
			printf("Filename recieved : %s\n",filename);
			//open transfer socket
			transfer_socket = socket(AF_INET,SOCK_STREAM,0);
			struct sockaddr_in transfer_addr;
			transfer_addr.sin_family = AF_INET;
			transfer_addr.sin_port = htons(port_no);
			transfer_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			if (connect(transfer_socket, (struct sockaddr *)&transfer_addr, sizeof(transfer_addr))==-1){
				perror("Failed to connect for file transfer");
				return 1;
			}
			fileptr = fopen(filename, "rb");
			if (fileptr==NULL){
				perror("Requested file doesn't exist on the server\n");
				return 1;
			}
			printf("File transfer is starting\n");
			while (1){
				count  = fread(buffer,1,sizeof(buffer),fileptr);
				if (count>0){
					send(transfer_socket,buffer,count,0);
					printf("Sent %d bytes\n",count);
				}
				else if (count == 0){
					close(transfer_socket);
					printf("Sent successful\n");
					break;
				}
				else {
					printf("Read error\n");
				}
			}
		}
	}
	return 0;
}
