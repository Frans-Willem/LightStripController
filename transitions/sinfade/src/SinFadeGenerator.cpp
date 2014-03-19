#include "CSinFadeGenerator.h"
#include "CConfigObject.h"

extern "C" IGenerator* CreateGenerator(unsigned int nLength, CConfigObject *s, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	IGenerator *pFrom=NULL;
	IGenerator *pTo=NULL;
	if (vArguments.size() > 0) pTo = vArguments[0];
	if (vArguments.size() > 1) pFrom = vArguments[1];
	double dDuration = s->getDouble("duration", 1.0);
	return new CSinFadeGenerator(nLength, pScheduler, pFrom, pTo, dDuration);
}