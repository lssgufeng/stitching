#include "MyLog.h"

MyLog::MyLog() {
}

void MyLog::Write(char* filename,char* logline){
	//file<<""<<logline<<endl;
	std::fstream file;
	file.open(filename,std::ios::app);
	file<<endl<<endl<<logline;
	file<<endl;
	file.close();
}

void MyLog::Write(char* filename,const char* logline, ...){
	std::fstream file;
	file.open(filename,std::ios_base::app);
	va_list argList;
	char cbuffer[1024];
	va_start(argList, logline);
	vsnprintf(cbuffer, 1024, logline, argList);
	va_end(argList);
	file<<cbuffer << endl;
	file.close();
}

MyLog::~MyLog(){
}