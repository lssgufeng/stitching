#include "MyLog.h"

MyLog::MyLog(char* filename) {
	m_stream.open(filename,std::ios::app);
}

void MyLog::Write(char* logline){
  m_stream << logline << endl;
}

void MyLog::Write(const char* logline, ...){
  va_list argList;
  char cbuffer[1024];
  va_start(argList, logline);
  vsnprintf(cbuffer, 1024, logline, argList);
  va_end(argList);
  m_stream << cbuffer << endl;
}

MyLog::~MyLog(){
  m_stream.close();
}