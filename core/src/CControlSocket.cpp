#include "CControlSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include "CJSONConfigObject.h"

CControlSocket::CControlSocket(SOCKET sock, CControlThread *pOwner) :
	m_sock(sock),
	m_bHadError(false),
	m_pOwner(pOwner)
{
	m_pstream = json_new_stream(&stream_callback, &error_callback, (void*)this);
}
CControlSocket::~CControlSocket() {
	json_delete_stream(m_pstream);
}
CControlSocket *CControlSocket::Create(SOCKET sock, CControlThread *pOwner) {
	return new CControlSocket(sock, pOwner);
}
void CControlSocket::Destroy() {
	close(m_sock);
	delete this;
}
int CControlSocket::FillFdSet(fd_set *readfds, fd_set *writefds, fd_set *exceptfds) {
	FD_SET(m_sock, readfds);
	FD_SET(m_sock, exceptfds);
	return m_sock + 1;
}
bool CControlSocket::CheckFdSet(fd_set *readfds, fd_set *writefds, fd_set *exceptfds) {
	char buf[2048];
	if (FD_ISSET(m_sock, exceptfds)) {
		Destroy();
		return false;
	}
	if (FD_ISSET(m_sock, readfds)) {
		int r = recv(m_sock, (void *)buf, sizeof(buf) - 2, 0);
		if (r <= 0) {
			Destroy();
			return false;
		}
		buf[r] = 0; //Add trailing zero;
		char *cur = buf;
		while (cur < &buf[r]) {
			int len = strlen(cur);
			char *next = &cur[len+1];
			bool last = (next > &buf[r]);
			if (!m_bHadError)
				json_stream_push(m_pstream, (const char *)cur);
			if (!last) {
				json_stream_reset(m_pstream);
				m_bHadError = false;
			}
			cur = next;	
		}
	}
	return true;
}
void CControlSocket::onNode(JSONNODE *node) {
	CConfigObject *c = new CJSONConfigObject(node);
	std::string cmd = c->getString("cmd");
	std::string id = c->getString("id");
	CConfigObject* arg = c->get("arg");
	if (cmd == "listPresets") {
		onListPresets(id);
	} else if (cmd == "listTransitions") {
		onListTransitions(id);
	} else if (cmd == "listStrips") {
		onListStrips(id);
	} else if (cmd == "change") {
		onChange(id, arg);
	} else {
		JSONNODE *root = json_new(JSON_NODE);
		json_push_back(root, json_new_a("cmd","error"));
		json_push_back(root, json_new_a("id", id.c_str()));
		json_push_back(root, json_new_a("arg", "Unknown command"));
		json_char *reply = json_write_formatted(root);
		json_delete(root);
		send(m_sock, reply,strlen(reply)+1,0);
		json_free(reply);
	}
	delete c;
}
void CControlSocket::onListPresets(std::string id) {
	JSONNODE *root = json_new(JSON_NODE);
	json_push_back(root, json_new_a("cmd","reply"));
	json_push_back(root, json_new_a("id", id.c_str()));
	JSONNODE *list = json_new(JSON_ARRAY);
	json_set_name(list,"arg");
	std::set<std::string> s = m_pOwner->GetPresets();
	for (std::set<std::string>::iterator i = s.begin(); i!=s.end(); i++) {
		json_push_back(list, json_new_a(NULL, i->c_str()));
	}
	json_push_back(root, list);
	json_char *reply = json_write_formatted(root);
	json_delete(root);
	send(m_sock, reply,strlen(reply)+1,0);
	json_free(reply);
}
void CControlSocket::onListTransitions(std::string id) {
	JSONNODE *root = json_new(JSON_NODE);
	json_push_back(root, json_new_a("cmd","reply"));
	json_push_back(root, json_new_a("id", id.c_str()));
	JSONNODE *list = json_new(JSON_ARRAY);
	json_set_name(list,"arg");
	std::set<std::string> s = m_pOwner->GetTransitions();
	for (std::set<std::string>::iterator i = s.begin(); i!=s.end(); i++) {
		json_push_back(list, json_new_a(NULL, i->c_str()));
	}
	json_push_back(root, list);
	json_char *reply = json_write_formatted(root);
	json_delete(root);
	send(m_sock, reply,strlen(reply)+1,0);
	json_free(reply);
}
void CControlSocket::onListStrips(std::string id) {
	JSONNODE *root = json_new(JSON_NODE);
	json_push_back(root, json_new_a("cmd","reply"));
	json_push_back(root, json_new_a("id", id.c_str()));
	JSONNODE *list = json_new(JSON_ARRAY);
	json_set_name(list,"arg");
	std::map<int, std::string> s = m_pOwner->GetStrips();
	for (std::map<int, std::string>::iterator i = s.begin(); i!=s.end(); i++) {
		JSONNODE *cur = json_new(JSON_NODE);
		json_push_back(cur, json_new_i("id", i->first));
		json_push_back(cur, json_new_a("name", i->second.c_str()));
		json_push_back(list, cur);
	}
	json_push_back(root, list);
	json_char *reply = json_write_formatted(root);
	json_delete(root);
	send(m_sock, reply,strlen(reply)+1,0);
	json_free(reply);
}
void CControlSocket::onChange(std::string id, CConfigObject *arg) {
	std::set<int> lStrips;
	CConfigObject *pStrips = arg->get("strips");
	for (int i=0; i<pStrips->length(); i++)
		lStrips.insert(pStrips->getInt(i));
	bool ret = m_pOwner->Change(lStrips, arg->getString("transition"), arg->getString("preset"));
	
	JSONNODE *root = json_new(JSON_NODE);
	json_push_back(root, json_new_a("cmd","reply"));
	json_push_back(root, json_new_a("id", id.c_str()));
	json_push_back(root, json_new_b("arg", ret));
	json_char *reply = json_write_formatted(root);
	json_delete(root);
	send(m_sock, reply,strlen(reply)+1,0);
	json_free(reply);
}

void CControlSocket::stream_callback(JSONNODE *node, void *pThis) {
	((CControlSocket *)pThis)->onNode(node);
}
void CControlSocket::error_callback(void *pThis) {
	((CControlSocket *)pThis)->onError();
}

void CControlSocket::onError() {
	m_bHadError = true;
}