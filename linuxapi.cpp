#include "sysapi.h"
namespace LinuxApi
{
	#include<unistd.h>
	#include<stdlib.h>
	#include <fcntl.h>
}
namespace Sysapi
{

std::string getHomePath()
{
	char *path=LinuxApi::getenv("HOME");
	std::string ret(path);
	return ret;
}

void creatLock()
{
	std::string filepath=getHomePath();
	filepath+="/.potatimelock";
	int lock_desc=LinuxApi::open(filepath.c_str(),O_RDWR|O_CREAT|O_EXCL,0444);
	if (lock_desc==-1)
	{
			LinuxApi::exit(1);
	}
}

void deleteLock()
{
	std::string filepath=getHomePath();
	filepath+="/.potatimelock";
	LinuxApi::unlink(filepath.c_str());
}
void Quit(int exitnum)
{
	LinuxApi::exit(exitnum);
}
}
