#include "CConfigObject.h"

CNullConfigObject *CNullConfigObject::s_instance = NULL;

CConfigObject::~CConfigObject() {

}

bool CConfigObject::isNull() {
	return false;
}

std::string CConfigObject::getString(std::string name, std::string def) {
	CConfigObject *child = get(name);
	if (!child)
		return def;
	return child->asString();
}
std::string CConfigObject::getString(int index, std::string def) {
	CConfigObject *child = get(index);
	if (child->isNull())
		return def;
	return child->asString();
}
int CConfigObject::getInt(std::string name, int def) {
	CConfigObject *child = get(name);
	if (child->isNull())
		return def;
	return child->asInt();
}
int CConfigObject::getInt(int index, int def) {
	CConfigObject *child = get(index);
	if (child->isNull())
		return def;
	return child->asInt();
}
double CConfigObject::getDouble(std::string name, double def) {
	CConfigObject *child = get(name);
	if (child->isNull())
		return def;
	return child->asDouble();
}
double CConfigObject::getDouble(int index, double def) {
	CConfigObject *child = get(index);
	if (child->isNull())
		return def;
	return child->asDouble();
}
bool CConfigObject::getBool(std::string name, bool def) {
	CConfigObject *child = get(name);
	if (child->isNull())
		return def;
	return child->asBool();
}
bool CConfigObject::getBool(int index, bool def) {
	CConfigObject *child = get(index);
	if (child->isNull())
		return def;
	return child->asBool();
}


CNullConfigObject::CNullConfigObject() {
	
}
CConfigObject *CNullConfigObject::get() {
	if (s_instance)
		return s_instance;
	return (s_instance = new CNullConfigObject());
}
CConfigObject* CNullConfigObject::get(std::string name) {
	return this;
}
std::set<std::string> CNullConfigObject::keys() {
	return std::set<std::string>();
}
CConfigObject* CNullConfigObject::get(int index) {
	return this;
}
int CNullConfigObject::length() {
	return 0;
}
bool CNullConfigObject::isObject() {
	return false;
}
bool CNullConfigObject::isArray() {
	return false;
}
std::string CNullConfigObject::asString() {
	return "";
}
int CNullConfigObject::asInt() {
	return 0;
}
double CNullConfigObject::asDouble() {
	return 0.0;
}
bool CNullConfigObject::asBool() {
	return false;
}
bool CNullConfigObject::isNull() {
	return true;
}