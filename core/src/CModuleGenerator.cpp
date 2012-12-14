#include "CModuleGenerator.h"
#include <dlfcn.h>
#include <libconfig.h++>

CModuleGenerator::CModuleGenerator(void *pModule, IGenerator *pGenerator) :
	m_pModule(pModule),
	m_pGenerator(pGenerator)
{

}
CModuleGenerator::~CModuleGenerator() {
	m_pGenerator->Release();
	dlclose(m_pModule);
}
void CModuleGenerator::Generate(CColor *pColors) {
	return m_pGenerator->Generate(pColors);
}
CModuleGenerator *CModuleGenerator::Create(std::string strModule, unsigned int nLength, libconfig::Setting &s) {
	void *pModule = dlopen(strModule.c_str(), RTLD_NOW);
	if (!pModule) {
		return NULL;
	}
	CreateGeneratorPtr pCreate = (CreateGeneratorPtr)dlsym(pModule, "CreateGenerator");
	if (!pCreate) {
		dlclose(pModule);
		return NULL;
	}
	IGenerator *pGenerator = pCreate(nLength, s);
	if (!pGenerator) {
		dlclose(pModule);
		return NULL;
	}
	return new CModuleGenerator(pModule, pGenerator);
}