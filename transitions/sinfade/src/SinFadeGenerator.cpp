#include "CSinFadeGenerator.h"
#include <libconfig.h++>

extern "C" IGenerator* CreateGenerator(std::vector<IGenerator*> vArguments, unsigned int nLength, libconfig::Setting &s) {
	IGenerator *pFrom=NULL;
	IGenerator *pTo=NULL;
	if (vArguments.size() > 0) pTo = vArguments[0];
	if (vArguments.size() > 1) pFrom = vArguments[1];
	double dDuration;
	if (!s.lookupValue("duration", dDuration)) dDuration = 1.0;
	return new CSinFadeGenerator(nLength, pFrom, pTo, dDuration);
}