/*! @file
	@ingroup utils
	@brief 定义随机设置命中函数
*/
#ifndef __ZOIC_RANDOM_H
#define __ZOIC_RANDOM_H

#include <vector>
#include "Types.h"

namespace Zoic
{
	const BYTE MAX_RANGE_EXP = 9;
	extern DWORD _exp10[MAX_RANGE_EXP+1];

	/*! @brief 随机设置命中函数
		@ingroup utils
	*/
	template<class T>
	int randomHit(
		const std::vector<T>& setting,	//!< 随机设置,存放的是一组概率值
		BYTE rangeExp					//!< 概率精度,10的rangeExp幂的概率值相当于100%
	)
	{
		if(rangeExp>MAX_RANGE_EXP)
		{
			return -1;
		}
		const DWORD RATE_RANGE = _exp10[rangeExp];	//随机几率的最大范围( 0 ~ RATE_RANGE-1 )
		const DWORD TEST_RANGE = 100;		//每次随机运算的取值范围
		//注意，上述值必须遵循以下配置：
		//RATE_RANGE和TEST_RANGE的关系必须为RATE_RANGE = (TEST_RANGE的x次方)*y的关系，而且必须y<=TEST_RANGE，且所有涉及到的值都不为0
		//如果y大于1，则MAX_STEPS = x+1,否则 MAX_STEPS = x 。
		DWORD curPrecision = RATE_RANGE;		//当前随机测试已达到的精度范围
		DWORD curTestRange = TEST_RANGE;
		DWORD curTestLowerBound = 0;			//根据当前随机测试所能取得的值的可能范围的下限（包括下限）
		DWORD curTestUpperBound = RATE_RANGE;	//根据当前随机测试所能取得的值的可能范围的上限（不包括上限）

		int settingSize = static_cast<int>( setting.size() );
		for( int index = 0 ; index<settingSize; ++index )
		{
			const T& prob = setting[index];
			do
			{
				if( curTestUpperBound <= prob)
				{
					return index;
				}
				else if( curTestLowerBound >= prob )
				{
					curTestLowerBound -= prob;
					curTestUpperBound -= prob;
					break;
				}
				if( rangeExp <= 0)
				{
					break;
				}
				if(curPrecision>=curTestRange)
				{
					curPrecision/=curTestRange;
					rangeExp -= 2;
				}
				else
				{
					curPrecision = 1;
					curTestRange = curPrecision;
					rangeExp = 0;
				}
				curTestLowerBound += rand()%curTestRange * curPrecision;
				curTestUpperBound = curTestLowerBound + curPrecision;
			}while( 1 );
		}
		return -1;
	}

	/*! @brief 产生[a-b]之间的随机数
		@ingroup utils
	*/
	template <class T>
	T random(T a, T b)
	{
		T minValue = (a < b) ? a : b;
		T maxValue = (a > b) ? a : b;
		T difference = maxValue - minValue;
		return static_cast<T>(rand()/(RAND_MAX+1.0)*(difference+1)+minValue);
	}
}
#endif
