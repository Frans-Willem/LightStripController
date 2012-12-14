#ifndef CTHREAD_H
#define CTHREAD_H
#include <pthread.h>

class CThread {
private:
	pthread_t m_thread;
	bool m_bStarted;
	static void* RunThread(void *pData);
public:
	CThread();
	virtual ~CThread();
	void Start();
	virtual void Stop();
protected:
	virtual void* Run() = 0;
};
#endif//CTHREAD_H
