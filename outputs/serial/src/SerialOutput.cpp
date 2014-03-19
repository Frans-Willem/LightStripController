#include <stdio.h>
#include <stdlib.h>
#include <IOutput.h>
#include "CSerialOutput.h"
#include <string>
#include <CConfigObject.h>

extern "C" IOutput *CreateOutput(CConfigObject *s) {
	std::string strDevice = s->getString("device");
	return CSerialOutput::Create(strDevice.c_str(), s->getInt("baudrate"));
}