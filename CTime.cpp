#include "CTime.h"
#include <string.h>

CTime::CTime() {
	memset((void *)&m_time, 0, sizeof(m_time));
}

CTime::CTime(const CTime &a) {
	m_time = a.m_time;
}

CTime CTime::Now() {
	CTime ret;
	clock_gettime(CLOCK_REALTIME, &ret.m_time);
	return ret;
}

CTime CTime::Monotonic() {
	CTime ret;
	clock_gettime(CLOCK_MONOTONIC, &ret.m_time);
	return ret;
}

CTime CTime::Null() {
	return CTime();
}

CTimeSpan CTime::operator-(const CTime &rhs) {
	CTimeSpan ret;
	ret.m_span.tv_sec = m_time.tv_sec - rhs.m_time.tv_sec;
	ret.m_span.tv_nsec = m_time.tv_nsec - rhs.m_time.tv_nsec;
	if (rhs.m_time.tv_nsec > m_time.tv_nsec) {
		ret.m_span.tv_sec -= 1;
		ret.m_span.tv_nsec += 1000000000;
	}
	return ret;
}

CTime CTime::operator+(const CTimeSpan &rhs) {
	CTime ret;
	unsigned long nsec = m_time.tv_nsec + rhs.m_span.tv_nsec;
	ret.m_time.tv_sec = m_time.tv_sec + rhs.m_span.tv_sec + (nsec / 1000000000);
	ret.m_time.tv_nsec = nsec % 1000000000;
	return ret;
}

CTime &CTime::operator+=(const CTimeSpan &rhs) {
	unsigned long nsec = m_time.tv_nsec + rhs.m_span.tv_nsec;
	m_time.tv_sec += rhs.m_span.tv_sec + (nsec / 1000000000);
	m_time.tv_nsec = nsec % 1000000000;
	return *this;
}

bool CTime::operator>(const CTime &rhs) {
	if (m_time.tv_sec > rhs.m_time.tv_sec) return true;
	if (m_time.tv_sec < rhs.m_time.tv_sec) return false;
	if (m_time.tv_nsec > rhs.m_time.tv_nsec) return true;
	if (m_time.tv_nsec < rhs.m_time.tv_nsec) return false;
	return false;	
}

bool CTime::operator>=(const CTime &rhs) {
	if (m_time.tv_sec > rhs.m_time.tv_sec) return true;
	if (m_time.tv_sec < rhs.m_time.tv_sec) return false;
	if (m_time.tv_nsec > rhs.m_time.tv_nsec) return true;
	if (m_time.tv_nsec < rhs.m_time.tv_nsec) return false;
	return true;	
}

bool CTime::operator<(const CTime &rhs) {
	if (m_time.tv_sec < rhs.m_time.tv_sec) return true;
	if (m_time.tv_sec > rhs.m_time.tv_sec) return false;
	if (m_time.tv_nsec < rhs.m_time.tv_nsec) return true;
	if (m_time.tv_nsec > rhs.m_time.tv_nsec) return false;
	return false;	
}

bool CTime::operator<=(const CTime &rhs) {
	if (m_time.tv_sec < rhs.m_time.tv_sec) return true;
	if (m_time.tv_sec > rhs.m_time.tv_sec) return false;
	if (m_time.tv_nsec < rhs.m_time.tv_nsec) return true;
	if (m_time.tv_nsec > rhs.m_time.tv_nsec) return false;
	return true;	
}

timespec CTime::ToTimeSpec() {
	return m_time;
}

CTimeSpan::CTimeSpan() {
	//Don't do anything yet
	memset((void *)&m_span, 0 , sizeof(m_span));
};

double CTimeSpan::ToSeconds() {
	double ret = m_span.tv_sec;
	ret += ((double)m_span.tv_nsec) / 1000000000.0;
	return ret;
}

int CTimeSpan::ToIntSeconds() {
	return m_span.tv_sec;
}

CTimeSpan CTimeSpan::FromSeconds(int n) {
	CTimeSpan ret;
	ret.m_span.tv_sec = n;
	ret.m_span.tv_nsec = 0;
	return ret;
}

CTimeSpan CTimeSpan::FromSeconds(double n) {
	CTimeSpan ret;
	ret.m_span.tv_sec = (int) n;
	n -= ret.m_span.tv_sec;
	n *= 1000000000.0;
	ret.m_span.tv_nsec = (int) n;
	return ret;
}

CTimeSpan CTimeSpan::FromNanoSeconds(unsigned long n) {
	CTimeSpan ret;
	ret.m_span.tv_sec = n / 1000000000;
	ret.m_span.tv_nsec = n % 1000000000;
	return ret;
}

CTimeSpan CTimeSpan::operator+(const CTimeSpan &rhs) {
	CTimeSpan ret;
	unsigned long nsec = m_span.tv_nsec + rhs.m_span.tv_nsec;
	ret.m_span.tv_sec = m_span.tv_sec + rhs.m_span.tv_sec + (nsec / 1000000000);
	ret.m_span.tv_nsec = nsec % 1000000000;
	return ret;
}

