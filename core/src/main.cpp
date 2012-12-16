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
#include "IGenerator.h"
#include "CStripThread.h"
#include "COutputThread.h"
#include <iostream>
#include <IStripCommand.h>
#include "CModuleGenerator.h"

using namespace libconfig;

std::string strRoot;

class CLoadModuleCommand : public IStripCommand {
private:
	std::string m_strModuleGenerator;
	Setting& m_settingGenerator;
	std::string m_strModuleTransition;
	Setting& m_settingTransition;
public:
	CLoadModuleCommand(std::string strModuleGenerator, Setting& settingGenerator, std::string strModuleTransition, Setting& settingTransition) :
		m_strModuleGenerator(strModuleGenerator),
		m_strModuleTransition(strModuleTransition),
		m_settingGenerator(settingGenerator),
		m_settingTransition(settingTransition)
	{
	}
	void Execute(LightStripConfig *pConfig, IGenerator **ppGenerator) {
		printf("Execute CLoadModuleCommand:\n\t%s\n\t%s\n\t%d\n", m_strModuleGenerator.c_str(), m_strModuleTransition.c_str(), pConfig->nId);
		std::vector<IGenerator*> vArguments;
		IGenerator *pNewGenerator = CModuleGenerator::Create(m_strModuleGenerator, vArguments, pConfig->nLengthDisplay, m_settingGenerator);
		if (pNewGenerator) {
			vArguments.push_back(pNewGenerator);
			vArguments.push_back(*ppGenerator);
			IGenerator *pTransition = CModuleGenerator::Create(m_strModuleTransition, vArguments, pConfig->nLengthDisplay, m_settingTransition);
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
	}
};

void mainloop(IOutput *pOutput, Config &config) {
	Setting &strips = config.lookup("strips");
	CEvent eventNewData;
	std::list<CStripThread *> lStrips;
	for (int i = 0; i < strips.getLength(); i++) {
		Setting &currentStrip = strips[i];
		LightStripConfig *config = new LightStripConfig;
		config->nId = currentStrip["id"];
		config->strName = currentStrip["name"].c_str();
		config->nLengthDisplay = currentStrip["visibleLength"];
		config->nLengthTotal = currentStrip["totalLength"];
		config->nMinimumPauseNs = currentStrip["pauseNs"];
		config->nMaxFramerate = currentStrip["maxFramerate"];
		CStripThread *pThread = new CStripThread(config, &eventNewData);
		pThread->Start();
		lStrips.push_back(pThread);
	}
	
	COutputThread threadOutput(pOutput, lStrips, &eventNewData);
	threadOutput.Start();
	
	std::cout<<"Running"<<std::endl;
	std::string strLine;
	Setting& sPresets = config.lookup("presets");
	Setting& sTransitions = config.lookup("transitions");
	while (std::getline(std::cin, strLine)) {
		std::string::size_type nSplit = strLine.find_first_of(' ');
		if (nSplit == std::string::npos) {
			std::cout<<"<transition> <generator>"<<std::endl;
		} else {
			std::string strTransition(strLine, 0, nSplit);
			std::string strPreset(strLine, nSplit+1);
			if (!sPresets.exists(strPreset)) {
				std::cout<<"Preset does not exist"<<std::endl;
			} else if (!sTransitions.exists(strTransition)) {
				std::cout<<"Transition does not exist"<<std::endl;
			} else {
				Setting& sPreset = sPresets[strPreset];
				Setting& sTransition = sTransitions[strTransition];
				CLoadModuleCommand cmd(strRoot + "/generators/" + sPreset["type"].c_str() + ".so", sPreset["config"], strRoot + "/transitions/" + sTransition["type"].c_str() + ".so", sTransition["config"]);
				for (std::list<CStripThread *>::iterator i = lStrips.begin(); i!=lStrips.end(); i++) {
					(*i)->ExecuteCommand(&cmd);
				}
			}
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

extern "C" int coreEntryPoint(const char *szRoot, int argc, const char **argv) {
	try {
		strRoot = szRoot;
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
	} catch (SettingException &s) {
		fprintf(stderr, "Setting exception (%s):\n\t%s\n", s.what(), s.getPath());
	} catch (std::string &s) {
		fprintf(stderr, "std::string exception:\n\t%s\n", s.c_str());
	}
	return 0;
}