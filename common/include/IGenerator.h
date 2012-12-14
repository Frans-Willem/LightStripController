#ifndef IGENERATOR_H
#define IGENERATOR_H
#include "CColor.h"

namespace libconfig {
	class Setting;
}

class IGenerator;
extern "C" IGenerator* CreateGenerator(unsigned int nLength, libconfig::Setting &s);
typedef IGenerator* (*CreateGeneratorPtr)(unsigned int nLength, libconfig::Setting &s);

class IGenerator {
private:
	volatile unsigned long m_nRef;
protected:
	virtual ~IGenerator();
	IGenerator();
public:
	void AddRef();
	void Release();
	virtual void Generate(CColor *pColors) = 0;
};
#endif//IGENERATOR_H
