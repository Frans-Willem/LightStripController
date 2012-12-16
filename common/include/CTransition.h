#ifndef CTRANSITION_H
#define CTRANSITION_H
#include "IGenerator.h"
class CColor;
class CTransition : public IGenerator {
private:
	CColor *m_pBuffer;
	IGenerator *m_pFrom;
	IGenerator *m_pTo;
protected:
	unsigned int m_nLength;
	virtual bool Transition(CColor *pColors, CColor *pFrom) = 0;
public:
	CTransition(unsigned int nLength, IGenerator *pFrom, IGenerator *pTo);
	~CTransition();
	bool Generate(CColor *pColors, IGenerator **ppNextGenerator);
};
#endif//CTRANSITION_H
