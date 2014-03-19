#ifndef CCONTROLTHREAD_H
#define CCONTROLTHREAD_H
#include "CThread.h"
#include <list>
#include <set>
#include <map>
#include <string>


#ifndef SOCKET
#define SOCKET int
#endif//SOCKET


class CStripThread;
class CConfigObject;
class CEvent;
class IOutput;
class CControlSocket;
class CControlThread : public CThread {
private:
	bool m_bDone;
	SOCKET m_sock;
	CConfigObject *m_pConfig;
	std::list<CStripThread*> m_lStrips;
protected:
	friend class CControlSocket;
	bool Change(std::set<int> lStrips, std::string strTransition, std::string strPreset);
	std::set<std::string> GetPresets();
	std::set<std::string> GetTransitions();
	std::map<int, std::string> GetStrips();
public:
	CControlThread(std::list<CStripThread*> lStrips, CConfigObject *pConfig);
	~CControlThread();
	virtual void Stop();
	void* Run();
};
#endif//CCONTROLTHREAD_H
