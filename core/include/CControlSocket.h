#ifndef CCONTROLSOCKET_H
#define CCONTROLSOCKET_H
#include "CControlThread.h"
#include <sys/select.h>
#include <libjson.h>
#include "CConfigObject.h"

class CControlThread;
class CControlSocket {
private:
	CControlSocket(SOCKET sock, CControlThread *pOwner);
	~CControlSocket();

	SOCKET m_sock;
	JSONSTREAM *m_pstream;
	bool m_bHadError;
	CControlThread *m_pOwner;
	
	static void stream_callback(JSONNODE *node, void *pThis);
	static void error_callback(void *pThis);
	
	void onError();
	void onNode(JSONNODE *node);
	void onListPresets(std::string id);
	void onListTransitions(std::string id);
	void onListStrips(std::string id);
	void onChange(std::string id, CConfigObject *arg);
public:
	static CControlSocket *Create(SOCKET sock, CControlThread *pOwner);
	void Destroy();
	int FillFdSet(fd_set *readfds, fd_set *writefds, fd_set *exceptfds);
	bool CheckFdSet(fd_set *readfds, fd_set *writefds, fd_set *exceptfds); //Returns false if this socket has been destroyed, otherwise returns true.
};

#endif//CCONTROLSOCKET_H
