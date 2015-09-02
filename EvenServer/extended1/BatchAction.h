#ifndef __BANCHACTION_H
#define __BANCHACTION_H

template<class CONTEXT_T>
class BatchAction
{
public:
	virtual void doAction(CONTEXT_T&) = 0;
};

template<class CONTEXT_T>
class BatchCheck
{
public:
	virtual bool doCheck(CONTEXT_T&) = 0;
};

template<class CONTEXT_T>
class BatchOperation
{
public:
	virtual bool doOperation(CONTEXT_T&) = 0;
};

#endif
