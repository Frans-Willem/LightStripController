#include "CSinFadeGenerator.h"
#include <cmath>
#include <IFrameScheduler.h>

CSinFadeGenerator::CSinFadeGenerator(unsigned int nLength, IFrameScheduler *pScheduler, IGenerator *pFrom, IGenerator *pTo, double dDuration) :
	m_nLength(nLength),
	m_pScheduler(pScheduler),
	m_pBuffer(new CColor[nLength]),
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
bool CSinFadeGenerator::Generate(CColor *pColors, IGenerator **ppNextGenerator) {
	IGenerator *pNewFrom = NULL;
	if (m_pTo) {
		IGenerator *pNew = NULL;
		if (m_pTo->Generate(pColors, &pNew)) {
			m_pTo->Release();
			m_pTo = pNew;
		}
	}
	if (m_pFrom) {
		IGenerator *pNew = NULL;
		if (m_pFrom->Generate(m_pBuffer, &pNew)) {
			m_pFrom->Release();
			m_pFrom = pNew;
		}
	}
	if (!Transition(pColors, m_pBuffer)) {
		//Not done, so schedule another frame ASAP.
		if (m_pScheduler)
			m_pScheduler->ScheduleFrame();
		return false;
	}
	//If we're already done, m_pTo should schedule the new frame
	if (m_pTo) {
		m_pTo->AddRef();
	}
	*ppNextGenerator = m_pTo;
	return true;
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
	return false;
}