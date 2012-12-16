#ifndef IFRAMESCHEDULER_H
#define IFRAMESCHEDULER_H
class CTime;
class IFrameScheduler {
public:
	//Will schedule a new frame ASAP.
	virtual void ScheduleFrame() = 0;
	//Will schedule a new frame at that time.
	virtual void ScheduleFrame(CTime &timeNextFrame) = 0;
};
#endif//IFRAMESCHEDULER_H