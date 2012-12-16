#include "CSinFadeGenerator.h"
#include <libconfig.h++>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	IGenerator *pFrom=NULL;
	IGenerator *pTo=NULL;
	if (vArguments.size() > 0) pTo = vArguments[0];
	if (vArguments.size() > 1) pFrom = vArguments[1];
	double dDuration;
	if (!settings.lookupValue("duration", dDuration)) dDuration = 1.0;
	return new CSinFadeGenerator(nLength, pScheduler, pFrom, pTo, dDuration);
}