#ifndef CKNIGHTRIDERGENERATOR_H
#define CKNIGHTRIDERGENERATOR_H
#include <IGenerator.h>
#include <CTime.h>

class IFrameScheduler;
class CKnightRiderGenerator : public IGenerator {
private:
	unsigned int m_nLength;
	IFrameScheduler *m_pScheduler;
	CColor m_color;
	double m_dDuration;
	double m_dTrail;
	double m_dWidth;
	CTime m_timeStarted;
public:
	CKnightRiderGenerator(unsigned int nLength, IFrameScheduler *pScheduler, CColor color, double dDuration, double dWidth, double dTrail);
	~CKnightRiderGenerator();
	bool Generate(CColor *pColors, IGenerator **ppNextGenerator);
};
#endif//CKNIGHTRIDERGENERATOR_H
