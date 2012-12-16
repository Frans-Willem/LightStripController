#ifndef CMODULEGENERATOR_H
#define CMODULEGENERATOR_H
#include "IGenerator.h"
#include <string>
namespace libconfig {
	class Setting;
}

class CModuleGenerator : public IGenerator {
private:
	void *m_pModule;
	IGenerator *m_pGenerator;
	CModuleGenerator(void *pModule, IGenerator *pGenerator);
public:
	~CModuleGenerator();
	bool Generate(CColor *pColors, IGenerator **ppNextGenerator);
	static CModuleGenerator *Create(std::string strModule, unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments);
};
#endif//CMODULEGENERATOR_H
