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
#include <libconfig.h++>
#include <IOutput.h>
#include <dlfcn.h>

using namespace libconfig;

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

void mainloop(IOutput *pOutput, Config &config) {
	Setting &strips = config.lookup("strips");
	CEvent eventNewData;
	std::list<LightStripData *> lStrips;
	for (int i = 0; i < strips.getLength(); i++) {
		Setting &currentStrip = strips[i];
		LightStripConfig *config = new LightStripConfig;
		config->nId = currentStrip["id"];
		config->strName = currentStrip["name"].c_str();
		config->nLengthDisplay = currentStrip["visibleLength"];
		config->nLengthTotal = currentStrip["totalLength"];
		config->nMinimumPauseNs = currentStrip["pauseNs"];
		config->nMaxFramerate = currentStrip["maxFramerate"];
		/*
			int nId;
	std::string strName; //Name
	int nLengthDisplay; //Number of LEDs that should light up
	int nLengthTotal; //Total number of LEDs, including ones that should be off. (usually 160)
	unsigned long nMinimumPauseNs; //In nanoseconds, minimum pause between frames.
	unsigned long nMaxFramerate; //Maximum number of frames to allow, or 0 for infinite
		*/
		LightStripData *data = new LightStripData;
		data->pConfig = config;
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
					std::vector<unsigned char> *pOutputData = pData->pQueue->Get();
					pData->pEventDataSent->Set();
					//write(fd, (void *)&pOutput->front(), pOutput->size());
					pOutput->Write((void *)&pOutputData->front(), pOutputData->size());
					delete pOutputData;
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
		delete (*i)->pConfig;
		delete *i;
	}
}

extern "C" int coreEntryPoint(const char *szRoot, int argc, const char **argv) {
	try {
		Config config;
		//config.setAutoConvert(true);
		config.readFile((argc>1)?argv[1]:"lights.cfg");
		Setting& settingOutput=config.lookup("output");
		Setting& settingOutputConfig=settingOutput["config"];
		std::string strOutputLib(szRoot);
		strOutputLib += "/outputs/";
		strOutputLib += settingOutput["type"].c_str();
		strOutputLib += ".so";
		void *dlOutput = dlopen(strOutputLib.c_str(), RTLD_NOW);
		if (!dlOutput) {
			fprintf(stderr, "Unable to open '%s':\n\t%s\n", strOutputLib.c_str(), dlerror());
		} else {
			CreateOutputPtr pCreateOutput = (CreateOutputPtr)dlsym(dlOutput,"CreateOutput");
			if (!pCreateOutput) {
				fprintf(stderr, "No CreateOutput symbol found in '%s'.\n", strOutputLib.c_str());
			} else {
				IOutput *pOutput = pCreateOutput(settingOutputConfig);
				if (!pOutput) {
					fprintf(stderr, "CreateOutput returned NULL.\n");
				} else {
					mainloop(pOutput, config);
					delete pOutput;
				}
			}
			dlclose(dlOutput);
		}
		/*
		char szDefaultDevice[] = "/dev/ttyAMA0";
		const char *szDevice = szDefaultDevice;
		if (argc > 1) {
			szDevice = argv[1];
		}
		int fd = open(szDevice, O_NOCTTY|O_WRONLY);
		printf("FD %d\n", fd);
		mainloop(fd, config);
		close(fd);*/
	} catch (SettingException &s) {
		fprintf(stderr, "Setting exception (%s):\n\t%s\n", s.what(), s.getPath());
	}
	return 0;
}