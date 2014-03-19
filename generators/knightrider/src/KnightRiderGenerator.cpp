#include <IGenerator.h>
#include "CKnightRiderGenerator.h"
#include <CConfigObject.h>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, CConfigObject *s, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	double dDuration = s->getDouble("duration", 4.0),
		dTrail = s->getDouble("trail", 0.2),
		dWidth = s->getDouble("width", 0.1);
	CConfigObject *sColor=s->get("color");
	CColor color;
	if (sColor->isNull()) color = CColor::RGB(1.0,0.0,0.0);
	else color = CColor::FromSetting(sColor);
	
	return new CKnightRiderGenerator(nLength, pScheduler, color, dDuration, dWidth, dTrail);
}