#ifndef IGENERATOR_H
#define IGENERATOR_H
#include "CColor.h"
#include <vector>

namespace libconfig {
	class Setting;
}

class CTime;
class IGenerator;
class IFrameScheduler;
extern "C" IGenerator* CreateGenerator(unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments);
typedef IGenerator* (*CreateGeneratorPtr)(unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments);

class IGenerator {
private:
	volatile unsigned long m_nRef;
protected:
	virtual ~IGenerator();
	IGenerator();
public:
	void AddRef();
	void Release();
	//Generate a frame
	//If this generator should be replaced by another one, return true and set ppNextGenerator (and increase the reference count)
	virtual bool Generate(CColor *pColors, IGenerator **ppNextGenerator) = 0;
};
#endif//IGENERATOR_H
