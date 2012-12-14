#include "CDefaultGenerator.h"
#include <math.h>

CDefaultGenerator::CDefaultGenerator(unsigned int nLength) : m_nLength(nLength) {
	
}

CDefaultGenerator::~CDefaultGenerator() {

}
void CDefaultGenerator::Generate(CColor *pColors) {
	CColor black = CColor::RGB(0.0,0.0,0.0);
	for (unsigned int i=0; i<m_nLength; i++)
		pColors[i] = black;
}