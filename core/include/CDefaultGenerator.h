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
	void Generate(CColor *pColors);
};
#endif//CDEFAULTGENERATOR_H
