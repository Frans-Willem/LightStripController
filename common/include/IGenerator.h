#ifndef IGENERATOR_H
#define IGENERATOR_H
#include <vector>
#include "CColor.h"

class IGenerator {
public:
	virtual ~IGenerator();
	std::vector<CColor> Generate();
};
#endif//IGENERATOR_H
