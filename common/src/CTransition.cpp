#include "CTransition.h"
#include "CColor.h"
#include "IGenerator.h"
#include <stddef.h>

CTransition::CTransition(unsigned int nLength, IGenerator *pFrom, IGenerator *pTo) :
	m_nLength(nLength),
	m_pBuffer(new CColor[nLength]),
	m_pFrom(pFrom),
	m_pTo(pTo)
{
	if (m_pFrom) m_pFrom->AddRef();
	if (m_pTo) m_pTo->AddRef();
	CColor black = CColor::RGB(0.0,0.0,0.0);
	for (unsigned int i = 0; i<m_nLength; i++)
		m_pBuffer[i] = black;
}
CTransition::~CTransition() {
	if (m_pFrom) m_pFrom->Release();
	if (m_pTo) m_pTo->Release();
	delete[] m_pBuffer;
}
bool CTransition::Generate(CColor *pColors, IGenerator **ppNextGenerator) {
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
	if (!Transition(pColors, m_pBuffer))
		return false;
	if (m_pTo) {
		m_pTo->AddRef();
	}
	*ppNextGenerator = m_pTo;
	return true;
}