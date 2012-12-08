#ifndef CEVENT_H
#define CEVENT_H
#include <semaphore.h>
#include <time.h>

class CEvent {
private:
	sem_t m_sem;
public:
	CEvent();
	~CEvent();
	void Set();
	void Reset();
	void Wait();
	void Wait(timespec t);
};
#endif//CEVENT_H
