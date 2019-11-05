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

int main(){
	char header_404[]="HTTP/1.1 404 Not Found";
	char header_200[]="HTTP/1.1 200 OK";
	char header_con_len[]="Content-Length: ";
	char header_con_con[]="Connection: Keep-Alive";
	char header_con_typ[]="Content-Type: ";
	char buffer[50];
	char packet[1024];

	FILE *fp=fopen("hello.html","rb");
	int tt;
	while((tt=fread(buffer,sizeof(char),sizeof(buffer),fp))>0){
		std::ostringstream body;
		if(strstr(file_type,"image")==NULL){
		body << header_200 << "\r\n"
			<< header_con_len << tt << "\r\n"
			<< header_con_con << "\r\n"
			<< header_con_typ << file_type << "\r\n"
			<< "\r\n";
		}
		else{
		body << header_200 << "\r\n"
			<< header_con_len << tt << "\r\n"
			<< header_con_con << "\r\n"
			<< header_con_typ << "image/jpeg" << "\r\n"
			<< "\r\n";
		}
		printf("header_size=%d\n",sizeof(body));
		body << buffer;
		memset(buffer,0x00,sizeof(buffer));
		string body1=body.str();
		printf("total size=%d\n",body1.size());
		puts(body1.c_str());
		send(clnt_sock,body1.c_str(),body1.size()-1,0);
		memset(packet,0x00,sizeof(packet));
		printf("size=%d\n",sizeof(packet));
	}
}