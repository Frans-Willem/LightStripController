#ifndef CRAINBOWGENERATOR_H
#define CRAINBOWGENERATOR_H
#include <IGenerator.h>
#include <CTime.h>

class IFrameScheduler;
class CRainbowGenerator : public IGenerator {
private:
	unsigned int m_nLength;
	IFrameScheduler *m_pScheduler;
	double m_dRPM;
	double m_dRep;
	CTime m_timeStarted;
public:
	CRainbowGenerator(unsigned int nLength, IFrameScheduler *pScheduler, double dRPM, double dRep);
	~CRainbowGenerator();
	bool Generate(CColor *pColors, IGenerator **ppNextGenerator);
};
#endif//CRAINBOWGENERATOR_H
