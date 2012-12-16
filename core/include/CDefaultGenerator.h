#ifndef CDEFAULTGENERATOR_H
#define CDEFAULTGENERATOR_H
#include <IGenerator.h>
#include <CTime.h>

class CDefaultGenerator : public IGenerator {
private:
	unsigned int m_nLength;
public:
	CDefaultGenerator(unsigned int nLength);
	~CDefaultGenerator();
	bool Generate(CColor *pColors, IGenerator **ppNextGenerator);
};
#endif//CDEFAULTGENERATOR_H
