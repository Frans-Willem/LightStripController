#ifndef IOUTPUT_H
#define IOUTPUT_H
#include <sys/types.h>

class CConfigObject;
class IOutput;
extern "C" IOutput* CreateOutput(CConfigObject *s);
typedef IOutput* (*CreateOutputPtr)(CConfigObject *s);

class IOutput {
public:
	virtual ~IOutput();
	virtual void Write(const void *pData, size_t nLen) = 0;
};
#endif//IOUTPUT_H
