#include "CSerialOutput.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <linux/serial_core.h>
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <termios.h>

CSerialOutput::CSerialOutput(int nFD) : m_nFD(nFD) {

}

CSerialOutput::~CSerialOutput() {
	close(m_nFD);
}

void CSerialOutput::Write(const void *pData, size_t nLen) {
	write(m_nFD, pData, nLen);
	tcdrain(m_nFD);
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
	/*config.c_oflag = 0;
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	config.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CREAD | PARENB);
	config.c_cflag |= CS8;*/
	cfmakeraw(&config);
	cfsetospeed(&config,B38400);
	config.c_cflag &= ~(CREAD);
	if (tcsetattr(nFD, TCSAFLUSH, &config) < 0) {
		fprintf(stderr, "'%s' is not a valid serial device (tcsetattr failed).\n", szDevice);
		close(nFD);
		return NULL;
	}
	struct serial_struct ss;
	ioctl(nFD, TIOCGSERIAL, &ss);
	ss.flags = (ss.flags & ~ASYNC_SPD_MASK) | ASYNC_SPD_CUST;
	//ss.baud_base = 16000000;
	ss.custom_divisor = (ss.baud_base + (baudrate / 2)) / baudrate;
	if (ss.custom_divisor < 1)
		ss.custom_divisor = 1;
	printf("baud_base: %d %d %d\n", ss.baud_base, ss.custom_divisor, ss.baud_base / ss.custom_divisor);
	ioctl(nFD, TIOCSSERIAL, &ss);
	return new CSerialOutput(nFD);
}