#include "MyLog.h"

MyLog::MyLog() {
}

void MyLog::Write(char* logline){
	//file<<""<<logline<<endl;
	std::fstream file;
	file.open("Stitching.log",std::ios::app);
	file<<"\n\n"<<logline;
	file<<"\n";
	file.close();
}

void MyLog::Write(const char* logline, ...){
	std::fstream file;
	file.open("Stitching.log",std::ios::app);
	va_list argList;
	char cbuffer[1024];
	va_start(argList, logline);
	vsnprintf(cbuffer, 1024, logline, argList);
	va_end(argList);
	file << cbuffer << endl;
	file.close();
}

MyLog::~MyLog(){
}