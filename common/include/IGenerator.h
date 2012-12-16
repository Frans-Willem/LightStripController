#ifndef IGENERATOR_H
#define IGENERATOR_H
#include "CColor.h"
#include <vector>

namespace libconfig {
	class Setting;
}

class CTime;
class IGenerator;
extern "C" IGenerator* CreateGenerator(std::vector<IGenerator*> lArguments, unsigned int nLength, libconfig::Setting &s);
typedef IGenerator* (*CreateGeneratorPtr)(std::vector<IGenerator*> lArguments, unsigned int nLength, libconfig::Setting &s);

class IGenerator {
private:
	volatile unsigned long m_nRef;
protected:
	virtual ~IGenerator();
	IGenerator();
public:
	void AddRef();
	void Release();
	virtual void Generate(CColor *pColors, CTime &timeNextFrame, IGenerator **ppNextGenerator) = 0;
};
#endif//IGENERATOR_H
