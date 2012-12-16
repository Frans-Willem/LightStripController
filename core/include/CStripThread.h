#ifndef CSTRIPTHREAD_H
#define CSTRIPTHREAD_H
#include "CThread.h"
#include "CSizedQueue.h"
#include <vector>
#include "CEvent.h"
#include <CTime.h>
#include "IFrameScheduler.h"

struct LightStripConfig;
class IGenerator;
class CColor;
class IStripCommand;
class CStripThread : public CThread, public IFrameScheduler {
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
	CTime m_timeNextOutput;
	bool m_bStop;
	IGenerator *m_pGenerator;
	CColor *m_pOutput;
	bool m_bFrameScheduledASAP;
	CTime m_timeNextFrameScheduled;
	
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
	LightStripConfig *GetConfig();
//IFrameScheduler
	void ScheduleFrame();
	void ScheduleFrame(CTime &timeNextFrame);
};
#endif//CSTRIPTHREAD_H
