#ifndef CCOLORGENERATOR_H
#define CCOLORGENERATOR_H
#include <IGenerator.h>

class IFrameScheduler;
class CColorGenerator : public IGenerator {
private:
	unsigned int m_nLength;
	IFrameScheduler *m_pScheduler;
	CColor m_color;
public:
	CColorGenerator(unsigned int nLength, IFrameScheduler *pScheduler, CColor color);
	~CColorGenerator();
	bool Generate(CColor *pColors, IGenerator **ppNextGenerator);
};
#endif//CCOLORGENERATOR_H
