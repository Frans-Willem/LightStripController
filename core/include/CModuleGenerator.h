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
	void Generate(CColor *pColors);
	static CModuleGenerator *Create(std::string strModule, unsigned int nLength, libconfig::Setting &s);
};
#endif//CMODULEGENERATOR_H
