#include "CKnightRiderGenerator.h"
#include <CTime.h>
#include <IFrameScheduler.h>
#include <cmath>

CKnightRiderGenerator::CKnightRiderGenerator(unsigned int nLength, IFrameScheduler *pScheduler, CColor color, double dDuration, double dWidth, double dTrail) :
	m_nLength(nLength),
	m_pScheduler(pScheduler),
	m_color(color),
	m_dDuration(dDuration),
	m_dWidth(dWidth),
	m_dTrail(dTrail),
	m_timeStarted(CTime::Now())
{
}
CKnightRiderGenerator::~CKnightRiderGenerator() {
	//Don't really do anything
}
bool CKnightRiderGenerator::Generate(CColor *pColors, IGenerator **ppNextGenerator) {
	CTimeSpan timespanElapsed = CTime::Now() - m_timeStarted;
	double dElapsed = timespanElapsed.ToSeconds();
	double dPosition = std::fmod(dElapsed / m_dDuration, 1.0) * 2.0;
	double dPi = std::atan(1.0) * 4.0;
	/*
	//Optional speedup in middle, not sure if this looks good.
	//The trial will be looking off-ish with this
	if (dPosition < 1.0) {
		dPosition = (std::cos(dPosition * dPi) - 1.0) / -2.0;
	} else {
		dPosition = ((std::cos((dPosition - 1.0) * dPi) - 1.0) / -2.0) + 1.0;
	}
	*/
	double dLength = (double)(m_nLength - 1);
	for (unsigned int i = 0; i < m_nLength; i++) {
		double dCur = ((double)i) / dLength;
		double dDist1 = std::fmod((dPosition + 2.0) - dCur, 2.0);
		double dDist2 = std::fmod((dPosition + 2.0) - (2.0 - dCur), 2.0);
		double dDist = std::min(dDist1,dDist2);
		if (dDist < m_dWidth) {
			pColors[i] = m_color;
		} else {
			dDist -= m_dWidth;
			dDist /= m_dTrail;
			dDist = 1.0 - dDist;
			if (dDist < 0.0)
				dDist = 0.0;
			pColors[i] = m_color * dDist;
		}
	}
	if (m_pScheduler)
		m_pScheduler->ScheduleFrame();
	return false;
}