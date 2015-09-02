/*! @file
	@ingroup mysql
	@brief 定义数据库记录类
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
	/*! @brief 数据库记录类
		@ingroup mysql
	*/
	class Recordset:
		public NoCopy
	{
		friend class RecordsetPtr;
	public:
		virtual ~Recordset();

		//!获得记录
		virtual bool fetch()=0;

		//!关闭记录
		void close();

		//@{
		/*! @brief 输出参数
			@param val 输出参数
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

		//!得到记录行数
		unsigned long long getRows();
	protected:
		//! 构造函数
		Recordset(
			Connection & conn	//!< 相关连的数据库连接
			);

		//!设置字段数量
		void setFieldCount(
			WORD fieldCount		//!< 字段数量
			);

		//!获得字段数量
		WORD getFieldCount();

		//!获得最大字段数量
		WORD getFieldMaxCount();

		//!重置字段索引
		void resetFieldIndex();

		//!设置记录行数
		void setRows(
			unsigned long long rows	//!< 行数
			);

		void setColumnFormat(
			WORD column,				//!< 列索引
			const MYSQL_FIELD &field	//!< 列描述
			);
		ParameterValue	*	m_Values;			//!< 结果数组
	private:
		unsigned long long	m_rows;				//!< 行数
		WORD				m_fieldMaxCount;	//!< 最多字段数
		WORD				m_fieldCount;		//!< 当前字段数
		WORD				m_fieldIndex;		//!< 字段索引
		Connection &		m_conn;				//!< 相关连连接
	};

	/*! @brief 数据库记录指针类
		@ingroup mysql


		在析构时会自动调用Recordset::close方法。<br>
		RecordsetPrt 所持有的Recordset指针有可能为空，在数据库查询失败时，会发生变种情况。<br>
		所以，在调用 operator * 和 operator -> 时，应当先通过 operator bool() 查询所持有指针是否为空。<br>
		典型用法如下:<br>
<pre>
		RecordsetPrt rs = query->store();
		if(!rs)
		{
			//数据库错误
			...
			return false;
		}
		if(!rs->fetch())
		{
			//数据库查询成功，但没有返回记录
			...
			return false;
		}
*		*rs >> ..>> ...
		...;
		return true;
</pre><br>
		用为简化操作，可以调用fetchRow()方法,该方法仅当查询返回记录时才成功，但区分不出是数据库错误还是没有记录。<br>
		典型用法如下:<br>
<pre>
		RecordsetPrt rs = query->store();
		if(!rs.fetchRow())
		{
			//没有返回记录
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
		//!构造函数
		RecordsetPtr(
			Recordset *rs		//!< 数据库记录对象
			);

		//!构造函数
		RecordsetPtr(const RecordsetPtr & other);

		~RecordsetPtr();

		//! * 操作符
		Recordset & operator *();

		//! -> 操作符
		Recordset * operator ->();

		/*! @brief bool转换操作符
			@return 如果取到记录返回true，否则返回false
		*/
		operator bool() const;

		//!获得记录
		bool fetchRow();
	private:
		RecordsetPtr & operator = (const RecordsetPtr & other);

		mutable Recordset *	m_rs;	//!< 数据库记录对象
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
