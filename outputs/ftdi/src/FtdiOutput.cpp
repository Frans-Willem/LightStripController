#include <stdio.h>
#include <stdlib.h>
#include <IOutput.h>
#include "CFtdiOutput.h"
#include <libconfig.h++>

extern "C" IOutput *CreateOutput(libconfig::Setting& s) {
	return CFtdiOutput::Create(s["baudrate"]);
}