#include <IGenerator.h>
#include "CColorGenerator.h"
#include <libconfig.h++>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	CColor color = CColor::FromSetting(settings);
	return new CColorGenerator(nLength, pScheduler, color);
}