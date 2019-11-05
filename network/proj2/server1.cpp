#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <pthread.h>

using namespace std;
void *connection(void *sock);
int clnt_count=0;

int main(){
	int clnt_sock;
	int serv_sock;
	//int clnt_sock;
	int port=10080;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;

	pthread_t tid[10];
	pid_t pid;
	int sock[10];
	int pthread_con;

	

	serv_sock=socket(PF_INET,SOCK_STREAM,0);

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;		// using TCP
	serv_addr.sin_addr.s_addr=/*htonl*/(INADDR_ANY);	// server ip=computer ip
	serv_addr.sin_port=htons(port);		// port num= 10080
	// set serv_addr

	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1){
		puts("bind error");
		return -1;
	}//  bind new socket to address of server
	puts("binded!");
	while(1){
		listen(serv_sock,5);
		puts("wait to accetpt");
		clnt_addr_size=sizeof(clnt_addr);
		if((sock[clnt_count]=accept(serv_sock,(struct sockaddr *)&clnt_addr,
			(socklen_t *)&clnt_addr_size))<0){
			puts("accept error");
		}
		puts("accepted!");
		if((pthread_con=pthread_create(&tid[clnt_count],NULL,
			&connection,(void *)&sock[clnt_count]))!=0){
			puts("thread error");
		}
		clnt_count++;
		sleep(2);
	}
}

void *connection(void *sock){
	int clnt_sock=(int) *((int*)sock);
	char req_buffer[4096];
	char buffer[1024];
	char file_name[100];
	int temp,temp1,temp2;

	printf("new thread create");

	while(1){
		memset(req_buffer,0x00,sizeof(req_buffer));
		int req_len=recv(clnt_sock,req_buffer,sizeof(req_buffer),0);
		if(req_len==-1){
			puts("recv error");
		}
		//recv req
		memset(file_name,0x00,sizeof(file_name));
		char* file_name_pos=strstr(req_buffer," ");
		int count=0;
		for(file_name_pos+=1;*file_name_pos!=32;file_name_pos++){
			file_name[count]=*file_name_pos;
		}
		// get wanted file name
		FILE *fp;
		if((fp=fopen(file_name,"rb"))<0){
			puts("file open error");
		}
		memset(buffer,0x00,sizeof(buffer));
		int len;
		while((len=fread(buffer,sizeof(char),sizeof(buffer),fp))>0){
			std::ostringstream buffer1;
			buffer1 << "HTTP/1.1 200 OK\r\n" 
				<< "Content-Length: " << len << "\r\n"
				<< "Connection: close\r\n"
				<< "Content-Type: text/html; charset=ISO-8859-1\r\n"
				<< "\r\n";
			buffer1 << buffer;
			std::string buffer2=buffer1.str();
			send(clnt_sock,buffer2.c_str(),buffer2.size(),0);
			puts(buffer2.c_str());
			memset(buffer,0x00,sizeof(buffer));
		}

		puts("request=");
		puts(req_buffer);
	}
}