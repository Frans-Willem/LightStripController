#include "CSinSweepGenerator.h"
#include <libconfig.h++>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
//unsigned int nLength, IFrameScheduler *pScheduler, IGenerator *pFrom, IGenerator *pTo, double dDuration, double dSweepLen, bool bDirection
	IGenerator *pFrom=NULL;
	IGenerator *pTo=NULL;
	if (vArguments.size() > 0) pTo = vArguments[0];
	if (vArguments.size() > 1) pFrom = vArguments[1];
	double dDuration;
	double dSweepLen;
	bool bDirection;
	if (!settings.lookupValue("duration", dDuration)) dDuration = 1.0;
	if (!settings.lookupValue("sweep", dSweepLen)) dSweepLen = 0.1;
	if (!settings.lookupValue("reverse", bDirection)) bDirection = false;
	return new CSinSweepGenerator(nLength, pScheduler, pFrom, pTo, dDuration, dSweepLen, bDirection);
}