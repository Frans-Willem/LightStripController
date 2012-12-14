#include "COutputThread.h"
#include <CEvent.h>
#include <CTime.h>
#include <vector>
#include "CStripThread.h"
#include <IOutput.h>

COutputThread::COutputThread(IOutput *pOutput, std::list<CStripThread *> lStrips, CEvent *pEventWakeup) :
	m_pOutput(pOutput),
	m_lStrips(lStrips),
	m_pEventWakeup(pEventWakeup),
	m_bDone(false)
{

}
COutputThread::~COutputThread() {
	
}
void COutputThread::Stop() {
	m_bDone = true;
	m_pEventWakeup->Set();
	CThread::Stop();
}
void* COutputThread::Run() {
	while (!m_bDone) {
		bool bDidSomething = false;
		bool bWaitForTime = false;
		CTime timeWaitForTime;
		
		for (std::list<CStripThread *>::iterator i = m_lStrips.begin(); i != m_lStrips.end(); i++) {
			CStripThread *pStrip = *i;
			bool bStripNeedsWait = false;
			CTime timeStripWait;
			std::vector<unsigned char> *pOutputData = pStrip->GetFrame(bStripNeedsWait, timeStripWait);
			if (pOutputData) {
				m_pOutput->Write((void *)&pOutputData->front(), pOutputData->size());
				pStrip->OnFrameSent();
				delete pOutputData;
				m_lStrips.erase(i);
				m_lStrips.push_back(pStrip);
				bDidSomething = true;
				break;
			} else {
				if (bStripNeedsWait) {
					if (!bWaitForTime || timeStripWait < timeWaitForTime) {
						timeWaitForTime = timeStripWait;
					}
					bWaitForTime = true;
				}
			}
		}
		
		if (!bDidSomething) {
			if (bWaitForTime) {
				m_pEventWakeup->Wait(timeWaitForTime.ToTimeSpec());
			} else {
				m_pEventWakeup->Wait();
			}
		}
	}
	return NULL;
}