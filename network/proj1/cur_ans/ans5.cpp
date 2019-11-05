#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <vector>
#include <time.h>
#define buffer_size 1024

using namespace std;
//// global var
mutex log_lock;
FILE *log_F=fopen("log.txt","wb");

//// thread running func
void* copy_file(std::string input, std::string output){
	//// var
	FILE *input_F,*output_F;
	char buffer[buffer_size];
	size_t len;
	clock_t my_clock;
	//// record start log
	my_clock=clock();
	log_lock.lock();
	fprintf(log_F,"%4f  start copying %s to %s\n",
		(double)my_clock/1000,input.c_str(),output.c_str());
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
	my_clock=clock();
	log_lock.lock();
	fprintf(log_F,"%4f  %s is copied completely\n",
		(double)my_clock/1000,output.c_str());
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
		//////////////// create thread
		working_thread.push_back(thread(copy_file,input,output));
		working_thread[temp-2].detach();
	}
}