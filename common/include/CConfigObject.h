#ifndef CCONFIGOBJECT_H
#define CCONFIGOBJECT_H
#include <string>
#include <set>

class CConfigObject {
public:
	virtual ~CConfigObject();
	virtual CConfigObject* get(std::string name)=0;
	virtual std::set<std::string> keys()=0;
	virtual CConfigObject* get(int index)=0;
	virtual int length()=0;
	virtual bool isObject()=0;
	virtual bool isArray()=0;
	virtual std::string asString()=0;
	virtual int asInt()=0;
	virtual double asDouble()=0;
	virtual bool asBool()=0;
	virtual bool isNull();
	
	virtual std::string getString(std::string name, std::string def="");
	virtual std::string getString(int index, std::string def="");
	virtual int getInt(std::string name, int def=0);
	virtual int getInt(int index, int def=0);
	virtual double getDouble(std::string name, double def=0.0);
	virtual double getDouble(int index, double def=0.0);
	virtual bool getBool(std::string name, bool def=false);
	virtual bool getBool(int index, bool def=false);
};

class CNullConfigObject : public CConfigObject {
private:
	CNullConfigObject();
	static CNullConfigObject *s_instance;
public:
	static CConfigObject *get();
	std::set<std::string> keys();
	CConfigObject* get(std::string name);
	CConfigObject* get(int index);
	int length();
	bool isObject();
	bool isArray();
	std::string asString();
	int asInt();
	double asDouble();
	bool asBool();
	bool isNull();
};
#endif//ICONFIGOBJECT_H
