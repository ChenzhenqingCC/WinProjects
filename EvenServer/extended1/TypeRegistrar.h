#ifndef __TYPEREGISTRAR_H
#define __TYPEREGISTRAR_H

#include "BaseTypes.h"

template<class T,BYTE TYPE_BOUND>
class TypeRegistrar
{
public:
	TypeRegistrar();
	bool isRegistered( BYTE type ) const;
	void doRegister( BYTE type,bool isRegister = true );
private:
	bool TYPE_BOOK[TYPE_BOUND];
};

template<class T,BYTE TYPE_BOUND>
TypeRegistrar<T,TYPE_BOUND>::TypeRegistrar()
{
	memset(TYPE_BOOK,false,sizeof(TYPE_BOOK) );
}

template<class T,BYTE TYPE_BOUND>
void TypeRegistrar<T,TYPE_BOUND>::doRegister( BYTE type,bool isRegister )
{
	ASSERT(type < TYPE_BOUND);
	TYPE_BOOK[type] = isRegister;
}

template<class T,BYTE TYPE_BOUND>
bool TypeRegistrar<T,TYPE_BOUND>::isRegistered( BYTE type ) const
{
	ASSERT(type < TYPE_BOUND);
	return TYPE_BOOK[type];
}

#endif
