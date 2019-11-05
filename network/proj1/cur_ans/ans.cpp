#include <iostream>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#define buffer_size 1024
using namespace std;

struct Name_data{
	string input_s;
	string output_s;
	int num_thread;
};

int working_thread[100]={0,};
pthread_t threads[100];

void* copy_file(void *temp_name){
	Name_data *thread_name_str=(Name_data *)temp_name;
	FILE *input_F,*output_F;
	char* buffer;
	size_t len;
	int t;

	input_F=fopen(thread_name_str->input_s.c_str(),"rb");
	output_F=fopen(thread_name_str->output_s.c_str(),"wb");
	t=thread_name_str->num_thread;
	buffer=(char *)malloc(buffer_size);

	while((len=fread(buffer,sizeof(char),sizeof(buffer),input_F))!=NULL){
		fwrite(buffer,sizeof(char),len,output_F);
	}

	fclose(input_F);
	fclose(output_F);
	free(buffer);
	free(temp_name);
	working_thread[t]=0;
	cout << "im done!";
	pthread_exit(NULL);
}

int main(){
	int temp,temp1,temp2;
	string input,output;

	while(1){
		//////////////// input part
		cout << "Input the file name: ";
		cin >> input;
		cout << "Input the new name: ";
		cin >> output;
		//////////////// create thread
		Name_data *temp_name;
		temp_name=(Name_data *)malloc(sizeof(Name_data));
		temp_name->input_s=input;
		temp_name->output_s=output;
		for(temp=0;temp<100;temp++){
			if(working_thread[temp]==0){
				working_thread[temp]=1;			// lock needed
				temp_name->num_thread=temp;
				pthread_create(&threads[temp],NULL,copy_file,(void *)temp_name);
				break;
			}
		}
	}
}