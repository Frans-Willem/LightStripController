#include "CSinSweepGenerator.h"
#include <CConfigObject.h>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, CConfigObject *s, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
//unsigned int nLength, IFrameScheduler *pScheduler, IGenerator *pFrom, IGenerator *pTo, double dDuration, double dSweepLen, bool bDirection
	IGenerator *pFrom=NULL;
	IGenerator *pTo=NULL;
	if (vArguments.size() > 0) pTo = vArguments[0];
	if (vArguments.size() > 1) pFrom = vArguments[1];
	double dDuration = s->getDouble("duration", 1.0);
	double dSweepLen = s->getDouble("sweep", 0.1);
	bool bDirection = s->getInt("reverse", 0) != 0;
	return new CSinSweepGenerator(nLength, pScheduler, pFrom, pTo, dDuration, dSweepLen, bDirection);
}