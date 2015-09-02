/*! @file
	@ingroup utils
	@brief ��������ģ��
*/

#ifndef __ZOIC_BOND_H
#define __ZOIC_BOND_H

#include "Types.h"

namespace Zoic
{
	/*! @brief ����ģ��
		@ingroup utils
		@param IMPL ����ľ���ʵ����,�Ӹ�ģ��̳�
		@param A �����A��
		@param B �����B��
	*/
	template <class IMPL,class A,class B>
	class Bond
	{
	public:
		//! ��������
		static bool buildBond(
			A * a,			//!< A ����ָ��
			B * b			//!< B ����ָ��
			);

		//! ��������
		static bool breakBond(
			A * a,			//!< A ����ָ��
			B * b			//!< B ����ָ��
			);

		//! ��������
		static bool breakBond(
			B * b,			//!< B ����ָ��
			A * a			//!< A ����ָ��
			);

		//! ���������¼�
		static void onBuildBond(
			A * a,			//!< A ����ָ��
			B * b			//!< B ����ָ��
			);

		//! ���������¼�
		static void onBreakBond(
			A * a,			//!< A ����ָ��
			B * b			//!< B ����ָ��
			);
	};

	/*! @breif ��ָ��İ�װ��
		@ingroup utils
		@param IMPL	����ʵ����
		@param SELF	�̳б�������
		@param PARTER �Է�����
		@param T	��װ��ָ������
	*/
	template <class IMPL,class SELF,class PARTER>
	class Bonder_Pointer
	{
	public:
		//! ���캯��
		Bonder_Pointer();

		//! ��Ӻ�����
		bool insertParter(
			PARTER * parter			//!< ������ָ��
			);

		//! �ƶӺ�����
		bool removeParter(
			PARTER * parter			//!< ������ָ��
			);

		//! �������
		void breakBond();

		//! ��ú�����
		PARTER * getParter() const;
	private:
		PARTER * m_parter;	//!< ��װ��ֵ
	};

	/*! @brief һ��һ����ģ��
		@ingroup utils
		@param IMPL ����ľ���ʵ����,�Ӹ�ģ��̳�
		@param A �����A��
		@param B �����B��
	*/
	template <class IMPL,class A,class B>
	class Bond_1_1:
		public Bond<IMPL,A,B>
	{
	public:
		//! ����A��Ĳ���
		class A_BONDER:
			public Bonder_Pointer<IMPL,A,B>
		{
		};

		//! ����B��Ĳ���
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
