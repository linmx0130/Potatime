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
		filepath=getHomePath();
		filepath+="/.potawake";
		LinuxApi::close(LinuxApi::open(filepath.c_str(),O_CREAT,0666));
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
bool wakeFileExist()
{
	std::string wakefile=getHomePath();
	wakefile+="/.potawake";
	if (!LinuxApi::access(wakefile.c_str(),0))
	{
		LinuxApi::unlink(wakefile.c_str());
		return 1;
	}
	return 0;
}

}
