
#include "Dumping.h"

#define DUMPIDTOFILE

void Dump::DumpClassIds()
{
#ifdef DUMPIDTOFILE
#endif
	ClientClass* cClass = m_pClient->GetAllClasses();
	while (cClass)
	{
		cClass = cClass->m_pNext;
	}
}