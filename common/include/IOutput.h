#ifndef IOUTPUT_H
#define IOUTPUT_H
#include <sys/types.h>
#include <libconfig.h++>

class IOutput;
extern "C" IOutput* CreateOutput(libconfig::Setting &s);
typedef IOutput* (*CreateOutputPtr)(libconfig::Setting &s);

class IOutput {
public:
	virtual ~IOutput();
	virtual void Write(const void *pData, size_t nLen) = 0;
};
#endif//IOUTPUT_H
