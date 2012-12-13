#include "CEvent.h"

CEvent::CEvent() {
	sem_init(&m_sem, 0, 0);
}

CEvent::~CEvent() {
	sem_destroy(&m_sem);
}

void CEvent::Set() {
	sem_post(&m_sem);
}

void CEvent::Reset() {
	while (sem_trywait(&m_sem) == 0);
}

void CEvent::Wait() {
	sem_wait(&m_sem);
	Reset();
}

void CEvent::Wait(timespec t) {
	if (0 == sem_timedwait(&m_sem, &t))
		Reset();
}