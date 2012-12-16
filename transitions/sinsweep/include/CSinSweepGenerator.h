#ifndef CSINSWEEPGENERATOR_H
#define CSINSWEEPGENERATOR_H
#include <CTransition.h>
#include <CTime.h>

class IFrameScheduler;
class CSinSweepGenerator : public CTransition {
private:
	IFrameScheduler *m_pScheduler;
	CTime m_timeStarted;
	double m_dDuration;
	double m_dSweepLen;
	bool m_bReverse;
public:
	CSinSweepGenerator(unsigned int nLength, IFrameScheduler *pScheduler, IGenerator *pFrom, IGenerator *pTo, double dDuration, double dSweepLen, bool bReverse);
	~CSinSweepGenerator();
	bool Transition(CColor *pColors, CColor *pFrom);
};
#endif//CSINSWEEPGENERATOR_H