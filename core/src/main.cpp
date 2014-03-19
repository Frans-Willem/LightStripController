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
#include <IOutput.h>
#include <dlfcn.h>
#include "IGenerator.h"
#include "CStripThread.h"
#include "COutputThread.h"
#include <iostream>
#include <IStripCommand.h>
#include "CModuleGenerator.h"
#include "CControlThread.h"
#include "CConfigObject.h"
#include <libjson.h>
#include "CJSONConfigObject.h"
#include <fstream>

std::string strRoot;

class CLoadModuleCommand : public IStripCommand {
private:
	std::string m_strModuleGenerator;
	CConfigObject *m_configGenerator;
	std::string m_strModuleTransition;
	CConfigObject *m_configTransition;
public:
	CLoadModuleCommand(std::string strModuleGenerator, CConfigObject *configGenerator, std::string strModuleTransition, CConfigObject *configTransition) :
		m_strModuleGenerator(strModuleGenerator),
		m_strModuleTransition(strModuleTransition),
		m_configGenerator(configGenerator),
		m_configTransition(configTransition)
	{

	}
	void Execute(CStripThread *pThread, IGenerator **ppGenerator) {
		printf("Execute CLoadModuleCommand:\n\t%s\n\t%s\n\t%d\n", m_strModuleGenerator.c_str(), m_strModuleTransition.c_str(), pThread->GetConfig()->nId);
		std::vector<IGenerator*> vArguments;
		IGenerator *pNewGenerator = CModuleGenerator::Create(m_strModuleGenerator, pThread->GetConfig()->nLengthDisplay, m_configGenerator, pThread, vArguments);
		if (pNewGenerator) {
			vArguments.push_back(pNewGenerator);
			vArguments.push_back(*ppGenerator);
			IGenerator *pTransition = CModuleGenerator::Create(m_strModuleTransition, pThread->GetConfig()->nLengthDisplay, m_configTransition, pThread, vArguments);
			if (pTransition) {
				if (*ppGenerator)
					(*ppGenerator)->Release();
				*ppGenerator = pTransition;
				pTransition->AddRef();
				//---
				pTransition->Release();
			} else {
				printf("Unable to load transition :(\n");
				if (*ppGenerator)
					(*ppGenerator)->Release();
				*ppGenerator = pNewGenerator;
				pNewGenerator->AddRef();
			}
			pNewGenerator->Release();
		}
		//Schedule a new frame ASAP!
		pThread->ScheduleFrame();
	}
};

bool change(std::list<CStripThread*> lStrips, std::string strTransition, std::string strPreset, CConfigObject *pConfig) {
	CConfigObject* sPresets = pConfig->get("presets");
	CConfigObject* sTransitions = pConfig->get("transitions");
	CConfigObject *sPreset=sPresets->get(strPreset);
	CConfigObject *sTransition=sTransitions->get(strTransition);
	if (sPreset->isNull()) {
		std::cout<<"Preset does not exist"<<std::endl;
	} else if (sTransition->isNull()) {
		std::cout<<"Transition does not exist"<<std::endl;
	} else {
		std::cout<<"LoadModuleCommand"<<std::endl;
		CLoadModuleCommand cmd(strRoot + "/generators/" + sPreset->getString("type") + ".so", sPreset->get("config"), strRoot + "/transitions/" + sTransition->getString("type") + ".so", sTransition->get("config"));
		std::cout<<"Execute"<<std::endl;
		for (std::list<CStripThread *>::iterator i = lStrips.begin(); i!=lStrips.end(); i++) {
			(*i)->ExecuteCommand(&cmd);
		}
	}
	return true;
}

void mainloop(IOutput *pOutput, CConfigObject *pConfig) {
	CConfigObject *configStrips = pConfig->get("strips");
	CEvent eventNewData;
	std::list<CStripThread *> lStrips;
	for (int i = 0; configStrips && i < configStrips->length(); i++) {
		CConfigObject *currentStrip = configStrips->get(i);
		LightStripConfig *config = new LightStripConfig;
		config->nId = currentStrip->getInt("id");
		config->strName = currentStrip->getString("name");
		config->nLengthDisplay = currentStrip->getInt("visibleLength");
		config->nLengthTotal = currentStrip->getInt("totalLength");
		config->nMinimumPauseNs = currentStrip->getInt("pauseNs");
		config->nMaxFramerate = currentStrip->getInt("maxFramerate");
		CStripThread *pThread = new CStripThread(config, &eventNewData);
		pThread->Start();
		lStrips.push_back(pThread);
	}
	
	COutputThread threadOutput(pOutput, lStrips, &eventNewData);
	threadOutput.Start();
	CControlThread threadControl(lStrips, pConfig);
	threadControl.Start();
	
	std::cout<<"Running"<<std::endl;
	std::string strLine;
	while (std::getline(std::cin, strLine)) {
		std::string::size_type nSplit = strLine.find_first_of(' ');
		if (nSplit == std::string::npos) {
			std::cout<<"<transition> <generator>"<<std::endl;
		} else {
			std::string strTransition(strLine, 0, nSplit);
			std::string strPreset(strLine, nSplit+1);
			change(lStrips, strTransition, strPreset, pConfig);
		}
	}
	
	//End properly:
	//TODO
	/*
	for (std::list<CStripThread *>::iterator i = lStrips.begin(); i != lStrips.end(); i++) {
		void *retval;
		pthread_join((*i)->thread, &retval);
		delete (*i)->pConfig;
		delete *i;
	}
	*/
}

struct StreamResult {
	bool error;
	JSONNODE *node;
};

void stream_node_callback(JSONNODE *node, void *p) {
	if (((StreamResult *)p)->error)
		return;
	((StreamResult *)p)->node = json_copy(node);
}

void stream_error_callback(void *p) {
	((StreamResult *)p)->error = true;
}

extern "C" int coreEntryPoint(const char *szRoot, int argc, const char **argv) {
	strRoot = szRoot;
	
	std::ifstream ifConfig((argc>1)?argv[1]:"lights.cfg");
	if (ifConfig.is_open()) {
		StreamResult res;
		res.error=false;
		res.node=NULL;
		JSONSTREAM *str = json_new_stream(&stream_node_callback, &stream_error_callback, (void *)&res);
		char buffer[1024];
		std::streamsize n;
		while (n = ifConfig.readsome(buffer, sizeof(buffer)-1)) {
			buffer[n]=0;
			json_stream_push(str, buffer);
		}
		json_delete_stream(str);
		ifConfig.close();
		
		if (res.node != NULL && !res.error) {
			json_preparse(res.node);
			try {
				CConfigObject *config=new CJSONConfigObject(res.node);
				CConfigObject *settingOutput=config->get("output");
				CConfigObject *settingOutputConfig=settingOutput->get("config");
				std::string strOutputLib(szRoot);
				strOutputLib += "/outputs/";
				strOutputLib += settingOutput->getString("type");
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
			} catch (std::string &s) {
				fprintf(stderr, "std::string exception:\n\t%s\n", s.c_str());
			}
		} else {
			std::cerr<<"Unable to parse config file (check jsonlint?)"<<std::endl;
		}
	} else {
		std::cerr<<"Unable to open config file"<<std::endl;
	}
	return 0;
}