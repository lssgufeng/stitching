#include "MyLog.h"

MyLog::MyLog() {
	
}

void MyLog::Write(char* logline){
	//file<<""<<logline<<endl;
	std::fstream file;
	file.open("stitching.log",std::ios::app);
	file<<endl<<endl<<logline;
	file<<endl;
	file.close();
}

void MyLog::Write(const char* logline, ...){
	std::fstream file;
	file.open("stitching.log",std::ios::app);
	va_list argList;
	char cbuffer[1024];
	va_start(argList, logline);
	vsnprintf(cbuffer, 1024, logline, argList);
	va_end(argList);
	file <<cbuffer << endl;
	//file <<endl<<endl<<cbuffer << endl;
	file.close();
}

MyLog::~MyLog(){
}