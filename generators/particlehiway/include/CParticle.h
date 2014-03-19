#ifndef CPARTICLE_H
#define CPARTICLE_H
#include <CTime.h>

class CColor;
class CParticle {
private:
	CTime m_timeStarted;
	double m_dSpeed;
	double m_dLightness;
	double m_dTail;
	double m_dHue;
	bool m_bDirection;
	
	double getMoved(CTime time);
public:
	CParticle(CTime timeStart, double dSpeed, double dLightness, double dTail, double dHue, bool bDirection);
	CColor render(CTime time, double dPos);
	bool IsActive(CTime time);
};
#endif//CPARTICLE_H
