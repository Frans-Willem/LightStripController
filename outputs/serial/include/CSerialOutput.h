#ifndef CSERIALOUTPUT_H
#define CSERIALOUTPUT_H
#include <IOutput.h>

class CSerialOutput : public IOutput {
private:
	CSerialOutput(int nFD);
	
	int m_nFD;
public:
	static CSerialOutput *Create(const char *szDevice, unsigned int baudrate);
	~CSerialOutput();
	void Write(const void *pData, size_t nLen);
};
#endif//CSERIALOUTPUT_H
