#ifndef __COMMONMESSAGESTREAM_H
#define __COMMONMESSAGESTREAM_H

#include <Zoic/include/Message.h>
#include "CommonTypes.h"

template<typename HANDLE_T,typename SERIAL_T>
inline Zoic::Message & operator << ( Zoic::Message &m,const T_SERIALHANDLE<HANDLE_T,SERIAL_T> & p )
{
	m << p.handle;
	m << p.serial;
	return m;
}
template<typename HANDLE_T,typename SERIAL_T>
inline Zoic::Message & operator >> ( Zoic::Message &m,T_SERIALHANDLE<HANDLE_T,SERIAL_T> &p )
{
	m >> p.handle;
	m >> p.serial;
	return m;
}

template <class T>
Zoic::Message & operator << ( Zoic::Message &m,const T_POS2D<T> & p )
{
	m << p.x;
	m << p.y;
	return m;
}
template <class T>
Zoic::Message & operator >> ( Zoic::Message &m,T_POS2D<T> &p )
{
	m >> p.x;
	m >> p.y;
	return m;
}


template <class T>
inline Zoic::Message & operator << ( Zoic::Message &m,const T_GRADS<T> & p )
{
	m << p.dx;
	m << p.dy;
	return m;
}
template <class T>
inline Zoic::Message & operator >> ( Zoic::Message &m, T_GRADS<T> &p )
{
	m >> p.dx;
	m >> p.dy;
	return m;
}

#endif
