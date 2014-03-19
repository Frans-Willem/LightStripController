#ifndef CPARTICLEHIGHWAYGENERATOR_H
#define CPARTICLEHIGHWAYGENERATOR_H
#include <IGenerator.h>
#include <CTime.h>
#include <vector>
#include "CParticle.h"

class IFrameScheduler;
class CParticleHighwayGenerator : public IGenerator {
private:
	unsigned int m_nLength;
	IFrameScheduler *m_pScheduler;
	CTime m_timeNextParticle;
	double m_dMinCreateInterval, m_dMaxCreateInterval;
	double m_dMinSpeed, m_dMaxSpeed;
	double m_dMinLightness, m_dMaxLightness;
	double m_dMinTail, m_dMaxTail;
	int m_nNumColors;
	std::vector<CParticle> m_vParticles;
	
	double getRandomInbetween(double min, double max);
	int getRandomInteger(int min, int max);
public:
	CParticleHighwayGenerator(unsigned int nLength, IFrameScheduler *pScheduler, double dMinCreateInterval, double dMaxCreateInterval, double dMinSpeed, double dMaxSpeed, double dMinLightness, double dMaxLightness, double dMinTail, double dMaxTail, int nNumColors);
	~CParticleHighwayGenerator();
	bool Generate(CColor *pColors, IGenerator **ppNextGenerator);
};
#endif//CPARTICLEHIGHWAYGENERATOR_H
