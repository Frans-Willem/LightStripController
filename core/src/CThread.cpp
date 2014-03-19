#include "CThread.h"
#include <string>
#include <stdio.h>

void* CThread::RunThread(void *pData) {
	CThread *thread = (CThread *)pData;
	try {
		return thread->Run();
	} catch (std::string &s) {
		fprintf(stderr, "(Thread) std::string exception:\n\t%s\n", s.c_str());
	}
	return NULL;
}

CThread::CThread() {
	m_bStarted = false;
}
CThread::~CThread() {
	if (m_bStarted) {
		throw std::string("Thread was not stopped yet!");
	}
}
void CThread::Start() {
	if (m_bStarted) {
		throw std::string("Thread was already started");
	}
	m_bStarted = true;
	pthread_create(&m_thread, NULL, &CThread::RunThread, (void *)this);
}
void CThread::Stop() {
	if (m_bStarted) {
		void *retval;
		pthread_join(m_thread, &retval);
	}
	m_bStarted = false;
}