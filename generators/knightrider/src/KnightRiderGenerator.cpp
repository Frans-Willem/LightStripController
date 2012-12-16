#include <IGenerator.h>
#include "CKnightRiderGenerator.h"
#include <libconfig.h++>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, libconfig::Setting &settings, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	double dDuration, dTrail, dWidth;
	if (!settings.lookupValue("duration", dDuration)) dDuration = 4.0;
	if (!settings.lookupValue("trail", dTrail)) dTrail = 0.2;
	if (!settings.lookupValue("width", dWidth)) dWidth = 0.1;
	CColor color;
	if (!settings.exists("color")) color = CColor::RGB(1.0,0.0,0.0);
	else color = CColor::FromSetting(settings["color"]);
	
	return new CKnightRiderGenerator(nLength, pScheduler, color, dDuration, dWidth, dTrail);
}