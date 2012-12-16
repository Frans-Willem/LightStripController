#ifndef CSINFADEGENERATOR_H
#define CSINFADEGENERATOR_H
#include <IGenerator.h>
#include <CTime.h>
#include "CTransition.h"

class IFrameScheduler;
class CSinFadeGenerator : public CTransition {
private:
	IFrameScheduler *m_pScheduler;
	CTime m_timeStarted;
	double m_dDuration;
public:
	CSinFadeGenerator(unsigned int nLength, IFrameScheduler *pScheduler, IGenerator *pFrom, IGenerator *pTo, double dDuration);
	~CSinFadeGenerator();
	bool Transition(CColor *pColors, CColor *pFrom);
};
#endif//CSINFADEGENERATOR_H
