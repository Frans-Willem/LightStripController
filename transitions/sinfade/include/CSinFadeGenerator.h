#ifndef CSINFADEGENERATOR_H
#define CSINFADEGENERATOR_H
#include <IGenerator.h>
#include <CTime.h>

class IFrameScheduler;
class CSinFadeGenerator : public IGenerator {
private:
	unsigned int m_nLength;
	IFrameScheduler *m_pScheduler;
	CTime m_timeStarted;
	IGenerator *m_pFrom;
	IGenerator *m_pTo;
	double m_dDuration;
	CColor *m_pBuffer;
public:
	CSinFadeGenerator(unsigned int nLength, IFrameScheduler *pScheduler, IGenerator *pFrom, IGenerator *pTo, double dDuration);
	~CSinFadeGenerator();
	bool Generate(CColor *pColors, IGenerator **ppNextGenerator);
	bool Transition(CColor *pColors, CColor *pFrom);
};
#endif//CSINFADEGENERATOR_H
