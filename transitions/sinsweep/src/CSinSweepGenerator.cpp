#include "CSinSweepGenerator.h"
#include "IFrameScheduler.h"
#include <cmath>

CSinSweepGenerator::CSinSweepGenerator(unsigned int nLength, IFrameScheduler *pScheduler, IGenerator *pFrom, IGenerator *pTo, double dDuration, double dSweepLen, bool bReverse) :
	CTransition(nLength,pFrom,pTo),
	m_pScheduler(pScheduler),
	m_dDuration(dDuration),
	m_dSweepLen(dSweepLen),
	m_bReverse(bReverse),
	m_timeStarted(CTime::Now())
{
}
CSinSweepGenerator::~CSinSweepGenerator() {

}
bool CSinSweepGenerator::Transition(CColor *pColors, CColor *pFrom) {
	double dProgress = (CTime::Now() - m_timeStarted).ToSeconds() / m_dDuration;
	if (dProgress >= 1.0) {
		//Already done, so don't bother mixing :)
		return true;
	}
	if (dProgress < 0.0) {
		dProgress = 0.0;
	}
	double dPi = std::atan(1.0) * 4.0;
	double dSweepLen = m_dSweepLen * (double)m_nLength;
	if (m_bReverse) {
		dProgress = 1.0 - dProgress;
	}
	double dSweepPos = (((double)m_nLength) * dProgress) - (dSweepLen * (1.0 - dProgress));
	for (unsigned int i = 0; i < m_nLength; i++) {
		double dPos = (double) i;
		dPos -= dSweepPos;
		dPos /= dSweepLen;
		if (dPos < 0.0) dPos = 0.0;
		else if (dPos > 1.0) dPos = 1.0;
		else {
			dPos = (std::cos(dPos * dPi) - 1.0)/-2.0;
		}
		if (m_bReverse) {
			dPos = 1.0 - dPos;
		}
		pColors[i] = (pColors[i] * (1.0 - dPos)) + (pFrom[i] * dPos);
	}
	if (m_pScheduler)
		m_pScheduler->ScheduleFrame();
	return false;
}