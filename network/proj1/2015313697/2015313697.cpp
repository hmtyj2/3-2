#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#define buffer_size 1024

using namespace std;
using namespace chrono;
//// global var
mutex log_lock;
FILE *log_F=fopen("log.txt","wb");
system_clock::time_point start=system_clock::now();
//// thread running func
void* copy_file(std::string input, std::string output){
	//// var
	FILE *input_F,*output_F;
	char buffer[buffer_size];
	size_t len;
	system_clock::time_point end;
	duration<double> sec;
	//// record start log
	log_lock.lock();
	end=system_clock::now();
	sec=end-start;
	fprintf(log_F,"%.2f  start copying %s to %s\n",
		sec,input.c_str(),output.c_str());
	fflush(log_F);
	log_lock.unlock();
	//// open input and output files
	input_F=fopen(input.c_str(),"rb");
	output_F=fopen(output.c_str(),"wb");
	//// read input -> copy to buffer -> write to new file
	while((len=fread(buffer,sizeof(char),sizeof(buffer),input_F))>0){
		fwrite(buffer,sizeof(char),len,output_F);
	}
	//// close input and output file
	fclose(input_F);
	fclose(output_F);
	//// record end log
	log_lock.lock();
	end=system_clock::now();
	sec=end-start;
	fprintf(log_F,"%.2f  %s is copied completely\n",
		sec,output.c_str());
	fflush(log_F);
	log_lock.unlock();
}

int main(){
	//// var
	int temp=1;
	string input,output;
	vector<thread> working_thread;

	while(temp++){
		//// get file names
		cout << "Input the file name: ";
		cin >> input;
		cout << "Input the new name: ";
		cin >> output;
		//// create thread
		working_thread.push_back(thread(copy_file,input,output));
		working_thread[temp-2].detach();
	}
}