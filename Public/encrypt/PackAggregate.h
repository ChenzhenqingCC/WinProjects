#pragma once
#include <vector>
#include "ccMacros.h"

#pragma warning (push)
#pragma warning (disable : 4201)
class PackAggregate
{
public:
	enum enumFlags
	{
		flagCompress = 0x1,
		flagEncrypt = 0x2,
		flagEncryptHeaderOnly = 0x4,	//仅仅文件头加密
	};

	struct stPackHeader
	{
		DWORD magic;
		DWORD version;
		int filenum;
		int headerSize;
		DWORD flag;
	};
	struct stFileInfo1
	{
		DWORD dwFileID;
		DWORD fileOffset;
		DWORD size;		//压缩加密后的大小
		DWORD interspaceSize;	//所有空间大小，包括未用的空间
		DWORD dwCRC;	//压缩加密后的CRC
	};
	struct stFileInfo
	{
		union
		{
			struct
			{
				DWORD dwFileID;
				DWORD fileOffset;
				DWORD size;		//压缩加密后的大小
				DWORD interspaceSize;//所有空间大小，包括未用的空间
				DWORD dwCRC;//压缩加密后的CRC
			};
			stFileInfo1 st1;
		};
		char szName[MAX_PATH];
	};

	struct stFileSizeInfo
	{
		DWORD version;
		DWORD flags;
		DWORD uncompressedSize;	//未压缩加密的大小
		DWORD dwCRC;	//未压缩加密的CRC
	};

	// 在子文件内部的信息
	struct stFileBaseInfo
	{
		char szName[MAX_PATH];
		DWORD fileOffset;
		DWORD size;	//文件整个大小（包括头）
		DWORD dwCRC;//压缩加密后的CRC
	};

	struct stFileDetailInfo
	{
		DWORD fileOffset;
		DWORD uncompressedSize;	//文件实际大小
		DWORD dwUnprocessCRC;	//未压缩加密的CRC
		DWORD flags;
	};
private:
	char m_szFileName[MAX_PATH];
	std::vector<stFileBaseInfo> m_aFileInfo;
	DWORD m_dwFlags;
	DWORD m_dwVersion;
	const BYTE* m_pDesKey;
public:
	typedef std::vector<stFileBaseInfo>::const_iterator iterator;
	PackAggregate();
	~PackAggregate();

	//bool getFileInfo(DWORD dwFileIndex, Stream* pStream, DWORD dwVersion, DWORD flags, DWORD offset, stFileDetailInfo * result);
	DWORD getFlags(){ return m_dwFlags; }
	DWORD getVersion(){ return m_dwVersion; }
	bool refreshAggregate();

	enum enumOpenResult
	{
		OpenOk,
		OpenUnkFormat,
		OpenVersionError,
		OpenDataError,
		OpenFileError,
	};
	enumOpenResult openAggregate(const char * szFileName);
	static bool getFileInfo(const char * szFileName, std::vector<stFileInfo> & files);
	static bool resortFilePack(const char* pack, std::vector<PackAggregate::stFileInfo> & files);

	void closeAggregate();
	iterator begin(){ return m_aFileInfo.begin(); }
	iterator end(){ return m_aFileInfo.end(); }
	void SetDesKey(const BYTE* pKey)
	{
		m_pDesKey = pKey;
	}
	static const char* GetDefaultPassword();
	typedef void(*tOuntputFunc)(LPCTSTR lpszFmt, ...);
	//static bool MakePackDir(LPCTSTR szDir, LPCTSTR szPackName, bool bZip, int zipLevel, const char* szPassword, 
		//std::vector<std::string> & fileFilter, tOuntputFunc pOutputFunc, bool & bStop);
};

