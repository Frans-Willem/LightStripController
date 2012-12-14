#include "CFtdiOutput.h"
#include <stdio.h>

CFtdiOutput::CFtdiOutput(ftdi_context fc) : m_fc(fc) {

}

CFtdiOutput::~CFtdiOutput() {
	ftdi_usb_close(&m_fc);
	ftdi_deinit(&m_fc);
}

void CFtdiOutput::Write(const void *pData, size_t nLen) {
	ftdi_write_data(&m_fc, (unsigned char *)pData, nLen);
}

CFtdiOutput *CFtdiOutput::Create(int baudrate) {
	ftdi_context ftdic;
	if (ftdi_init(&ftdic) < 0) {
		fprintf(stderr, "ftdi_init failed.\n");
		return NULL;
	}
	if (ftdi_usb_open(&ftdic, 0x0403, 0x6001) < 0) {
		fprintf(stderr, "ftdi_usb_open failed.\n");
		ftdi_deinit(&ftdic);
		return NULL;
	}
	if (ftdi_set_baudrate(&ftdic, baudrate) < 0) {
		fprintf(stderr, "ftdi_set_baudrate failed.\n");
		ftdi_usb_close(&ftdic);
		ftdi_deinit(&ftdic);
		return NULL;
	}
	return new CFtdiOutput(ftdic);
}