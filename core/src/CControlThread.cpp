#include "CControlThread.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <list>
#include <algorithm>
#include "CControlSocket.h"
#include "main.h"
#include "CStripThread.h"

#define PORT	"12348"
#define BACKLOG	10
#define SOCKET	int


CControlThread::CControlThread(std::list<CStripThread*> lStrips, CConfigObject *pConfig) :
	m_bDone(false),
	m_sock(0),
	m_lStrips(lStrips),
	m_pConfig(pConfig)
{
	addrinfo hints;
	addrinfo* pservinfo;
	SOCKET sockfd;
	int yes = 1;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if (getaddrinfo(NULL, PORT, &hints, &pservinfo) != 0) {	
		std::cerr << "ERR: getaddrinfo failed" << std::endl;
		return;
	}
	for (addrinfo* p=pservinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cerr << "WARN: Unable to open socket" << std::endl;
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			std::cerr << "WARN: setsockopt failed" << std::endl;
			close(sockfd);
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			std::cerr << "WARN: bind failed" << std::endl;
			close(sockfd);
			continue;
		}
		m_sock = sockfd;
		break;
	}
	freeaddrinfo(pservinfo);
	
	if (m_sock == 0) {
		std::cerr << "ERR: Unable to bind to a port" <<std::endl;
		m_bDone = true;
		return;
	}
	if (listen(m_sock, BACKLOG) == -1) {
		std::cerr << "ERR: Unable to listen on port" <<std::endl;
		close(m_sock);
		m_sock = 0;
		m_bDone = true;
		return;
	}
	std::cout << "Socket opened" << std::endl;
}
CControlThread::~CControlThread() {
	
}
void CControlThread::Stop() {
	m_bDone = true;
	if (m_sock)
		close(m_sock); //Should trigger the select call
	CThread::Stop();
}

void* CControlThread::Run() {
	std::list<CControlSocket *> lClients;
	fd_set readfds, writefds, exceptfds;
	char buf[2048];
	while (!m_bDone) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
		
		int numfds = m_sock + 1;
		FD_SET(m_sock, &readfds);
		FD_SET(m_sock, &exceptfds);
		
		for (std::list<CControlSocket *>::iterator si = lClients.begin(); si != lClients.end(); si++) {
			numfds = std::max(numfds, (*si)->FillFdSet(&readfds, &writefds, &exceptfds));
		}
		if (-1 == select(numfds, &readfds, &writefds, &exceptfds, NULL)) {
			std::cerr << "Select returned an error!" << std::endl;
			break;
		}
		
		//Clients
		for (std::list<CControlSocket *>::iterator si = lClients.begin(); si != lClients.end();) {
			if ((*si)->CheckFdSet(&readfds, &writefds, &exceptfds))
				si++;
			else
				si = lClients.erase(si);
		}
		
		//Server
		if (FD_ISSET(m_sock, &exceptfds)) {
			//Close the socket
			close(m_sock);
			m_sock = 0;
			m_bDone = true;
			break;
		}
		
		if (FD_ISSET(m_sock, &readfds)) {
			sockaddr_storage remoteaddr;
			socklen_t addrlen = sizeof(remoteaddr);
			SOCKET sock = accept(m_sock, (sockaddr *)&remoteaddr, &addrlen);
			if (sock >= 0) {
				lClients.push_back(CControlSocket::Create(sock, this));
			} else {
				std::cerr << "accept failed" << std::endl;
			}
		}
	}
	for (std::list<CControlSocket *>::iterator si = lClients.begin(); si != lClients.end(); si = lClients.erase(si)) {
		(*si)->Destroy();
	}
	return NULL;
}

bool CControlThread::Change(std::set<int> sStrips, std::string strTransition, std::string strPreset) {
	std::list<CStripThread*> lStrips;
	for (std::list<CStripThread*>::iterator i = m_lStrips.begin(); i != m_lStrips.end(); i++) {
		if (sStrips.find((*i)->GetConfig()->nId) != sStrips.end())
			lStrips.push_back(*i);
	}
	return change(lStrips, strTransition, strPreset, m_pConfig);
}

std::set<std::string> CControlThread::GetPresets() {
	return m_pConfig->get("presets")->keys();
}
std::set<std::string> CControlThread::GetTransitions() {
	return m_pConfig->get("transitions")->keys();
}
std::map<int, std::string> CControlThread::GetStrips() {
	std::map<int, std::string> strips;
	for (std::list<CStripThread*>::iterator i=m_lStrips.begin(); i!=m_lStrips.end(); i++) {
		LightStripConfig *pStripConfig = (*i)->GetConfig();
		strips.insert(std::pair<int,std::string>(pStripConfig->nId, pStripConfig->strName));
	}
	return strips;
}