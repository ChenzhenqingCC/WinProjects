#ifndef __NUMERICEXTEND_H
#define __NUMERICEXTEND_H

////////////////////  ----------------  Declare Begin ----------------  /////////////////////
//MaskBoundChecker
//�����һЩ����,ͨ������һ�������ȡֵ�߽�(��̬����,>=0��<MASK_BOUND):
//1.������ֵ�Ƿ���2���ݴη�����ȷ����߽�(���Ϸ��������������)
//2.�����Чbit��(VALID_BIT_COUNT)
//3.�����Чֵ������(FULL_MASK_VALUE)
template<unsigned int MASK_BOUND> class MaskBoundChecker;

//IntegerPower
//ָ���ľ�̬�������
//1.�������������ָ������(POWER)
template<unsigned int BASE,unsigned char EXPONENT> class IntegerPower;


////////////////////  ----------------  Declare End ----------------  /////////////////////

////////////////////  ----------------  MaskBoundChecker Implement Begin  ----------------  /////////////////////
template<> class MaskBoundChecker<(unsigned int)0>{};

template<>
class MaskBoundChecker<(unsigned int)1>
{
protected:
	static const unsigned char Error_MaskBoundChecker_Template_Value_Must_Be_Powers_Of_2 = 0;
public:
	static const unsigned char VALID_BIT_COUNT = 0;
	static const unsigned int FULL_MASK_VALUE = 0;
};

template<unsigned int MASK_BOUND>
class MaskBoundChecker
	:protected MaskBoundChecker<(MASK_BOUND>>1)>
{
protected:
	typedef MaskBoundChecker< (MASK_BOUND>>1)+MaskBoundChecker<1-(MASK_BOUND&1)>::Error_MaskBoundChecker_Template_Value_Must_Be_Powers_Of_2> LowerChecker;
public:
	static const unsigned char VALID_BIT_COUNT = LowerChecker::VALID_BIT_COUNT + 1;
	static const unsigned int FULL_MASK_VALUE = MASK_BOUND - 1;
};
////////////////////  ----------------  MaskBoundChecker Implement End  ----------------  /////////////////////

////////////////////  ----------------  ExponentAssistant Implement Begin  ----------------  /////////////////////
template<unsigned int BASE>
class IntegerPower<BASE,0>
{
public:
	static const unsigned int POWER = 1;
};

template<unsigned int BASE,unsigned char EXPONENT>
class IntegerPower
{
public:
	static const unsigned int POWER = BASE*IntegerPower<BASE,EXPONENT-1>::POWER;
};
////////////////////  ----------------  ExponentAssistant Implement End  ----------------  /////////////////////

#endif
