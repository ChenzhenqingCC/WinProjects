#ifndef __LOGTARGETDESCRIPTION_H
#define __LOGTARGETDESCRIPTION_H

#include <string>

#ifdef _MSC_VER
#else
#include <string.h>
#include <stdio.h>
#endif


namespace Zoic
{
	class LogTargetDescription
	{
	public:
		LogTargetDescription(unsigned char* buf,unsigned long limit);
		LogTargetDescription(const LogTargetDescription& rhd);
		void setType(unsigned char type);
		unsigned char getType() const;

		void resetSize();
		unsigned long getSize() const;
		const unsigned char* getData() const;

		void clear();

		void readData(void* pData,int n);
		void writeData(const void* pData,int n);
		void skipData(int n);

		void* getWritePos() const;
		void* getReadPos() const;

		LogTargetDescription& operator = (const LogTargetDescription& o);

		LogTargetDescription & operator << (const char & val);
		LogTargetDescription & operator << (const unsigned char & val);
		LogTargetDescription & operator << (const short & val);
		LogTargetDescription & operator << (const unsigned short & val);
		LogTargetDescription & operator << (const int & val);
		LogTargetDescription & operator << (const unsigned int & val);
		LogTargetDescription & operator << (const long & val);
		LogTargetDescription & operator << (const unsigned long & val);
		LogTargetDescription & operator << (const long long & val);
		LogTargetDescription & operator << (const unsigned long long & val);
		LogTargetDescription & operator << (const float & val);
		LogTargetDescription & operator << (const double & val);
		LogTargetDescription & operator << (const char* & val);
		LogTargetDescription & operator << (const std::string & val);


		LogTargetDescription & operator >> (char & val);
		LogTargetDescription & operator >> (unsigned char & val);
		LogTargetDescription & operator >> (short & val);
		LogTargetDescription & operator >> (unsigned short & val);
		LogTargetDescription & operator >> (int & val);
		LogTargetDescription & operator >> (unsigned int & val);
		LogTargetDescription & operator >> (long & val);
		LogTargetDescription & operator >> (unsigned long & val);
		LogTargetDescription & operator >> (long long & val);
		LogTargetDescription & operator >> (unsigned long long & val);
		LogTargetDescription & operator >> (float & val);
		LogTargetDescription & operator >> (double & val);
		LogTargetDescription & operator >> (std::string &val);
		LogTargetDescription & operator >> (const char* & val);		

	private:
		unsigned long * m_pSize;
		unsigned char * m_pType;
		unsigned char * m_pRead;
		unsigned char * m_pWrite;

		unsigned long m_limit;
		unsigned char * m_pBuf;
	};

	inline LogTargetDescription& LogTargetDescription::operator = (const LogTargetDescription& o)
	{
		clear();
		memmove(m_pBuf,o.getData(),o.getSize());
		return *this;
	}

	inline const unsigned char* LogTargetDescription::getData() const
	{
		return m_pBuf;
	}

	inline void LogTargetDescription::resetSize()
	{
		*m_pSize = sizeof(unsigned char) + sizeof(unsigned long);
	}

	inline unsigned long LogTargetDescription::getSize() const
	{
		return *m_pSize;
	}

	inline unsigned char LogTargetDescription::getType() const
	{
		return *m_pType;
	}

	inline void LogTargetDescription::setType(unsigned char type)
	{
		*m_pType = type;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const char & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const unsigned char & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const short & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const unsigned short & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const int & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const unsigned int & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const long & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const unsigned long & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const long long & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const unsigned long long & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const float & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const double & val)
	{
		writeData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const char* & val)
	{
		writeData(val, static_cast<int>(strlen(val) + 1));
		return * this;
	}

	inline LogTargetDescription & LogTargetDescription::operator << (const std::string & val)
	{
		writeData(val.c_str(), static_cast<int>(val.length() + 1));
		return * this;
	};

	inline LogTargetDescription & LogTargetDescription::operator >> (char & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (unsigned char & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (short & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (unsigned short & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (int & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (unsigned int & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (unsigned long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (long long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (unsigned long long & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (float & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}

	inline LogTargetDescription & LogTargetDescription::operator >> (double & val)
	{
		readData(&val,sizeof(val));
		return *this;
	}
}

#endif