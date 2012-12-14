#ifndef CFTDIOUTPUT_H
#define CFTDIOUTPUT_H
#include <IOutput.h>
#include <ftdi.h>

class CFtdiOutput : public IOutput {
private:
	CFtdiOutput(ftdi_context fc);
	
	ftdi_context m_fc;
public:
	static CFtdiOutput *Create(int baudrate);
	~CFtdiOutput();
	void Write(const void *pData, size_t nLen);
};
#endif//CFTDIOUTPUT_H
