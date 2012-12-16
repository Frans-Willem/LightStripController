#include "CStripThread.h"
#include <stdio.h>
#include <math.h>
#include <IGenerator.h>
#include "CDefaultGenerator.h"
#include "main.h"
#include "IStripCommand.h"

CStripThread::CStripThread(LightStripConfig *pConfig, CEvent *pEventOutput) :
	m_pConfig(pConfig),
	m_qOutput(1),
	m_qCommands(10),
	m_pEventOutput(pEventOutput),
	m_timeNextOutput(CTime::Now()),
	m_bStop(false),
	m_pGenerator(NULL),
	m_pOutput(NULL),
	m_bFrameScheduledASAP(true),
	m_timeNextFrameScheduled(CTime::Now())
{

}
CStripThread::~CStripThread() {
	while (m_qOutput.GetSize())
		delete m_qOutput.Get();
}
void CStripThread::Stop() {
	m_bStop=true;
	m_eventWake.Set();
	CThread::Stop();
}
double CStripThread::lum2duty(double lum) {
	if (lum>0.07999591993063804) {
		return pow(((lum+0.16)/1.16),3.0);
	} else {
		return lum/9.033;
	}
}
unsigned char CStripThread::channel2byte(double color) {
	double duty = lum2duty(color);
	duty *= 256.0;
	if (duty < 0.0) return 0;
	if (duty >= 255.0) return 255;
	return (int)duty;
}
std::vector<unsigned char> *CStripThread::Generate() {
	std::vector<unsigned char> *pRetval = new std::vector<unsigned char>();
	if (m_pGenerator) {
		IGenerator *pNewGenerator = NULL;
		if (m_pGenerator->Generate(m_pOutput, &pNewGenerator)) {
			m_pGenerator->Release();
			m_pGenerator = pNewGenerator;
			if (m_pGenerator)
				m_pGenerator->AddRef();
		}
	} else {
		CColor black = CColor::RGB(0.0,0.0,0.0);
		for (unsigned int i = 0; i < m_pConfig->nLengthDisplay; i++)
			m_pOutput[i] = black;
	}
	pRetval->push_back((UART_PACKET_RECIPIENT << 6) | m_pConfig->nId);
	for (int i=0; i<m_pConfig->nLengthTotal; i++) {
		unsigned char r = channel2byte(m_pOutput[i].r);
		unsigned char g = channel2byte(m_pOutput[i].g);
		unsigned char b = channel2byte(m_pOutput[i].b);
		pRetval->push_back((UART_PACKET_DATA << 6) | (b >> 2));
		pRetval->push_back((UART_PACKET_DATA << 6) | ((b << 4) & 0x3F) | (g >> 4));
		pRetval->push_back((UART_PACKET_DATA << 6) | ((g << 2) & 0x3F) | (r >> 6));
		pRetval->push_back((UART_PACKET_DATA << 6) | (r & 0x3F));
	}
	return pRetval;
}
void* CStripThread::Run() {
	printf("CStripThread::Run %d\n", m_pConfig->nId);
	//Init
	m_pOutput = new CColor[m_pConfig->nLengthTotal];
	for (int i=0; i<m_pConfig->nLengthTotal; i++) {
		m_pOutput[i] = CColor::RGB(0.0,0.0,0.0);
	}
	m_pGenerator = new CDefaultGenerator(m_pConfig->nLengthDisplay);
	std::vector<unsigned char> *pWaitingOutput = Generate();
	while (!m_bStop) {
		if (m_qCommands.GetSize() > 0) {
			Command *pCmd = m_qCommands.Get();
			m_eventCommand.Set();
			pCmd->pCmd->Execute(this, &m_pGenerator);
			pCmd->bDone = true;
			m_eventCommand.Set();
		} else if (pWaitingOutput == NULL && (m_bFrameScheduledASAP || CTime::Now() > m_timeNextFrameScheduled)) {
			m_bFrameScheduledASAP = false;
			m_timeNextFrameScheduled = CTime::Now() + CTimeSpan::FromSeconds(2);
			pWaitingOutput = Generate();
		} else if (m_qOutput.GetAvailable() > 0 && pWaitingOutput != NULL) {
			//Push out
			m_qOutput.Put(pWaitingOutput);
			m_pEventOutput->Set();
			//Generate new data
			pWaitingOutput = NULL;
		} else {
			if (pWaitingOutput == NULL) {
				m_eventWake.Wait(m_timeNextFrameScheduled.ToTimeSpec());
			} else {
				m_eventWake.Wait();
			}
		}
	}
	delete pWaitingOutput;
	delete[] m_pOutput;
	if (m_pGenerator)
		m_pGenerator->Release();
}

std::vector<unsigned char> *CStripThread::GetFrame(bool &bNeedWait, CTime &timeNextFrame) {
	//No frame(s) available
	if (m_qOutput.GetSize() < 1) {
		bNeedWait = false;
		return NULL;
	}
	//Frame has not yet been scheduled
	if (CTime::Now() < m_timeNextOutput) {
		bNeedWait = true;
		timeNextFrame = m_timeNextOutput;
		return NULL;
	}
	//Calculate next frame
	if (m_pConfig->nMaxFramerate > 0)
		m_timeNextOutput = CTime::Now() + CTimeSpan::FromSeconds(1.0 / (double)m_pConfig->nMaxFramerate);
	//Return
	std::vector<unsigned char> *pRetval = m_qOutput.Get();
	m_eventWake.Set();
	return pRetval;
}

void CStripThread::OnFrameSent() {
	CTime timeNextOutput = CTime::Now() + CTimeSpan::FromNanoSeconds(m_pConfig->nMinimumPauseNs);
	if (timeNextOutput > m_timeNextOutput) {
		m_timeNextOutput = timeNextOutput;
	}
}

void CStripThread::ExecuteCommand(IStripCommand *pCmd) {
	Command cmd;
	cmd.pCmd = pCmd;
	cmd.bDone = false;
	//TODO: Proper mutex!
	while (m_qCommands.GetAvailable() < 1)
		m_eventCommand.Wait();
	m_qCommands.Put(&cmd);
	while (!cmd.bDone)
		m_eventCommand.Wait();
}
LightStripConfig *CStripThread::GetConfig() {
	return m_pConfig;
}
void CStripThread::ScheduleFrame() {
	m_bFrameScheduledASAP = true;
	m_eventWake.Set();
}
void CStripThread::ScheduleFrame(CTime &timeNextFrame) {
	if (timeNextFrame < m_timeNextFrameScheduled) {
		m_timeNextFrameScheduled = timeNextFrame;
		m_eventWake.Set();
	}
}