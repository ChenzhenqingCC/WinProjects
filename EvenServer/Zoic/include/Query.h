/*! @file
	@ingroup mysql
	@brief �������ݿ��ѯ��
*/

#ifndef _ZOIC_QUERY_H
#define _ZOIC_QUERY_H
#include <string>
#include <vector>
#include "Types.h"
#include "Recordset.h"
#include "Connection.h"
#include "ParameterValue.h"
#include "NoCopy.h"

namespace Zoic
{
	class Connection;
	class Recordset;
	/*! @brief ���ݿ��ѯ��
		@ingroup mysql	
	*/
	class Query:
		public NoCopy
	{
		friend class Connection;
	public:
		//!��������
		struct ParameterType
		{
			std::string name;	//!< ��������
			enum TYPE
			{
				T_CHAR,
				T_VARCHAR,
				T_BINARY,
				T_VARBINARY,
				T_TINYBLOB,
				T_TINYTEXT,
				T_BLOB,
				T_TEXT,
				T_MEDIUMBLOB,
				T_MEDIUMTEXT,
				T_LONGBLOB,
				T_LONGTEXT,
				T_BIT,
				T_BOOL,
				T_TINYINT,
				T_SMALLINT,
				T_MEDIUMINT,
				T_INT,
				T_BIGINT,
				T_FLOAT,
				T_DOUBLE,
				T_DECIMAL,
				T_DATE,
				T_TIME,
				T_DATETIME,
				T_TIMESTAMP
			} type;		//!< ��������
			bool sign;	//!< ���ű��(true:�з���;false:�޷���)
		};

		//!��ִ��SQL���
		virtual bool execute()=0;

		//!ʹ��Use�ķ�ʽ��������
		virtual Recordset * use()=0;

		//!ʹ��store�ķ�ʽ��������
		virtual Recordset * store()=0;

		//!�õ�Ӱ������
		virtual unsigned long long getAffactedRows()=0;

		//!�õ��²����¼��ID
		virtual unsigned long long getInsertID()=0;

		//!�õ�������Ϣ
		virtual const char * error()=0;
		
		//@{
		/*! @brief �������
			@param arg �������
		*/
		Query & operator << ( const char & arg );
		Query & operator << ( const unsigned char & arg );
		Query & operator << ( const short & arg );
		Query & operator << ( const unsigned short & arg );
		Query & operator << ( const int & arg );
		Query & operator << ( const unsigned int & arg );
		Query & operator << ( const long & arg );
		Query & operator << ( const unsigned long & arg );
		Query & operator << ( const long long & arg );
		Query & operator << ( const unsigned long long & arg );
		Query & operator << ( const char * arg );
		Query & operator << ( const std::string & arg );
		Query & operator << ( const float & arg);
		Query & operator << ( const double & arg);
		Query & operator << ( const Date & arg);
		Query & operator << ( const Time & arg);
		Query & operator << ( const DateTime & arg);
		Query & operator << ( const TinyBinary & arg);
		//@}
	protected:
		//!���캯��
		Query(
			Connection & conn	//!< ���ݿ����Ӷ���
			);
		virtual ~Query();

		//!���MYSQL����
		MYSQL * getMySQL();

		//!������
		void parseArgTypes(
			const char *args	//!< ��ѯ����
			);

		/*! @brief ���Ҳ���
			@retval >=0 ����λ������
			@retval <0 û�ҵ�
		*/
		int findParameter(
			const char *name	//!< ��������
			);

		//!������������
		static void parseArgTypes(
			const char *args,					//!< �����б�
			std::vector<ParameterType> &types	//!< ��ŷ��صĲ�������
			);

		//! ������������
		static bool parseSign(
			const char * str_len,	//!< �����ַ���
			const char *separator,	//!< Ҫ����
			bool & sign				//!< ���޷���
			);

		ParameterType * m_paramTypes;	//!< ������������
		struct QueryElement
		{
			enum
			{
				T_TEXT,		//!< �ı����ͣ������κ��滻
				T_PARAM		//!< �ò����滻
			} type;
			union
			{
				struct		
				{
					const char * begin;	//!< ��ʼ�ı���ַ
					int len;			//!< �ı�����
				} text;
				struct
				{
					int index;			//!< ����λ������
				} param;
			} element;
		};
		//����Ԫ��
		void parseElements(
			const char *sql		//!< SQL���
			);

		std::list<QueryElement> m_queryElements;	//!< ������Ԫ���б�
		ParameterValue  * m_args;					//!< ��ǰ��������
		WORD m_argIndex;							//!< ��������
		WORD m_argCount;							//!< ��������
		Connection & m_conn;						//!< ���Ӷ���
	private:
		/*! @brief �رղ�ѯ
			
			��Connect::closeLastQuery����
		*/
		virtual void close()=0;
	};
	
	inline Query & Query::operator << ( const char & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const unsigned char & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const short & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const unsigned short & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const int & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const unsigned int & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const long & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const unsigned long & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const long long & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const unsigned long long & arg )
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const char * arg )
	{
		m_args[m_argIndex++].setValue(arg,static_cast<unsigned int>(strlen(arg)));
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const std::string & arg )
	{
		m_args[m_argIndex++].setValue(arg.c_str(),static_cast<unsigned int>(arg.length()));
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const float & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const double & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const Date & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const Time & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
		
	inline Query & Query::operator << ( const DateTime & arg)
	{
		m_args[m_argIndex++].setValue(arg);
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}
	
	inline Query & Query::operator << ( const TinyBinary & arg)
	{
		m_args[m_argIndex++].setValue(reinterpret_cast<const char*>(arg.getData()), arg.getSize());
		ASSERT(m_argIndex <= m_argCount);
		return *this;
	}

	inline MYSQL * Query::getMySQL()
	{
		return m_conn.getMySQL();
	}
}
	
#endif
