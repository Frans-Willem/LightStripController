#include "CColorGenerator.h"


CColorGenerator::CColorGenerator(unsigned int nLength, IFrameScheduler *pScheduler, CColor color) :
	m_nLength(nLength),
	m_pScheduler(pScheduler),
	m_color(color)
{
}
CColorGenerator::~CColorGenerator() {

}
bool CColorGenerator::Generate(CColor *pColors, IGenerator **ppNextGenerator) {
	for (unsigned int i = 0; i < m_nLength; i++)
		pColors[i] = m_color;
	return false;
}