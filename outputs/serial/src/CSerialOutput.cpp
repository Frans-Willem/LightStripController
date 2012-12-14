#include "CSerialOutput.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
/*#include <linux/serial_core.h>
#include <asm/ioctls.h>
#include <sys/ioctl.h>*/

#define BAUDCASE(x) case x: speed = B ## x ; break;

CSerialOutput::CSerialOutput(int nFD) : m_nFD(nFD) {

}

CSerialOutput::~CSerialOutput() {
	close(m_nFD);
}

void CSerialOutput::Write(const void *pData, size_t nLen) {
	write(m_nFD, pData, nLen);
}

CSerialOutput *CSerialOutput::Create(const char *szDevice, unsigned int baudrate) {
	int nFD=open(szDevice, O_NOCTTY|O_RDWR);
	if (nFD == -1) {
		fprintf(stderr, "Unable to open serial device '%s'.\n", szDevice);
		return NULL;
	}
	if (!isatty(nFD)) {
		fprintf(stderr, "'%s' is not a valid serial device (isatty failed).\n", szDevice);
		close(nFD);
		return NULL;
	}
	struct termios config;
	if (tcgetattr(nFD, &config) < 0) {
		fprintf(stderr, "'%s' is not a valid serial device (tcgetattr failed).\n", szDevice);
		close(nFD);
		return NULL;
	}

	cfmakeraw(&config);
	speed_t speed;
	switch (baudrate) {
		default:
			fprintf(stderr, "%d is an unsupported baudrate.\n", baudrate);
			close(nFD);
			return NULL;
		BAUDCASE(50)
		BAUDCASE(75)
		BAUDCASE(110)
		BAUDCASE(134)
		BAUDCASE(150)
		BAUDCASE(200)
		BAUDCASE(300)
		BAUDCASE(600)
		BAUDCASE(1200)
		BAUDCASE(1800)
		BAUDCASE(2400)
		BAUDCASE(4800)
		BAUDCASE(9600)
		BAUDCASE(19200)
		BAUDCASE(38400)
		BAUDCASE(57600)
		BAUDCASE(115200)
		BAUDCASE(230400)
		BAUDCASE(460800)
		BAUDCASE(500000)
		BAUDCASE(576000)
		BAUDCASE(921600)
		BAUDCASE(1000000)
		BAUDCASE(1152000)
		BAUDCASE(1500000)
		BAUDCASE(2000000)
		BAUDCASE(2500000)
		BAUDCASE(3000000)
		BAUDCASE(3500000)
		BAUDCASE(4000000)
	}
	cfsetospeed(&config, speed);
	cfsetispeed(&config, speed);
	config.c_cflag &= ~(CREAD);

	if (tcsetattr(nFD, TCSANOW, &config) < 0) {
		fprintf(stderr, "'%s' is not a valid serial device (tcsetattr failed).\n", szDevice);
		close(nFD);
		return NULL;
	}
	tcflush(nFD, TCIOFLUSH);
	return new CSerialOutput(nFD);
}