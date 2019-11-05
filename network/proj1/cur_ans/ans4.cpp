#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <vector>
#define buffer_size 1024

using namespace std;

void* copy_file(std::string input, std::string output){

	FILE *input_F,*output_F;
	char buffer[buffer_size];
	size_t len;

	input_F=fopen(input.c_str(),"rb");
	output_F=fopen(output.c_str(),"wb");

	while((len=fread(buffer,sizeof(char),sizeof(buffer),input_F))>0){
		fwrite(buffer,sizeof(char),len,output_F);
	}

	fclose(input_F);
	fclose(output_F);
	printf("im done!\n");
}

int main(){
	int temp,temp1,temp2;
	string input,output;
	vector<thread> working_thread;

	while(1){
		//////////////// input part
		cout << "Input the file name: ";
		cin >> input;
		cout << "Input the new name: ";
		cin >> output;
		//////////////// create thread
		working_thread.push_back(thread(copy_file,input,output));
	}
}