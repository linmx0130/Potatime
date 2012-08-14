#ifndef SYSAPI_H
#define SYSAPI_H
#include <string>
namespace Sysapi
{
std::string getHomePath();
void creatLock();
void deleteLock();
void Quit(int);
}
#endif
