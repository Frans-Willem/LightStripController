#ifndef COUTPUTTHREAD_H
#define COUTPUTTHREAD_H
#include "CThread.h"
#include <list>

class CStripThread;
class CEvent;
class IOutput;
class COutputThread : public CThread {
private:
	std::list<CStripThread *> m_lStrips;
	CEvent *m_pEventWakeup;
	IOutput *m_pOutput;
	bool m_bDone;
public:
	COutputThread(IOutput *pOutput, std::list<CStripThread *> lStrips, CEvent *pEventWakeup);
	~COutputThread();
	virtual void Stop();
	void* Run();
};
#endif//COUTPUTTHREAD_H
