#include <stdio.h>
#include <stdlib.h>
#include <IOutput.h>
#include "CSerialOutput.h"
#include <string>
#include <libconfig.h++>

extern "C" IOutput *CreateOutput(libconfig::Setting& s) {
	std::string strDevice = s["device"];
	return CSerialOutput::Create(strDevice.c_str(), s["baudrate"]);
}