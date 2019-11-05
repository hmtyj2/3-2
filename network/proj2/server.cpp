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

using namespace std;

int main(){
	char req_buffer[4096];
	char res_buffer[4096];
	char buffer[4096];
	int serv_sock;
	int clnt_sock;
	char id[20]="hmtyj2";
	char password[20]="hmt0301";
	char clnt_id[20];
	char clnt_password[20];
	int port=10080;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	int temp,temp1,temp2;
	int req_len;

	serv_sock=socket(PF_INET,SOCK_STREAM,0);		// create socket

	memset(&serv_addr,0,sizeof(serv_addr));		// initialize serv_addr
	serv_addr.sin_family=AF_INET;		// using TCP
	serv_addr.sin_addr.s_addr=/*htonl*/(INADDR_ANY);	// server ip=computer ip
	serv_addr.sin_port=htons(port);		// port num= 10080

	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))==-1){
		puts("bind error");
		return -1;
	}		//  bind new socket to address of server
	listen(serv_sock,5);
	puts("wait to accept");
	clnt_addr_size=sizeof(clnt_addr);
	if((clnt_sock=accept(serv_sock,(struct sockaddr *)&clnt_addr,
		(socklen_t*)&clnt_addr_size))<0){
		puts("accept error");
	}

	puts("accepted!");
	////// TCP connection estabilished


	memset(req_buffer,0x00,sizeof(req_buffer));
	req_len=recv(clnt_sock,req_buffer,sizeof(req_buffer),0);
	if(req_len==-1){
		puts("recv error");
	}
	puts(req_buffer);
	// get req message;

	FILE *fp=fopen("hello.html","rb");
	int len=fread(buffer,sizeof(char),sizeof(buffer),fp);
	// read html file
	std::ostringstream buffer1;
	buffer1 << "HTTP/1.1 200 OK\r\n" 
		<< "Content-Length: " << len << "\r\n"
		<< "Connection: close\r\n"
		<< "Content-Type: text/html; charset=ISO-8859-1\r\n"
		<< "\r\n";
	buffer1 << buffer;
	string responseb=buffer1.str();
	send(clnt_sock,responseb.c_str(),responseb.size(),0);
	puts(responseb.c_str());
	// send response message


	close(clnt_sock);
	close(serv_sock);

	return 0;

	

}