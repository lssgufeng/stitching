#include "MyLog.h"

MyLog::MyLog() {
}

void MyLog::Write(char* logline){
	m_stream.open("Stitching.log",std::ios::app);	
	m_stream << logline << endl;
	m_stream.close();
}

void MyLog::Write(const char* logline, ...){
	
	m_stream.open("Stitching.log",std::ios::app);	
	va_list argList;
	char cbuffer[1024];
	va_start(argList, logline);
	vsnprintf(cbuffer, 1024, logline, argList);
	va_end(argList);
	m_stream << cbuffer << endl;
	m_stream.close();
}

MyLog::~MyLog(){
	m_stream.close();
}