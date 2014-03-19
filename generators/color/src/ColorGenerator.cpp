#include <IGenerator.h>
#include "CColorGenerator.h"
#include <CConfigObject.h>

extern "C" IGenerator* CreateGenerator(unsigned int nLength, CConfigObject *s, IFrameScheduler *pScheduler, std::vector<IGenerator*> vArguments) {
	CColor color = CColor::FromSetting(s);
	return new CColorGenerator(nLength, pScheduler, color);
}