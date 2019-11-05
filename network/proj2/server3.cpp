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
char header_200[]="HTTP/1.1 200 OK";
char header_con_len[]="Content-Length: ";
char header_con_con[]="Connection: keep alive";
char header_con_typ[]="Content-Type: ";
//char header_charset[]="charset=ISO-8859-1";

void* connection(struct Net_str *n);
void send_response(FILE* fp, char* file_name, int clnt_sock);

struct Net_str{
	struct sockaddr_in servaddr,clntaddr;
	int serv_sock;
	int clnt_sock;
	int clnt_addr_size;
};

int main(){
	vector<thread> working_thread;
	while(1){
		struct Net_str *net_str=(Net_str*)malloc(sizeof(Net_str));
		// make socket
		if((net_str->serv_sock=socket(PF_INET,SOCK_STREAM,0))<0){
			perror("socket fail");
			exit(0);
		}
		memset(&(net_str->servaddr),0,sizeof(net_str->servaddr));
		net_str->servaddr.sin_family = AF_INET;
		net_str->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		net_str->servaddr.sin_port = htons(port);
		//bind
		if(bind(net_str->serv_sock,(struct sockaddr *)&(net_str->servaddr),
			sizeof(net_str->servaddr))<0){
			perror("bind fail");
			exit(0);
		}
		listen(net_str->serv_sock,5);
		puts("wait for sig");
		// accept
		net_str->clnt_addr_size=sizeof(net_str->clntaddr);
		if((net_str->clnt_sock=accept(net_str->serv_sock,(struct sockaddr *)
			&(net_str->clntaddr),(socklen_t *)&(net_str->clnt_addr_size)))<0){
			puts("accept fail");
		}
		puts("making thread");
		// give to thread
		working_thread.push_back(thread(connection,net_str));///////////
	}
}

void* connection(struct Net_str *n){
	char req_buf[4096];
	char file_name[100];

	while(1){
		// recv request
		memset(req_buf,0x00,sizeof(req_buf));
		int req_len=recv(n->clnt_sock,req_buf,sizeof(req_buf),0);
		if(req_len==-1){
			perror("recv fail");
		}
		puts("the request is====");
		puts(req_buf);
		//get file name
		memset(file_name,0x00,sizeof(file_name));
		char *file_name_pos=strchr(req_buf,'/');
		int count=0;
		for((file_name_pos)+=1;*file_name_pos!=32;(file_name_pos)++,count++){
			file_name[count]=*file_name_pos;
		}
		// open file
		FILE *fp=fopen(file_name,"rb");
		if(fp==NULL){
			perror("there are no such file");
			continue;
		}
		puts("file name is====");
		puts(file_name);
		//send response
		send_response(fp,file_name,n->clnt_sock);
		fclose(fp);
	}
}

void send_response(FILE* fp, char* file_name, int clnt_sock){
	int len;
	char buffer[1024];
	// get file len
	fseek(fp,0,SEEK_END);
	len=ftell(fp);
	fseek(fp,0,SEEK_SET);
	// make file header
	std::ostringstream buffer1;
	buffer1 << header_200 << "\r\n"
		<< header_con_len << len << "\r\n"
		<< header_con_con << "\r\n"
		<< "\r\n";
	// make body
	memset(buffer,0x00,sizeof(buffer));
	while((fread(buffer,sizeof(char),sizeof(buffer),fp))>0){
		buffer1 << buffer;
		memset(buffer,0x00,sizeof(buffer));
	}
	// send data
	string buffer2=buffer1.str();
	send(clnt_sock,buffer2.c_str(),buffer2.size(),0);
}