/*! @file
	@ingroup utils
	@brief ��������������к���
*/
#ifndef __ZOIC_RANDOM_H
#define __ZOIC_RANDOM_H

#include <vector>
#include "Types.h"

namespace Zoic
{
	const BYTE MAX_RANGE_EXP = 9;
	extern DWORD _exp10[MAX_RANGE_EXP+1];

	/*! @brief ����������к���
		@ingroup utils
	*/
	template<class T>
	int randomHit(
		const std::vector<T>& setting,	//!< �������,��ŵ���һ�����ֵ
		BYTE rangeExp					//!< ���ʾ���,10��rangeExp�ݵĸ���ֵ�൱��100%
	)
	{
		if(rangeExp>MAX_RANGE_EXP)
		{
			return -1;
		}
		const DWORD RATE_RANGE = _exp10[rangeExp];	//������ʵ����Χ( 0 ~ RATE_RANGE-1 )
		const DWORD TEST_RANGE = 100;		//ÿ����������ȡֵ��Χ
		//ע�⣬����ֵ������ѭ�������ã�
		//RATE_RANGE��TEST_RANGE�Ĺ�ϵ����ΪRATE_RANGE = (TEST_RANGE��x�η�)*y�Ĺ�ϵ�����ұ���y<=TEST_RANGE���������漰����ֵ����Ϊ0
		//���y����1����MAX_STEPS = x+1,���� MAX_STEPS = x ��
		DWORD curPrecision = RATE_RANGE;		//��ǰ��������Ѵﵽ�ľ��ȷ�Χ
		DWORD curTestRange = TEST_RANGE;
		DWORD curTestLowerBound = 0;			//���ݵ�ǰ�����������ȡ�õ�ֵ�Ŀ��ܷ�Χ�����ޣ��������ޣ�
		DWORD curTestUpperBound = RATE_RANGE;	//���ݵ�ǰ�����������ȡ�õ�ֵ�Ŀ��ܷ�Χ�����ޣ����������ޣ�

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

	/*! @brief ����[a-b]֮��������
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
