#include "IGenerator.h"

IGenerator::~IGenerator() {

}

IGenerator::IGenerator() :
	m_nRef(1)
{
}

void IGenerator::AddRef() {
	__sync_add_and_fetch(&m_nRef, 1);
}
void IGenerator::Release() {
	if (__sync_sub_and_fetch(&m_nRef, 1) == 0)
		delete this;
}