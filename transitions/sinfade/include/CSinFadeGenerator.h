#ifndef CSINFADEGENERATOR_H
#define CSINFADEGENERATOR_H
#include <IGenerator.h>
#include <CTime.h>

class CSinFadeGenerator : public IGenerator {
private:
	unsigned int m_nLength;
	CTime m_timeStarted;
	IGenerator *m_pFrom;
	IGenerator *m_pTo;
	double m_dDuration;
	CColor *m_pBuffer;
public:
	CSinFadeGenerator(unsigned int nLength, IGenerator *pFrom, IGenerator *pTo, double dDuration);
	~CSinFadeGenerator();
	void Generate(CColor *pColors, CTime &timeNextFrame, IGenerator **ppNextGenerator);
	bool Transition(CColor *pColors, CColor *pFrom, CTime timeNextToFrame, CTime timeNextFromFrame, CTime &timeNextFrame);
};
#endif//CSINFADEGENERATOR_H
