#ifndef MAIN_H
#define MAIN_H
#include <string>

#define UART_PACKET_NOTUSED 0
#define UART_PACKET_RECIPIENT 1
#define UART_PACKET_DATA 2
#define UART_PACKET_DEBUG 3

struct LightStripConfig {
	int nId;
	std::string strName; //Name
	int nLengthDisplay; //Number of LEDs that should light up
	int nLengthTotal; //Total number of LEDs, including ones that should be off. (usually 160)
	unsigned long nMinimumPauseNs; //In nanoseconds, minimum pause between frames.
	unsigned long nMaxFramerate; //Maximum number of frames to allow, or 0 for infinite
};
#endif//MAIN_H
