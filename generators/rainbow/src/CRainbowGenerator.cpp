#include "CRainbowGenerator.h"
#include <CTime.h>

CRainbowGenerator::CRainbowGenerator(unsigned int nLength, double dRPM, double dRep) :
	m_nLength(nLength),
	m_dRPM(dRPM),
	m_dRep(dRep),
	m_timeStarted(CTime::Now())
{
}
CRainbowGenerator::~CRainbowGenerator() {
	//Don't really do anything
}
void CRainbowGenerator::Generate(CColor *pColors) {
	CTimeSpan timespanElapsed = CTime::Now() - m_timeStarted;
	double dElapsed = timespanElapsed.ToSeconds();
	double dStart = (dElapsed / 60.0) * m_dRPM;
	double dLength = m_nLength;
	for (unsigned int i = 0; i < m_nLength; i++) {
		double dPosition = (((double)i) / dLength) * m_dRep;
		pColors[i] = CColor::HSL(dStart + dPosition, 1.0, 0.5);
	}
}