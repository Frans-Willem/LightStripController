#include <IGenerator.h>
#include "CParticleHighwayGenerator.h"
#include <CConfigObject.h>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, CConfigObject *s, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	double dMinCreateInterval = s->getDouble("mincreateinterval", 0.1),
		dMaxCreateInterval = s->getDouble("maxcreateinterval", 0.5);
	double dMinSpeed = s->getDouble("minspeed", 1.0/15.0),
		dMaxSpeed = s->getDouble("maxspeed", 1.0/3.0);
	double dMinLightness = s->getDouble("minlightness", 0.1),
		dMaxLightness = s->getDouble("maxlightness", 0.3);
	double dMinTail = s->getDouble("mintail", 1.0/70.0),
		dMaxTail = s->getDouble("maxtail", 10/7.0);
	int nNumColors = s->getInt("numcolors", 6);
	
	return new CParticleHighwayGenerator(nLength, pScheduler, dMinCreateInterval, dMaxCreateInterval, dMinSpeed, dMaxSpeed, dMinLightness, dMaxLightness, dMinTail, dMaxTail, nNumColors);
}