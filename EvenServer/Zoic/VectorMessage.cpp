#include "include/VectorMessage.h"

using namespace Zoic;

VectorPacker::VectorPacker(void *packet,void *vector,VectorPacker::PACK_FUNC func)
{
	m_packet = packet;
	m_vector = vector;
	m_func = func;
}

void VectorPacker::pack(Message &msgSend) const
{
	(*m_func)( msgSend,m_packet );
}

void * VectorPacker::getVectorPointer() const
{
	return m_vector;
}
