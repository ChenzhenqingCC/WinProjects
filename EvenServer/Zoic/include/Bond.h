/*! @file
	@ingroup utils
	@brief 定义联结模板
*/

#ifndef __ZOIC_BOND_H
#define __ZOIC_BOND_H

#include "Types.h"

namespace Zoic
{
	/*! @brief 联结模板
		@ingroup utils
		@param IMPL 联结的具体实现类,从该模块继承
		@param A 联结的A类
		@param B 联结的B类
	*/
	template <class IMPL,class A,class B>
	class Bond
	{
	public:
		//! 建立联结
		static bool buildBond(
			A * a,			//!< A 对象指针
			B * b			//!< B 对象指针
			);

		//! 打破联结
		static bool breakBond(
			A * a,			//!< A 对象指针
			B * b			//!< B 对象指针
			);

		//! 打破联结
		static bool breakBond(
			B * b,			//!< B 对象指针
			A * a			//!< A 对象指针
			);

		//! 建立联结事件
		static void onBuildBond(
			A * a,			//!< A 对象指针
			B * b			//!< B 对象指针
			);

		//! 打破联结事件
		static void onBreakBond(
			A * a,			//!< A 对象指针
			B * b			//!< B 对象指针
			);
	};

	/*! @breif 对指针的包装类
		@ingroup utils
		@param IMPL	联结实现类
		@param SELF	继承本结点的类
		@param PARTER 对方的类
		@param T	封装的指针类型
	*/
	template <class IMPL,class SELF,class PARTER>
	class Bonder_Pointer
	{
	public:
		//! 构造函数
		Bonder_Pointer();

		//! 添加合作者
		bool insertParter(
			PARTER * parter			//!< 合作者指针
			);

		//! 移队合作者
		bool removeParter(
			PARTER * parter			//!< 合作者指针
			);

		//! 清除联结
		void breakBond();

		//! 获得合作者
		PARTER * getParter() const;
	private:
		PARTER * m_parter;	//!< 封装的值
	};

	/*! @brief 一对一联结模板
		@ingroup utils
		@param IMPL 联结的具体实现类,从该模块继承
		@param A 联结的A类
		@param B 联结的B类
	*/
	template <class IMPL,class A,class B>
	class Bond_1_1:
		public Bond<IMPL,A,B>
	{
	public:
		//! 用于A类的部分
		class A_BONDER:
			public Bonder_Pointer<IMPL,A,B>
		{
		};

		//! 用于B类的部分
		class B_BONDER:
			public Bonder_Pointer<IMPL,B,A>
		{
		};
	};

	template <class IMPL,class A,class B>
	bool Bond<IMPL,A,B>::buildBond(A *a,B *b)
	{
		if(!static_cast<typename IMPL::A_BONDER *>(a)->insertParter(b))
		{
			return false;
		}
		if(!static_cast<typename IMPL::B_BONDER *>(b)->insertParter(a))
		{
			static_cast<typename IMPL::A_BONDER *>(a)->removeParter(b);
			return false;
		}
		//static_cast<typename IMPL::A_BONDER *>(a)->insertParter(b);
		IMPL::onBuildBond(a,b);
		return true;
	}

	template <class IMPL,class A,class B>
	bool Bond<IMPL,A,B>::breakBond(A *a,B *b)
	{
		if(!static_cast<typename IMPL::A_BONDER *>(a)->removeParter(b))
		{
			return false;
		}
		if(!static_cast<typename IMPL::B_BONDER *>(b)->removeParter(a))
		{
			return false;
		}
		IMPL::onBreakBond(a,b);
		return true;
	}

	template <class IMPL,class A,class B>
	bool Bond<IMPL,A,B>::breakBond(B * b,A * a)
	{
		IMPL::breakBond(a,b);
		return true;
	}

	template <class IMPL,class A,class B>
	void Bond<IMPL,A,B>::onBuildBond(A *a,B *b)
	{
	}

	template <class IMPL,class A,class B>
	void Bond<IMPL,A,B>::onBreakBond(A *a,B *b)
	{
	}

	template <class IMPL,class SELF,class PARTER>
	Bonder_Pointer<IMPL,SELF,PARTER>::Bonder_Pointer():
		m_parter(NULL)
	{
	}

	template <class IMPL,class SELF,class PARTER>
	bool Bonder_Pointer<IMPL,SELF,PARTER>::insertParter(PARTER * parter)
	{
		ASSERT(m_parter==NULL);
		m_parter = parter;
		return true;
	}

	template <class IMPL,class SELF,class PARTER>
	bool Bonder_Pointer<IMPL,SELF,PARTER>::removeParter(PARTER * parter)
	{
		ASSERT(m_parter==parter);
		m_parter = NULL;
		return true;
	}

	template <class IMPL,class SELF,class PARTER>
	void Bonder_Pointer<IMPL,SELF,PARTER>::breakBond()
	{
		if(m_parter)
		{
			IMPL::breakBond(static_cast<SELF *>(this),m_parter);
		}
	}

	template <class IMPL,class SELF,class PARTER>
	PARTER * Bonder_Pointer<IMPL,SELF,PARTER>::getParter() const
	{
		return m_parter;
	}
}

#endif
