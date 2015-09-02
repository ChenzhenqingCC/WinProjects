/*! @file
	@ingroup mysql
	@brief �������ݿ��¼��
*/

#ifndef _ZOIC_RECORDSET_H
#define _ZOIC_RECORDSET_H
#include <string>
#include "Types.h"
#include <Zoic/mysql/include/mysql.h>
#include "ParameterValue.h"
#include "NoCopy.h"
#include "TinyBinary.h"

namespace Zoic
{
	template <const WORD LEN> class String;
	class RecordsetPtr;
	class Connection;;
	/*! @brief ���ݿ��¼��
		@ingroup mysql
	*/
	class Recordset:
		public NoCopy
	{
		friend class RecordsetPtr;
	public:
		virtual ~Recordset();

		//!��ü�¼
		virtual bool fetch()=0;

		//!�رռ�¼
		void close();

		//@{
		/*! @brief �������
			@param val �������
		*/
		Recordset & operator >> (char & val);
		Recordset & operator >> (unsigned char & val);
		Recordset & operator >> (short & val);
		Recordset & operator >> (unsigned short & val);
		Recordset & operator >> (int & val);
		Recordset & operator >> (unsigned int & val);
		Recordset & operator >> (long & val);
		Recordset & operator >> (unsigned long & val);
		Recordset & operator >> (long long & val);
		Recordset & operator >> (unsigned long long & val);
		Recordset & operator >> (std::string &val);
		Recordset & operator >> (float &val);
		Recordset & operator >> (double &val);
		Recordset & operator >> (Date &val);
		Recordset & operator >> (Time &val);
		Recordset & operator >> (DateTime &val);
		Recordset & operator >> (TinyBinary &val);
		//@}

		//!�õ���¼����
		unsigned long long getRows();
	protected:
		//! ���캯��
		Recordset(
			Connection & conn	//!< ����������ݿ�����
			);

		//!�����ֶ�����
		void setFieldCount(
			WORD fieldCount		//!< �ֶ�����
			);

		//!����ֶ�����
		WORD getFieldCount();

		//!�������ֶ�����
		WORD getFieldMaxCount();

		//!�����ֶ�����
		void resetFieldIndex();

		//!���ü�¼����
		void setRows(
			unsigned long long rows	//!< ����
			);

		void setColumnFormat(
			WORD column,				//!< ������
			const MYSQL_FIELD &field	//!< ������
			);
		ParameterValue	*	m_Values;			//!< �������
	private:
		unsigned long long	m_rows;				//!< ����
		WORD				m_fieldMaxCount;	//!< ����ֶ���
		WORD				m_fieldCount;		//!< ��ǰ�ֶ���
		WORD				m_fieldIndex;		//!< �ֶ�����
		Connection &		m_conn;				//!< ���������
	};

	/*! @brief ���ݿ��¼ָ����
		@ingroup mysql


		������ʱ���Զ�����Recordset::close������<br>
		RecordsetPrt �����е�Recordsetָ���п���Ϊ�գ������ݿ��ѯʧ��ʱ���ᷢ�����������<br>
		���ԣ��ڵ��� operator * �� operator -> ʱ��Ӧ����ͨ�� operator bool() ��ѯ������ָ���Ƿ�Ϊ�ա�<br>
		�����÷�����:<br>
<pre>
		RecordsetPrt rs = query->store();
		if(!rs)
		{
			//���ݿ����
			...
			return false;
		}
		if(!rs->fetch())
		{
			//���ݿ��ѯ�ɹ�����û�з��ؼ�¼
			...
			return false;
		}
*		*rs >> ..>> ...
		...;
		return true;
</pre><br>
		��Ϊ�򻯲��������Ե���fetchRow()����,�÷���������ѯ���ؼ�¼ʱ�ųɹ��������ֲ��������ݿ������û�м�¼��<br>
		�����÷�����:<br>
<pre>
		RecordsetPrt rs = query->store();
		if(!rs.fetchRow())
		{
			//û�з��ؼ�¼
			...
			return false;
		}
*		*rs >> ..>> ...
		...;
		return true;
</pre><br>
	*/
	class RecordsetPtr
	{
	public:
		//!���캯��
		RecordsetPtr(
			Recordset *rs		//!< ���ݿ��¼����
			);

		//!���캯��
		RecordsetPtr(const RecordsetPtr & other);

		~RecordsetPtr();

		//! * ������
		Recordset & operator *();

		//! -> ������
		Recordset * operator ->();

		/*! @brief boolת��������
			@return ���ȡ����¼����true�����򷵻�false
		*/
		operator bool() const;

		//!��ü�¼
		bool fetchRow();
	private:
		RecordsetPtr & operator = (const RecordsetPtr & other);

		mutable Recordset *	m_rs;	//!< ���ݿ��¼����
	};

	template <const WORD LEN>
	Recordset & operator >> (Recordset &rs,String<LEN> & str)
	{
		std::string s;
		rs >> s;
		strlcpy(reinterpret_cast<char *>(&str),s.c_str(),LEN+1);
		return rs;
	}

	inline Recordset & Recordset::operator >> (char & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (unsigned char & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (short & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
			return *this;
	}

	inline Recordset & Recordset::operator >> (unsigned short & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (int & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (unsigned int & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (long & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (unsigned long & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (long long & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (unsigned long long & val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (std::string &val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (float &val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (double &val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (Date &val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (Time &val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (DateTime &val)
	{
		m_Values[m_fieldIndex++].getValue(val);
		return *this;
	}

	inline Recordset & Recordset::operator >> (TinyBinary &val)
	{
		std::string _val;
		m_Values[m_fieldIndex++].getValue(_val);		
		//val << _val;
		val.setData((WORD)_val.length(),(const BYTE*)_val.c_str());
		return *this;
	}

	inline unsigned long long Recordset::getRows()
	{
		return m_rows;
	}

	inline void Recordset::setFieldCount(WORD fieldCount)
	{
		if(fieldCount>m_fieldMaxCount)
		{
			if(m_Values)
			{
			delete [] m_Values;
			}
			m_fieldMaxCount = fieldCount;
			m_Values=new ParameterValue[fieldCount];
			memset(m_Values,0,sizeof(ParameterValue)*fieldCount);
		}
		m_fieldCount = fieldCount;
	}

	inline WORD Recordset::getFieldCount()
	{
		return m_fieldCount;
	}

	inline WORD Recordset::getFieldMaxCount()
	{
		return m_fieldMaxCount;
	}

	inline void Recordset::resetFieldIndex()
	{
		m_fieldIndex=0;
	}

	inline void Recordset::setRows(unsigned long long rows)
	{
		m_rows=rows;
	}

	inline RecordsetPtr::RecordsetPtr(Recordset *rs):
		m_rs(rs)
	{
	}

	inline RecordsetPtr::~RecordsetPtr()
	{
		if(m_rs)
		{
			m_rs->close();
		}
	}

	inline Recordset & RecordsetPtr::operator *()
	{
		return *m_rs;
	}

	inline Recordset * RecordsetPtr::operator ->()
	{
		return m_rs;
	}

	inline RecordsetPtr::operator bool() const
	{
		return m_rs ? true : false;
	}

	inline RecordsetPtr::RecordsetPtr(const RecordsetPtr & other)
	{
		m_rs=other.m_rs;
		other.m_rs=NULL;
	}

	inline bool RecordsetPtr::fetchRow()
	{
		if(m_rs)
		{
			return m_rs->fetch();
		}
		else
		{
			return false;
		}
	}
}

#endif
