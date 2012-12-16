#include <IGenerator.h>
#include "CRainbowGenerator.h"
#include <libconfig.h++>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	double dRPM, dRep;
	if (!settings.lookupValue("rpm", dRPM)) dRPM = 30.0;
	if (!settings.lookupValue("repetitions", dRep)) dRep = 1.0;
	return new CRainbowGenerator(nLength, pScheduler, dRPM, dRep);
}