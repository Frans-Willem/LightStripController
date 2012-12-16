#ifndef ISTRIPCOMMAND_H
#define ISTRIPCOMMAND_H
class IGenerator;
class CStripThread;
class IStripCommand {
public:
	virtual void Execute(CStripThread *pThread, IGenerator **ppGenerator) = 0;
};
#endif//ISTRIPCOMMAND_H
