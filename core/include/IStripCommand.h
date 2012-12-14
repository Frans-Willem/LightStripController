#ifndef ISTRIPCOMMAND_H
#define ISTRIPCOMMAND_H
class IGenerator;
struct LightStripConfig;
class IStripCommand {
public:
	virtual void Execute(LightStripConfig *pConfig, IGenerator **ppGenerator) = 0;
};
#endif//ISTRIPCOMMAND_H
