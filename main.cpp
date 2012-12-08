#include <stdio.h>
#include <stdlib.h>
#include "CEvent.h"
#include <pthread.h>
#include <unistd.h>
#include "main.h"
#include <time.h>
#include <list>
#include "CSizedQueue.h"
#include "CTime.h"
#include <ftdi.h>
#include <vector>
#include <math.h>
#include "CColor.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

LightStripConfig pConfig[] = {
	{
		0,
		"Bureau",
		142, 160,
		5000000, 0
	},
	{
		1,
		"TV",
		142, 160,
		5000000, 0
	},
	{
		2,
		"Tuin",
		160, 160,
		5000000, 0
	},
};

struct LightStripData {
	LightStripConfig *pConfig;
	CSizedQueue<std::vector<unsigned char> *> *pQueue;
	CEvent *pEventNewData;
	CEvent *pEventDataSent;
	CTime timeNextFrame;
	pthread_t thread;
};

double lum2duty(double lum) {
	if (lum>0.07999591993063804) {
		return pow(((lum+0.16)/1.16),3.0);
	} else {
		return lum/9.033;
	}
}

unsigned char color2byte(double color) {
	double duty = lum2duty(color);
	duty *= 256.0;
	if (duty < 0.0) return 0;
	if (duty >= 255.0) return 255;
	return (int)duty;
}

void* stripproc(void *pVoidData) {
	LightStripData *pData = (LightStripData *)pVoidData;
	int i;
	CTime timeStart = CTime::Now();
	int frame = 0;
	while (true) {
		std::vector<CColor> data;
		double seconds = (CTime::Now() - timeStart).ToSeconds();
		for (i=0; i<pData->pConfig->nLengthDisplay; i++) {
			double mysec = (seconds / 3.0) + (i * 0.01);
			/*CColor c;
			c.r = fmod(mysec,2.0) - 1.0;
			if (c.r < 0.0) c.r = 0.0 - c.r;*/
			data.push_back(CColor::HSL(fmod(mysec, 1.0), 1.0, 0.5));
		}
		std::vector<unsigned char> *pOutput = new std::vector<unsigned char>();
		pOutput->push_back((UART_PACKET_RECIPIENT << 6) | pData->pConfig->nId);
		for (i=0; i<pData->pConfig->nLengthDisplay && i<data.size(); i++) {
			CColor c = data[i];
			unsigned char r = color2byte(c.r);
			unsigned char g = color2byte(c.g);
			unsigned char b = color2byte(c.b);
			pOutput->push_back((UART_PACKET_DATA << 6) | (b >> 2));
			pOutput->push_back((UART_PACKET_DATA << 6) | ((b << 4) & 0x3F) | (g >> 4));
			pOutput->push_back((UART_PACKET_DATA << 6) | ((g << 2) & 0x3F) | (r >> 6));
			pOutput->push_back((UART_PACKET_DATA << 6) | (r & 0x3F));
		}
		for (; i<pData->pConfig->nLengthTotal; i++) {
			pOutput->push_back(UART_PACKET_DATA << 6);
			pOutput->push_back(UART_PACKET_DATA << 6);
			pOutput->push_back(UART_PACKET_DATA << 6);
			pOutput->push_back(UART_PACKET_DATA << 6);
		}
		while (pData->pQueue->GetAvailable() == 0) {
			pData->pEventDataSent->Wait();
		}
		pData->pQueue->Put(pOutput);
		pData->pEventNewData->Set();
		
		if ((frame += 1) % 50 == 0) {
			double fps = (double)frame / (CTime::Now() - timeStart).ToSeconds();
			printf("Framerate %d: %d %e\n", pData->pConfig->nId, (int)fps, fps);
		}
	}
	//Do nothing for now
}

void mainloop(int fd) {
	CEvent eventNewData;
	std::list<LightStripData *> lStrips;
	for (unsigned int i = 0; i < (sizeof(pConfig)/sizeof(*pConfig)); i++) {
		LightStripData *data = new LightStripData;
		data->pConfig = &pConfig[i];
		data->pQueue = new CSizedQueue<std::vector<unsigned char> *>(1);
		data->pEventNewData = &eventNewData;
		data->pEventDataSent = new CEvent();
		data->timeNextFrame = CTime::Now();
		pthread_create(&data->thread, NULL, stripproc, data);
		lStrips.push_back(data);
	}
	
	while (true) {
		bool bDone = false;
		bool bWait = false;
		CTime timeWaitTill;
		CTime timeNow = CTime::Now();
		for (std::list<LightStripData *>::iterator i = lStrips.begin(); !bDone && i != lStrips.end(); i++) {
			LightStripData *pData = *i;
			if (pData->pQueue->GetSize()) {
				if (pData->timeNextFrame > timeNow) {
					//Keep track until when to wait
					if (!bWait || pData->timeNextFrame < timeWaitTill) {
						timeWaitTill = pData->timeNextFrame;
					}
					bWait = true;
				} else {
					//Yes! we can push out one!
					std::vector<unsigned char> *pOutput = pData->pQueue->Get();
					pData->pEventDataSent->Set();
					/*int ret = ftdi_write_data(pftdic, &pOutput->front(), pOutput->size());
					if (ret < 0) {
						printf("ftdi_write_data failed on %d: %d\n", pData->pConfig->nId, ret);
					} else if (ret < pOutput->size()) {
						printf("ftdi_write_data not all data on %d: %d\n", pData->pConfig->nId, ret);
					}*/
					write(fd, (void *)&pOutput->front(), pOutput->size());
					delete pOutput;
					//Calculate when the next frame can be pushed out
					//Use time when frame was started as offset.
					CTime timeNextFramerate = timeNow;
					if (pData->pConfig->nMaxFramerate > 0) {
						timeNextFramerate += CTimeSpan::FromSeconds(1.0 / (double)pData->pConfig->nMaxFramerate);
					}
					//Use time when frame was sent out as offset.
					CTime timeNextPause = CTime::Now() + CTimeSpan::FromNanoSeconds(pData->pConfig->nMinimumPauseNs);
					if (timeNextPause > timeNextFramerate) {
						pData->timeNextFrame = timeNextPause;
					} else {
						pData->timeNextFrame = timeNextFramerate;
					}
					//Move to end of list so other strips will be checked first.
					lStrips.erase(i);
					lStrips.push_back(pData);
					bDone = true;
					break;
				}
			}
		}
		if (!bDone) {
			if (bWait) {
				eventNewData.Wait(timeWaitTill.ToTimeSpec());
			} else {
				eventNewData.Wait();
			}
		}
	}
	
	//End properly:
	for (std::list<LightStripData *>::iterator i = lStrips.begin(); i != lStrips.end(); i++) {
		void *retval;
		pthread_join((*i)->thread, &retval);
		delete *i;
	}
}

int main(int argc, const char **argv) {
	char szDefaultDevice[] = "/dev/ttyAMA0";
	const char *szDevice = szDefaultDevice;
	if (argc > 1) {
		szDevice = argv[1];
	}
	int fd = open(szDevice, O_NOCTTY|O_WRONLY);
	printf("FD %d\n", fd);
	mainloop(fd);
	close(fd);
	return 0;
}