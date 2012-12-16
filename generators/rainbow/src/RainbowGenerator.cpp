#include <IGenerator.h>
#include "CRainbowGenerator.h"
#include <libconfig.h++>

extern "C" IGenerator* CreateGenerator(std::vector<IGenerator*> lArguments, unsigned int nLength, libconfig::Setting &s) {
	double dRPM, dRep;
	if (!s.lookupValue("rpm", dRPM)) dRPM = 30.0;
	if (!s.lookupValue("repetitions", dRep)) dRep = 1.0;
	return new CRainbowGenerator(nLength, dRPM, dRep);
}