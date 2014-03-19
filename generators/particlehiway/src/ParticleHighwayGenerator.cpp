#include <IGenerator.h>
#include "CParticleHighwayGenerator.h"
#include <libconfig.h++>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	double dMinCreateInterval, dMaxCreateInterval;
	double dMinSpeed, dMaxSpeed;
	double dMinLightness, dMaxLightness;
	double dMinTail, dMaxTail;
	int nNumColors;
	if (!settings.lookupValue("mincreateinterval", dMinCreateInterval)) dMinCreateInterval = 0.1;
	if (!settings.lookupValue("maxcreateinterval", dMaxCreateInterval)) dMaxCreateInterval = 0.5;
	if (!settings.lookupValue("minspeed", dMinSpeed)) dMinSpeed = 1.0/15.0;
	if (!settings.lookupValue("maxspeed", dMaxSpeed)) dMaxSpeed = 1.0/3.0;
	if (!settings.lookupValue("minlightness", dMinLightness)) dMinLightness = 0.1;
	if (!settings.lookupValue("maxlightness", dMaxLightness)) dMaxLightness = 0.3;
	if (!settings.lookupValue("mintail", dMinTail)) dMinTail = 1.0/70.0;
	if (!settings.lookupValue("maxtail", dMaxTail)) dMaxTail = 1.0/7.0;
	if (!settings.lookupValue("numcolors", nNumColors)) nNumColors = 6;
	
	return new CParticleHighwayGenerator(nLength, pScheduler, dMinCreateInterval, dMaxCreateInterval, dMinSpeed, dMaxSpeed, dMinLightness, dMaxLightness, dMinTail, dMaxTail, nNumColors);
}