#ifndef CSTRIPTHREAD_H
#define CSTRIPTHREAD_H
#include "CThread.h"
#include "CSizedQueue.h"
#include <vector>
#include "CEvent.h"
#include <CTime.h>

struct LightStripConfig;
class IGenerator;
class CColor;
class IStripCommand;
class CStripThread : public CThread {
private:
	LightStripConfig *m_pConfig;
	CSizedQueue<std::vector<unsigned char> *> m_qOutput;
	struct Command {
		IStripCommand *pCmd;
		bool bDone;
	};
	CSizedQueue<Command *> m_qCommands;
	CEvent *m_pEventOutput; //Event to be triggered on new output
	CEvent m_eventWake; //Event to be triggered to wake up thread
	CEvent m_eventCommand; //Event to be triggered after popping commands and after executing commands
	CTime m_timeNextFrame;
	bool m_bStop;
	IGenerator *m_pGenerator;
	CColor *m_pOutput;
	
	std::vector<unsigned char> *Generate();
	static double lum2duty(double lum);
	static unsigned char channel2byte(double color);
	
public:
	CStripThread(LightStripConfig *pConfig, CEvent *pEventOutput);
	~CStripThread();
	virtual void Stop();
	void* Run();
	std::vector<unsigned char> *GetFrame(bool &bNeedWait, CTime &timeNextFrame);
	void OnFrameSent();
	void ExecuteCommand(IStripCommand *pCmd);
};
#endif//CSTRIPTHREAD_H
