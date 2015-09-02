#ifndef __NUMERICEXTEND_H
#define __NUMERICEXTEND_H

////////////////////  ----------------  Declare Begin ----------------  /////////////////////
//MaskBoundChecker
//掩码的一些方案,通过给出一个掩码的取值边界(静态编译,>=0且<MASK_BOUND):
//1.检查这个值是否是2的幂次方的正确掩码边界(不合法将发生编译错误)
//2.获得有效bit数(VALID_BIT_COUNT)
//3.获得有效值域掩码(FULL_MASK_VALUE)
template<unsigned int MASK_BOUND> class MaskBoundChecker;

//IntegerPower
//指数的静态编译策略
//1.获得这个底数获得指数的幂(POWER)
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
