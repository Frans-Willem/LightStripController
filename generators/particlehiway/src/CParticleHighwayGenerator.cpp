#include "CParticleHighwayGenerator.h"
#include <CTime.h>
#include <IFrameScheduler.h>
#include <cmath>
#include <stdlib.h>

CParticleHighwayGenerator::CParticleHighwayGenerator(unsigned int nLength, IFrameScheduler *pScheduler, double dMinCreateInterval, double dMaxCreateInterval, double dMinSpeed, double dMaxSpeed, double dMinLightness, double dMaxLightness, double dMinTail, double dMaxTail, int nNumColors) :
	m_nLength(nLength), m_pScheduler(pScheduler),
	m_dMinCreateInterval(dMinCreateInterval), m_dMaxCreateInterval(dMaxCreateInterval),
	m_dMinSpeed(dMinSpeed), m_dMaxSpeed(dMaxSpeed),
	m_dMinLightness(dMinLightness), m_dMaxLightness(dMaxLightness),
	m_dMinTail(dMinTail), m_dMaxTail(dMaxTail),
	m_nNumColors(nNumColors),
	m_timeNextParticle(CTime::Now())
{
}
CParticleHighwayGenerator::~CParticleHighwayGenerator() {
	
}
double CParticleHighwayGenerator::getRandomInbetween(double min, double max) {
	int r = rand();
	double dr = (double)r / (double)RAND_MAX;
	double diff = max - min;
	return min + (diff * dr);
}
int CParticleHighwayGenerator::getRandomInteger(int min, int max) {
	int diff = max-min;
	int r = rand() % diff;
	return min + r;
}

bool CParticleHighwayGenerator::Generate(CColor *pColors, IGenerator **ppNextGenerator) {
	CTime now = CTime::Now();
	while (m_timeNextParticle < now) {
		m_vParticles.push_back(
			CParticle(
				m_timeNextParticle,
				getRandomInbetween(m_dMinSpeed, m_dMaxSpeed), //Speed
				getRandomInbetween(m_dMinLightness, m_dMaxLightness), //Lightness
				getRandomInbetween(m_dMinTail, m_dMaxTail), //Tail
				(double)getRandomInteger(0, m_nNumColors) / (double)m_nNumColors, //Color (Hue)
				(bool)(rand() % 2) //Direction
			)
		);
		m_timeNextParticle += CTimeSpan::FromSeconds(getRandomInbetween(m_dMinCreateInterval, m_dMaxCreateInterval));
	}
	//Throw away inactive particles first
	for (std::vector<CParticle>::iterator p = m_vParticles.begin(); p != m_vParticles.end(); ) {
		if (p->IsActive(now))
			p++;
		else
			p = m_vParticles.erase(p);
	}
	for (int i=0; i< m_nLength; i++) {
		pColors[i] = CColor::RGB(0.0,0.0,0.0);
		double dPos = (double)i / (double)m_nLength;
		for (std::vector<CParticle>::iterator p = m_vParticles.begin(); p != m_vParticles.end(); p++) {
			pColors[i] = pColors[i] + p->render(now, dPos);
		}
	}
	if (m_pScheduler)
		m_pScheduler->ScheduleFrame();
	return false;
}