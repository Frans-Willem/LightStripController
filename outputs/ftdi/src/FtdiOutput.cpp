#include <stdio.h>
#include <stdlib.h>
#include <IOutput.h>
#include "CFtdiOutput.h"
#include <CConfigObject.h>

extern "C" IOutput *CreateOutput(CConfigObject *s) {
	return CFtdiOutput::Create(s->getInt("baudrate"));
}