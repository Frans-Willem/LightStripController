#ifndef CTIME_H
#define CTIME_H
#include <time.h>

class CTimeSpan;
class CTime {
private:
	timespec m_time;
	
	friend class CTimeSpan;
public:
	CTime();
	CTime(const CTime &a);
	static CTime Now();
	static CTime Monotonic();
	static CTime Null();
	
	CTimeSpan operator-(const CTime &rhs);
	CTime operator+(const CTimeSpan &rhs);
	CTime &operator+=(const CTimeSpan &rhs);
	bool operator>(const CTime &rhs);
	bool operator>=(const CTime &rhs);
	bool operator<(const CTime &rhs);
	bool operator<=(const CTime &rhs);
	
	timespec ToTimeSpec();
};

class CTimeSpan {
private:
	timespec m_span;
	CTimeSpan();
	
	friend class CTime;
public:
	double ToSeconds();
	int ToIntSeconds();
	static CTimeSpan FromSeconds(int n);
	static CTimeSpan FromSeconds(double n);
	static CTimeSpan FromNanoSeconds(unsigned long nanoseconds);
	
	CTimeSpan operator+(const CTimeSpan &rhs);
};
#endif//CTIME_H
