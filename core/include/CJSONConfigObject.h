#ifndef CJSONCONFIGOBJECT_H
#define CJSONCONFIGOBJECT_H
#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <CConfigObject.h>
#include <libjson.h>

class CJSONConfigObject : public CConfigObject {
private:
	struct MaybeConfigObject {
		bool isobject;
		union {
			JSONNODE *node;
			CConfigObject *object;
		};
	};

	JSONNODE *m_node;
	std::map<std::string, MaybeConfigObject> m_objmap;
	std::vector<MaybeConfigObject> m_arrmap;
public:
	CJSONConfigObject(JSONNODE *node);
	~CJSONConfigObject();
	
	CConfigObject* get(std::string name);
	std::set<std::string> keys();
	CConfigObject* get(int index);
	int length();
	bool isObject();
	bool isArray();
	std::string asString();
	int asInt();
	double asDouble();
	bool asBool();
};
#endif//CJSONCONFIGOBJECT_H
