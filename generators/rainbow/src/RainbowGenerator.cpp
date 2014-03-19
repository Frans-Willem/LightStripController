#include <IGenerator.h>
#include "CRainbowGenerator.h"
#include <CConfigObject.h>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, CConfigObject *s, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	double dRPM = s->getDouble("rpm", 30.0),
		dRep = s->getDouble("repetitions", 1.0);
	return new CRainbowGenerator(nLength, pScheduler, dRPM, dRep);
}