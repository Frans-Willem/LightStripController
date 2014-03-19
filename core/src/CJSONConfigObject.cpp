#include "CJSONConfigObject.h"
#include <iostream>

CJSONConfigObject::CJSONConfigObject(JSONNODE *node) :
	m_node(node)
{
	if (json_type(node) == JSON_NODE) {
		for (JSONNODE_ITERATOR i = json_begin(node); i != json_end(node); ++i) {
			MaybeConfigObject o;
			o.isobject = false;
			o.node = *i;
			json_char *node_name = json_name(*i);
			m_objmap.insert(std::pair<std::string,MaybeConfigObject>(node_name, o));
			json_free(node_name);
		}
	}
	if (json_type(node) == JSON_ARRAY) {
		for (JSONNODE_ITERATOR i = json_begin(node); i != json_end(node); ++i) {
			MaybeConfigObject o;
			o.isobject = false;
			o.node = *i;
			m_arrmap.push_back(o);
		}
	}
}
CJSONConfigObject::~CJSONConfigObject() {
	for (std::map<std::string, MaybeConfigObject>::iterator i = m_objmap.begin(); i != m_objmap.end(); i++) {
		if (i->second.isobject)
			delete i->second.object;
	}
	for (std::vector<MaybeConfigObject>::iterator i = m_arrmap.begin(); i != m_arrmap.end(); i++) {
		if (i->isobject)
			delete i->object;
	}
}
	
CConfigObject* CJSONConfigObject::get(std::string name) {
	std::map<std::string,MaybeConfigObject>::iterator f = m_objmap.find(name);
	if (f == m_objmap.end())
		return CNullConfigObject::get();
	if (!f->second.isobject) {
		f->second.object = new CJSONConfigObject(f->second.node);
		f->second.isobject = true;
	}
	return f->second.object;
}
std::set<std::string> CJSONConfigObject::keys() {
	std::set<std::string> keys;
	for (std::map<std::string,MaybeConfigObject>::iterator i=m_objmap.begin(); i!=m_objmap.end(); i++)
		keys.insert(i->first);
	return keys;
}
CConfigObject* CJSONConfigObject::get(int index) {
	if (index >= m_arrmap.size())
		return CNullConfigObject::get();
	MaybeConfigObject& f = m_arrmap[index];
	if (!f.isobject) {
		f.object = new CJSONConfigObject(f.node);
		f.isobject = true;
	}
	return f.object;
}
int CJSONConfigObject::length() {
	if (json_type(m_node) == JSON_ARRAY)
		return m_arrmap.size();
	return 0;
}
bool CJSONConfigObject::isObject() {
	return (json_type(m_node) == JSON_NODE);
}
bool CJSONConfigObject::isArray() {
	return (json_type(m_node) == JSON_ARRAY);
}
std::string CJSONConfigObject::asString() {
	json_char *ret = json_as_string(m_node);
	if (ret == NULL)
		return "";
	std::string strRet(ret);
	json_free(ret);
	return strRet;
}
int CJSONConfigObject::asInt() {
	return json_as_int(m_node);
}
double CJSONConfigObject::asDouble() {
	return json_as_float(m_node);
}

bool CJSONConfigObject::asBool() {
	return json_as_bool(m_node);
}

