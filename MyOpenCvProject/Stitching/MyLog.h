#include <fstream>
#include<stdarg.h>

using namespace std;

/** 
@File: MyLog.h
@Author: Baran Ornarli
@Description: Class for logging. 
@Use:Log *pLog = new Log("errors.log");
     pLog->Write("Looped: %d times!", i);
	 pLog->Write("Going into our loop");
@url: http://www.infernodevelopment.com/c-log-file-class-forget-debuggers
**/
class MyLog {
public:
	MyLog();
	~MyLog();
	void Write(char* logline);
	void Write(const char* logline, ...);
private:
	//char* fileName;
};