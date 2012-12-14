#ifndef CRAINBOWGENERATOR_H
#define CRAINBOWGENERATOR_H
#include <IGenerator.h>
#include <CTime.h>

class CRainbowGenerator : public IGenerator {
private:
	unsigned int m_nLength;
	double m_dRPM;
	double m_dRep;
	CTime m_timeStarted;
public:
	CRainbowGenerator(unsigned int nLength, double dRPM, double dRep);
	~CRainbowGenerator();
	void Generate(CColor *pColors);
};
#endif//CRAINBOWGENERATOR_H
