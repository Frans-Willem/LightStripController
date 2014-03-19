#include "CParticle.h"
#include "CColor.h"

double CParticle::getMoved(CTime time) {
	double elapsed = (time - m_timeStarted).ToSeconds();
	return elapsed * m_dSpeed;
}

CParticle::CParticle(CTime timeStart, double dSpeed, double dLightness, double dTail, double dHue, bool bDirection) :
	m_timeStarted(timeStart),
	m_dSpeed(dSpeed),
	m_dLightness(dLightness),
	m_dTail(dTail),
	m_dHue(dHue),
	m_bDirection(bDirection)
{
}
CColor CParticle::render(CTime time, double dPos) {
	double moved = getMoved(time);
	if (m_bDirection)
		dPos = 1.0 - dPos;
	double relative = (moved - dPos)/m_dTail;
	if (relative < 0.0 || relative > 1.0)
		return CColor::RGB(0.0,0.0,0.0);
	double lightness = (1.0 - relative) * m_dLightness;
	return CColor::HSL(m_dHue, 1.0, lightness);
}
bool CParticle::IsActive(CTime time) {
	double moved = getMoved(time);
	double tail = moved - m_dTail;
	return tail <= 1.0;
}