#include "CSinFadeGenerator.h"
#include <cmath>

CSinFadeGenerator::CSinFadeGenerator(unsigned int nLength, IGenerator *pFrom, IGenerator *pTo, double dDuration) :
	m_pBuffer(new CColor[nLength]),
	m_nLength(nLength),
	m_pFrom(pFrom),
	m_pTo(pTo),
	m_dDuration(dDuration),
	m_timeStarted(CTime::Now())
{
	if (m_pFrom) m_pFrom->AddRef();
	if (m_pTo) m_pTo->AddRef();
	CColor black = CColor::RGB(0.0,0.0,0.0);
	for (unsigned int i = 0; i<m_nLength; i++)
		m_pBuffer[i] = black;
}
CSinFadeGenerator::~CSinFadeGenerator() {
	if (m_pFrom) m_pFrom->Release();
	if (m_pTo) m_pTo->Release();
	delete[] m_pBuffer;
}
void CSinFadeGenerator::Generate(CColor *pColors, CTime &timeNextFrame, IGenerator **ppNextGenerator) {
	CTime timeNextToFrame = CTime::Now();
	CTime timeNextFromFrame = CTime::Now();
	IGenerator *pNewTo = NULL;
	IGenerator *pNewFrom = NULL;
	if (m_pTo) m_pTo->Generate(pColors, timeNextToFrame, &pNewTo);
	if (m_pFrom) m_pFrom->Generate(m_pBuffer, timeNextFromFrame, &pNewFrom);
	if (pNewTo) {
		if (m_pTo) m_pTo->Release();
		m_pTo = pNewTo;
	}
	if (pNewFrom) {
		if (m_pFrom) m_pFrom->Release();
		m_pFrom = pNewFrom;
	}
	bool bDone = Transition(pColors, m_pBuffer, timeNextToFrame, timeNextFromFrame, timeNextFrame);
	if (bDone && m_pTo) {
		m_pTo->AddRef();
		*ppNextGenerator = m_pTo;
	}
}

bool CSinFadeGenerator::Transition(CColor *pColors, CColor *pFrom, CTime timeNextToFrame, CTime timeNextFromFrame, CTime &timeNextFrame) {
	double dProgress = (CTime::Now() - m_timeStarted).ToSeconds() / m_dDuration;
	bool bDone = false;
	if (dProgress >= 1.0) {
		dProgress = 1.0;
		bDone = true;
	} else if (dProgress < 0.0) {
		dProgress = 0.0;
	}
	double dPi = std::atan(1.0) * 4.0;
	double dFrom = (std::cos(dPi * dProgress) + 1.0)/2.0;
	double dTo = 1.0 - dFrom;
	for (unsigned int i = 0; i < m_nLength; i++)
		pColors[i] = (pColors[i] * dTo) + (pFrom[i] * dFrom);
	return bDone;
}