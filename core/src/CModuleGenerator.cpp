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
bool CModuleGenerator::Generate(CColor *pColors, IGenerator **ppNextGenerator) {
	return m_pGenerator->Generate(pColors, ppNextGenerator);
}
CModuleGenerator *CModuleGenerator::Create(std::string strModule, unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	void *pModule = dlopen(strModule.c_str(), RTLD_NOW);
	if (!pModule) {
		return NULL;
	}
	CreateGeneratorPtr pCreate = (CreateGeneratorPtr)dlsym(pModule, "CreateGenerator");
	if (!pCreate) {
		dlclose(pModule);
		return NULL;
	}
	IGenerator *pGenerator = pCreate(nLength, settings, pScheduler, vArguments);
	if (!pGenerator) {
		dlclose(pModule);
		return NULL;
	}
	return new CModuleGenerator(pModule, pGenerator);
}