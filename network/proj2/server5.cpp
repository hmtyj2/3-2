#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
int port=10080;
char header_404[]="HTTP/1.1 404 Not Found";
char header_200[]="HTTP/1.1 200 OK";
char header_con_len[]="Content-Length: ";
char header_con_con[]="Connection: Keep-Alive";
char header_con_typ[]="Content-Type: ";
//char header_charset[]="charset=ISO-8859-1";

void* connection(int clnt_sock);
void send_response(FILE* fp, char* file_name, char* file_type, int clnt_sock);
void login(int clnt_sock);

int main(){
	struct sockaddr_in servaddr,clntaddr;
	int serv_sock;
	int clnt_sock;
	int clnt_addr_size;
	vector<thread> working_thread;
	// make socket
	if((serv_sock=socket(PF_INET,SOCK_STREAM,0))<0){
		perror("socket fail");
		exit(0);
	}
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	//bind
	if(bind(serv_sock,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
		perror("bind fail");
		exit(0);
	}
	//listen(serv_sock,5);
	while(1){
		listen(serv_sock,5);
		puts("wait for sig");
		// accept
		clnt_addr_size=sizeof(clntaddr);
		if((clnt_sock=accept(serv_sock,(struct sockaddr *)&(clntaddr),(socklen_t *)&(clnt_addr_size)))<0){
			puts("accept fail");
		}
		puts("making thread");
		// give to thread
		working_thread.push_back(thread(connection,clnt_sock));///////////
		printf("hello new thread\n");
	}
}

void* connection(int clnt_sock){
	char req_buf[4096];
	char file_name[100];
	char file_type[100];

	while(1){
		// recv request
		memset(req_buf,0x00,sizeof(req_buf));
		int req_len=recv(clnt_sock,req_buf,sizeof(req_buf),0);
		if(req_len==-1){
			perror("recv fail");
		}
		/*puts("the request is====");
		puts(req_buf);*/
		//get file name
		memset(file_name,0x00,sizeof(file_name));
		char *file_name_pos=strchr(req_buf,'/');
		int count=0;
		for((file_name_pos)+=1;*file_name_pos!=32;(file_name_pos)++,count++){
			file_name[count]=*file_name_pos;
		}
		//get file type
		memset(file_type,0x00,sizeof(file_type));
		char *file_type_t=strstr(req_buf,"Accept:");
		count=0;
		for((file_type_t)+=8;*file_type_t!=',';(file_type_t)++,count++){
			file_type[count]=*file_type_t;
		}
		if(file_name[0]=='\0'){
			login(clnt_sock);
			continue;
		}
		// open file
		FILE *fp=fopen(file_name,"rb");
		if(fp==NULL){
			//perror("there are no such file");
			ostringstream temps;
			temps << header_404 << "\r\n"
				<< header_con_len << "0" << "\r\n\r\n";
			string tempss=temps.str();
			send(clnt_sock,tempss.c_str(),tempss.size(),0);
			continue;
		}
		/*puts("file name is====");
		puts(file_name);
		puts("file_type is====");
		puts(file_type);*/
		//send response
		send_response(fp,file_name,file_type,clnt_sock);
		fclose(fp);
	}
}

void send_response(FILE* fp, char* file_name, char* file_type, int clnt_sock){
	int len;
	char buffer[10000];
	char packet[512];
	int temp=0;
	// get file len
	fseek(fp,0,SEEK_END);
	len=ftell(fp);
	fseek(fp,0,SEEK_SET);
	// make file header
	/*printf("length====%d\n",len);*/
	memset(buffer,0x00,sizeof(buffer));
	int tt;
	while((tt=fread(buffer,sizeof(char),sizeof(buffer),fp))>0){
		std::ostringstream body;
		if(strstr(file_name,"html")==NULL){
		body << header_200 << "\r\n"
			<< header_con_len << tt << "\r\n"
			<< header_con_con << "\r\n"
			<< header_con_typ << "application/octet-stream" << "\r\n"
			<< "\r\n";
		}
		else{
		body << header_200 << "\r\n"
			<< header_con_len << tt << "\r\n"
			<< header_con_con << "\r\n"
			<< header_con_typ << file_type << "\r\n"
			<< "\r\n";
		}
		/*printf("\ntt==%d\n",tt);*/
		string body1=body.str();
		send(clnt_sock,body1.c_str(),body1.size(),0);
		/*printf("header==%d\n",body1.size());
		puts(body1.c_str());*/
		send(clnt_sock,buffer,(size_t)tt,0);
		/*for(int temp=0;temp<tt;temp++){
			printf("%c",buffer[temp]);
		}*/
		memset(buffer,0x00,sizeof(buffer));
	}
}

void login(int clnt_sock){
	char buffer[10000];
	char req_buf[4096];
	int len;
	FILE *fp=fopen("index.html","rb");
	memset(req_buf,0x00,sizeof(req_buf));
	memset(buffer,0x00,sizeof(buffer));
	len=fread(buffer,sizeof(char),sizeof(buffer),fp);

	ostringstream body;
	body << header_200 << "\r\n"
		<< header_con_len << len <<"\r\n"
		<<header_con_con <<"\r\n"
		<< header_con_typ << "text/html" << "\r\n\r\n";
	string body1=body.str();
	send(clnt_sock,body1.c_str(),body1.size(),0);
	send(clnt_sock,buffer,(size_t)len,0);
	int req_len=recv(clnt_sock,req_buf,sizeof(req_buf),0);
	if(req_len==-1){
		perror("recv fail");
	}
	puts("the request is====");
	puts(req_buf);
	memset(req_buf,0x00,sizeof(req_buf));
	req_len=recv(clnt_sock,req_buf,sizeof(req_buf),0);
	if(req_len==-1){
		perror("recv fail");
	}
	puts("the second request is=====");
	puts(req_buf);
}