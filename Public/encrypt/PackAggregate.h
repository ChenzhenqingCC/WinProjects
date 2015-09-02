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
		flagEncryptHeaderOnly = 0x4,	//�����ļ�ͷ����
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
		DWORD size;		//ѹ�����ܺ�Ĵ�С
		DWORD interspaceSize;	//���пռ��С������δ�õĿռ�
		DWORD dwCRC;	//ѹ�����ܺ��CRC
	};
	struct stFileInfo
	{
		union
		{
			struct
			{
				DWORD dwFileID;
				DWORD fileOffset;
				DWORD size;		//ѹ�����ܺ�Ĵ�С
				DWORD interspaceSize;//���пռ��С������δ�õĿռ�
				DWORD dwCRC;//ѹ�����ܺ��CRC
			};
			stFileInfo1 st1;
		};
		char szName[MAX_PATH];
	};

	struct stFileSizeInfo
	{
		DWORD version;
		DWORD flags;
		DWORD uncompressedSize;	//δѹ�����ܵĴ�С
		DWORD dwCRC;	//δѹ�����ܵ�CRC
	};

	// �����ļ��ڲ�����Ϣ
	struct stFileBaseInfo
	{
		char szName[MAX_PATH];
		DWORD fileOffset;
		DWORD size;	//�ļ�������С������ͷ��
		DWORD dwCRC;//ѹ�����ܺ��CRC
	};

	struct stFileDetailInfo
	{
		DWORD fileOffset;
		DWORD uncompressedSize;	//�ļ�ʵ�ʴ�С
		DWORD dwUnprocessCRC;	//δѹ�����ܵ�CRC
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

