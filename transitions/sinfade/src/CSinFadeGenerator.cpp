#include "CSinFadeGenerator.h"
#include <cmath>
#include <IFrameScheduler.h>

CSinFadeGenerator::CSinFadeGenerator(unsigned int nLength, IFrameScheduler *pScheduler, IGenerator *pFrom, IGenerator *pTo, double dDuration) :
	CTransition(nLength,pFrom,pTo),
	m_pScheduler(pScheduler),
	m_dDuration(dDuration),
	m_timeStarted(CTime::Now())
{

}
CSinFadeGenerator::~CSinFadeGenerator() {

}

bool CSinFadeGenerator::Transition(CColor *pColors, CColor *pFrom) {
	double dProgress = (CTime::Now() - m_timeStarted).ToSeconds() / m_dDuration;
	if (dProgress >= 1.0) {
		//Already done, so don't bother mixing :)
		return true;
	}
	if (dProgress < 0.0) {
		dProgress = 0.0;
	}
	double dPi = std::atan(1.0) * 4.0;
	double dFrom = (std::cos(dPi * dProgress) + 1.0)/2.0;
	double dTo = 1.0 - dFrom;
	for (unsigned int i = 0; i < m_nLength; i++)
		pColors[i] = (pColors[i] * dTo) + (pFrom[i] * dFrom);
	if (m_pScheduler)
		m_pScheduler->ScheduleFrame();
	return false;
}