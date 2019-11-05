#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <iostream>
#include <sstream>

#define MAXLINE 1024 
#define LISTENQ 10
#define THREAD_NUM 5

using namespace std;

void *thrfunc(void *arg);

int result = 0;
int cntNum = 0; //client count

//mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	struct sockaddr_in servaddr, cliaddr;
	int listen_sock, accp_sock[THREAD_NUM];
	int addrlen = sizeof(servaddr);
	int i, status ;
	pthread_t tid[THREAD_NUM];
	pid_t pid;
	int port=10080;
	
	if((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket Fail");
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	//bind
	if(bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind Fail");
		exit(0);
	}

	while(1) {
		listen(listen_sock, LISTENQ);

		puts("client wait....");

		accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr,
		 (socklen_t *)&addrlen);
		if(accp_sock[cntNum] < 0) {
			perror("accept fail");
			exit(0);
		}

		if((status = pthread_create(&tid[cntNum], NULL, &thrfunc, (void *) &accp_sock[cntNum])) != 0) {
			printf("%d thread create error: %s\n", cntNum, strerror(status));
			exit(0);
		}
		cntNum++;

		/*//인자로 지정한 스레드 id가 종료하기를 기다립니다.
		pthread_join(tid[cntNum], NULL);
		cntNum++;
		if(cntNum == 5)
			cntNum = 0;*/
	}

	return 0;
}

void *thrfunc(void *arg) {
	int clnt_sock=(int) *((int*)arg);
	char req_buffer[4096];
	char buffer[512];
	char file_name[100];
	int temp,temp1,temp2;

	puts("new thread create");

	while(1){
		memset(req_buffer,0x00,sizeof(req_buffer));
		int req_len;
		req_len=recv(clnt_sock,req_buffer,sizeof(req_buffer),0);
		if(req_len==-1){
			puts("recv error");
		}
		puts("request=");
		puts(req_buffer);

		//recv req
		memset(file_name,0x00,sizeof(file_name));
		char* file_name_pos=strchr(req_buffer,'/');
		int count=0;

		for((file_name_pos)+=1;*file_name_pos!=32;(file_name_pos)++,count++){
			file_name[count]=*file_name_pos;
		}
		char* test;
		/*if((test=strstr(file_name,"hello.html"))==NULL){
			puts("it's not hello..");
			continue;
		}*/
		// get wanted file name
		FILE *fp;
		if((fp=fopen(file_name,"rb"))==NULL){
			puts("there are no such file");
			continue;
		}
		puts("file name is:");
		puts(file_name);
		memset(buffer,0x00,sizeof(buffer));
		// open wanted file if exist

		int len;
		fseek(fp,0,SEEK_END);
		int total_len=ftell(fp);
		fseek(fp,0,SEEK_SET);
		int counter=0;
		while((len=fread(buffer,sizeof(char),sizeof(buffer),fp))>0){
			std::ostringstream buffer1;
			buffer1 << "HTTP/1.1 200 OK\r\n" 
				<< "Content-Length: " << total_len << "\r\n"
				<< "Connection: keep alive\r\n"
				//<< "Content-Type: text/html; charset=ISO-8859-1\r\n"
				<< "\r\n";
			buffer1 << buffer;
			string buffer2=buffer1.str();
			send(clnt_sock,buffer2.c_str(),buffer2.size(),0);
			//puts(buffer2.c_str());
			memset(buffer,0x00,sizeof(buffer));
		}
		//read and send the file data
		
	}
}
/*void send_data(int clnt_sock, FILE *fp){
	int len;
	char buffer[1024];
	while((len=fread(buffer,sizeof(char),sizeof(buffer),fp))>0){
		std::ostringstream buffer1;
		buffer1 << "HTTP/1.1 200 OK\r\n"
			<< "Content-Length: " << len << "\r\n"
			<< "Connection: keep alive\r\n"
			<< "Content-Type: text/html; charset=ISO-8859-1\r\n"
			<< "\r\n";
		buffer1 << buffer;
		string buffer2=buffer1.str();
		send(clnt_siock,buffer2.c_str(),buffer2.size(),0);
		memset(buffer,0x00,sizeof(buffer));
	}

}*/