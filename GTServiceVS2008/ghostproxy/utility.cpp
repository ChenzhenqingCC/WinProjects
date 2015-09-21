/************************************************************************
* 
*
*
/************************************************************************/
#include "stdafx.h"
#include "utility.h"
#include <IPTypes.h>
#include <IPHlpApi.h>
#pragma comment(lib,"Iphlpapi.lib")
#include <WtsApi32.h>
#pragma comment(lib,"wtsapi32.lib")
#include <Userenv.h>
#pragma comment(lib,"Userenv.lib")
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#include <Nb30.h>
#include <WinIoCtl.h>
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#include <string>
#include <vector>
using namespace std;
/**
*	return current process path
*/
CString GetPath(HMODULE hm)
{
	CString m_path;
	TCHAR buffer[MAX_PATH]={0};
	if (::GetModuleFileName(hm,buffer,MAX_PATH))
	{
		m_path = buffer;
		int dist = m_path.ReverseFind(_T('\\'));
		if (dist > -1)
		{
			m_path.Delete(dist,m_path.GetLength()-dist);
		}
	}
	return m_path;
}
/**
*	return current process short path
*/
CString GetShortPath(LPCTSTR path)
{
	CString m_path = path;
	long     length = 0;
	TCHAR*   buffer = NULL;
	// First obtain the size needed by passing NULL and 0.
	length = GetShortPathName(m_path, NULL, 0);
	if (length == 0)
	{
		return m_path;
	}
	// Dynamically allocate the correct size 
	// (terminating null char was included in length)
	buffer = new TCHAR[length];
	// Now simply call again using same long path.
	length = GetShortPathName(m_path, buffer, length);
	if (length == 0)
	{
		delete [] buffer;
		return m_path;
	}
	m_path = buffer;
	delete [] buffer;
	return m_path;
}
/**
*
*/
CString GetConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,LPCTSTR default_value)
{
	TCHAR tmp_buffer[4096]={0};
	GetPrivateProfileString(secname, keyname, default_value, tmp_buffer,4095,path);
	return tmp_buffer;
}
/**
*
*/
int GetConfigInt(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,int default_value)
{
	int res = GetPrivateProfileInt(secname,keyname,default_value,path);
	if(!res)
	{
		res = default_value;
	}
	return res;
}
/**
*
*/
BOOL  SetConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname,LPCTSTR value)
{
	return WritePrivateProfileString(secname, keyname, value,path); 
}
/**
*if keyname if not NULL,will delete key,else delete the section
*
*/
BOOL DelConfig(LPCTSTR path,LPCTSTR secname,LPCTSTR keyname)
{
	return SetConfig(path,secname,keyname,NULL);
}
/**
*
*/
DWORD MyMessageBox(const CString& msg,const CString& cap)
{
	DWORD curSessionId = WTSGetActiveConsoleSessionId();
	DWORD dwResponse=0;
	WTSSendMessage(WTS_CURRENT_SERVER_HANDLE, curSessionId, (LPTSTR)(LPCTSTR)cap, cap.GetLength(), (LPTSTR)(LPCTSTR)msg, msg.GetLength(), MB_YESNO, 0 /*wait indefinitely*/, &dwResponse, TRUE);
	return dwResponse-6;//push ok
}
/**
*
*/
PROCESS_INFORMATION LaunchApp(LPCTSTR lpszClientPath,LPTSTR lpszCmdLine)
{
	BOOL bSuccess = FALSE;
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	si.cb = sizeof(si);
	DWORD dwSessionID = WTSGetActiveConsoleSessionId();
	HANDLE hToken = NULL;
	HANDLE hDuplicatedToken = NULL;
	LPVOID lpEnvironment = NULL;
	if (WTSQueryUserToken(dwSessionID, &hToken) == FALSE)
	{
		goto Cleanup;
	}
	
	if (DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hDuplicatedToken) == FALSE)
	{
		goto Cleanup;
	}
	
	if (CreateEnvironmentBlock(&lpEnvironment, hDuplicatedToken, FALSE) == FALSE)
	{
		goto Cleanup;
	}
	//WCHAR lpszClientPath[MAX_PATH];
	//if (GetModuleFileName(NULL, lpszClientPath, MAX_PATH) == 0)
	//{
	//	goto Cleanup;
	//}
	//PathRemoveFileSpec(lpszClientPath);
	//wcscat_s(lpszClientPath, sizeof(lpszClientPath)/sizeof(WCHAR), L"\\TimeServiceClient.exe");
	//si.wShowWindow = SW_HIDE;
	if (CreateProcessAsUser(hDuplicatedToken, lpszClientPath, lpszCmdLine, NULL, NULL, FALSE,NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,lpEnvironment, NULL, &si, &pi) == FALSE)
	{
		goto Cleanup;
	}
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
	bSuccess = TRUE;
Cleanup:
	if (!bSuccess)
	{
		//MyMessageBox(_T("An error occurred while creating fancy client UI"), L"Error");
	}
	if (hToken != NULL)
	{
		CloseHandle(hToken);
	}
	if (hDuplicatedToken != NULL)
	{
		CloseHandle(hDuplicatedToken);
	}
	if (lpEnvironment != NULL)
	{
		DestroyEnvironmentBlock(lpEnvironment);
	}
	return pi;
}
/**
*
*/
BOOL CreateSA(SECURITY_ATTRIBUTES& sa,BOOL inherit)
{
	SECURITY_DESCRIPTOR sd;
	ZeroMemory(&sd, sizeof(sd));
	if(!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
	{
		return FALSE;
	}

	if(!SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE))
	{
		return FALSE;
	}

	ZeroMemory(&sa, sizeof(sa));
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = &sd;
	sa.bInheritHandle = inherit;
	return TRUE;
}
/**
*
*/
CString PackMd5Hash(const BYTE *md5order,const DWORD len)
{
	CString strs;
	CCryptProv prov;
	BYTE  mybyte[256]={0};
	HRESULT hr = prov.Initialize(PROV_RSA_FULL,NULL,MS_DEF_PROV,CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
	if( hr == 0x8009016L )
		prov.Initialize( PROV_RSA_FULL,NULL,MS_DEF_PROV,CRYPT_VERIFYCONTEXT | CRYPT_SILENT|CRYPT_NEWKEYSET );
	CCryptMD5Hash hash;
	hash.Initialize( prov );
	hash.AddData(md5order,len,0);
	DWORD nlen=256;   
	hash.GetValue(mybyte,&nlen);
	//TCHAR strs=new TCHAR[256];
	//memset(strs,0,256);
	TCHAR buffer[256]={0};
	for(int i=0;i<(int)nlen;i++)
	{
		_itot_s(mybyte[i],buffer,256,16);
		strs +=buffer;
		//_tprintf(strs,_T("%s%02X"),strs,mybyte[i]);
	}
	return strs;
}



typedef std::string			String;
#define MACADDRESS_DEAULT_ID   "GDFSG4FDS1234"  
#define CPU_DEAULT_ID          "abcdefghi"  
#define HARDWARE_DEAULT_ID     "32145698712345FDFG123"  

#define GETVERSIONOUTPARAMS     GETVERSIONINPARAMS
#define DFP_GET_VERSION         SMART_GET_VERSION
#define DFP_SEND_DRIVE_COMMAND  SMART_SEND_DRIVE_COMMAND
#define DFP_RCV_DRIVE_DATA      SMART_RCV_DRIVE_DATA

const WORD IDE_ATAPI_IDENTIFY = 0xA1;   
const WORD IDE_ATA_IDENTIFY   = 0xEC;   

const int MAX_IDE_DRIVES = 4;


BOOL InternalGetCPUID(String & strCPUID);
char* FormatString(char* buffer, unsigned int count, const char* format...);
namespace net
{
	/// @brief 
	struct MACAddress
	{
	public:
		void Reset();
		MACAddress();
		MACAddress(BYTE m0, BYTE m1, BYTE m2, BYTE m3, BYTE m4, BYTE m5);
		void Set(BYTE m0, BYTE m1, BYTE m2, BYTE m3, BYTE m4, BYTE m5)
		{
			mac0 = m0; mac1 = m1; mac2 = m2; mac3 = m3; mac4 = m4; mac5 = m5;
		}
		/// @brief 
		bool Parse(const char* pstr);
		std::string ToString() const;
	public:
		BYTE mac0;	
		BYTE mac1;
		BYTE mac2;
		BYTE mac3;
		BYTE mac4;
		BYTE mac5;	
	};

	class CMAC
	{
	public:
		typedef std::vector<MACAddress>::iterator iterator;
		typedef std::vector<MACAddress>::const_iterator const_iterator;
	public:
		CMAC();
		~CMAC();
		void Reset(){m_mac.clear();}
		const std::vector<MACAddress>& GetMACAddress()const {return m_mac;}
		int Size(){	return static_cast<int>(m_mac.size());}	
	public:
		static int GetMACByNetbios(CMAC* pmac);

		static int GetMACByCmdPipe(CMAC* pmac);

		static int GetMACByIphlpapi(CMAC* pmac);

		static int GetMAC(CMAC* pmac)
		{
			int n = GetMACByIphlpapi(pmac);
			if(n == 0)
				n = GetMACByNetbios(pmac);
			if(n == 0)
				n = GetMACByCmdPipe(pmac);
			return n;
		}
	protected:
		std::vector<MACAddress> m_mac;
	};

	MACAddress::MACAddress(): mac0(0), mac1(0), mac2(0), mac3(0), mac4(0), mac5(0)
	{
	}
	MACAddress::MACAddress(BYTE m0, BYTE m1, BYTE m2, BYTE m3, BYTE m4, BYTE m5)
		: mac0(m0), mac1(m1), mac2(m2), mac3(m3), mac4(m4), mac5(m5)
	{
	}
	void MACAddress::Reset()
	{
		mac0 = mac1 = mac2 = mac3 = mac4 = mac5 = 0;
	}

	bool MACAddress::Parse(const char* pstr)
	{
		if(pstr == NULL)
		{
			this->Reset();
			return false;
		}
		int temp[6];
#if defined( _MSC_VER ) && _MSC_VER <= 1310	
		int n = sscanf(pstr, "%2X-%2X-%2X-%2X-%2X-%2X", &temp[0], &temp[1], &temp[2], &temp[3], &temp[4], &temp[5]);
#else
		int n = sscanf_s(pstr, "%2X-%2X-%2X-%2X-%2X-%2X", &temp[0], &temp[1], &temp[2], &temp[3], &temp[4], &temp[5]);
#endif
		
		this->Set(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]);
		if(n != 6)
		{
			this->Reset();
			return false;
		}
		else
			return true;
	}

	std::string MACAddress::ToString() const
	{
		char buf[256];
		FormatString(buf, sizeof(buf), "%02X-%02X-%02X-%02X-%02X-%02X",
			mac0, mac1, mac2, mac3, mac4, mac5);
		return buf;
	}
	CMAC::CMAC()
	{
	}
	CMAC::~CMAC()
	{
	}
	int CMAC::GetMACByNetbios(CMAC* pmac)     
	{     
		if(pmac == NULL)
			return 0;
		pmac->Reset();

		NCB ncb;     
		typedef struct _ASTAT_     
		{
			ADAPTER_STATUS	adapt;  
			NAME_BUFFER		NameBuff[30];     
		}ASTAT,*PASTAT;     
		ASTAT Adapter;     

		typedef struct _LANA_ENUM     
		{
			UCHAR   length;  
			UCHAR   lana[MAX_LANA];     
		}LANA_ENUM;     
		LANA_ENUM lana_enum;     

		UCHAR uRetCode;     
		memset(&ncb, 0, sizeof(ncb));     
		memset(&lana_enum, 0, sizeof(lana_enum));     

		ncb.ncb_command = NCBENUM;     
		ncb.ncb_buffer = (unsigned char *)&lana_enum;     
		ncb.ncb_length = sizeof(LANA_ENUM);     
		uRetCode = Netbios(&ncb);     
		if(uRetCode != NRC_GOODRET)     
			return 0;     

		for(int lana = 0; lana < lana_enum.length; lana++)     
		{
			ncb.ncb_command = NCBRESET; 
			ncb.ncb_lana_num = lana_enum.lana[lana]; 
			uRetCode = Netbios(&ncb);   
			if(uRetCode == NRC_GOODRET)  
			{
				memset(&ncb, 0, sizeof(ncb));  
				ncb.ncb_command = NCBASTAT;  
				ncb.ncb_lana_num = lana_enum.lana[0];
#if defined( _MSC_VER ) && _MSC_VER <= 1310	 
				strncpy((char*)ncb.ncb_callname, "*",NCBNAMSZ ); 
#else
				strcpy_s((char*)ncb.ncb_callname, NCBNAMSZ, "*"); 
#endif 
				ncb.ncb_buffer = (unsigned char *)&Adapter;
				ncb.ncb_length = sizeof(Adapter);
				uRetCode = Netbios(&ncb);  
				if(uRetCode != NRC_GOODRET)   
					continue;
				pmac->m_mac.push_back(MACAddress(
					Adapter.adapt.adapter_address[0],     
					Adapter.adapt.adapter_address[1],     
					Adapter.adapt.adapter_address[2],     
					Adapter.adapt.adapter_address[3],     
					Adapter.adapt.adapter_address[4],     
					Adapter.adapt.adapter_address[5]));
			}
		} 
		return pmac->Size();
	} 	

	int CMAC::GetMACByCmdPipe(CMAC* pmac)
	{
	
		const std::string str4Search = "Physical Address. . . . . . . . . : ";

		const long MAX_SHELL_COMMAND_SIZE = 10000;	

		if(pmac == NULL)
			return 0;
		pmac->Reset();

		BOOL bret; 
		SECURITY_ATTRIBUTES sa; 
		HANDLE hReadPipe,hWritePipe; 

		sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
		sa.lpSecurityDescriptor = NULL; 
		sa.bInheritHandle = TRUE; 

		bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
		if(bret == FALSE)
			return 0;

		STARTUPINFO si; 
		PROCESS_INFORMATION pi; 

		si.cb = sizeof(STARTUPINFO); 
		GetStartupInfo(&si); 
		si.hStdError = hWritePipe; 
		si.hStdOutput = hWritePipe; 
		si.wShowWindow = SW_HIDE; 
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		bret = CreateProcess (NULL, _T("ipconfig /all"), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ); 
		if (bret == FALSE) 
			return 0;

		WaitForSingleObject (pi.hProcess, INFINITE); 
		char szBuffer[MAX_SHELL_COMMAND_SIZE + 1]; 
		memset(szBuffer, 0x00, sizeof(szBuffer));
		unsigned long count;
		bret = ReadFile(hReadPipe,  szBuffer,  MAX_SHELL_COMMAND_SIZE,  &count,  0);
		if(bret == TRUE)
		{
			const std::string strBuffer(szBuffer);
			size_t n = 0;
			while(true)
			{
				n = strBuffer.find(str4Search, n);
				if(n == String::npos)
					break;
				else 
					n += str4Search.size();
				if(n >= strBuffer.size())
					break;
				const char* p = szBuffer + n;
				MACAddress mac;
				if(mac.Parse(p))
				{
					pmac->m_mac.push_back(mac);
				}
			}
		}

		CloseHandle(hWritePipe);
		CloseHandle(pi.hProcess); 
		CloseHandle(pi.hThread); 
		CloseHandle(hReadPipe);
		return pmac->Size();
	}

	int CMAC::GetMACByIphlpapi(CMAC* pmac)
	{
		if(pmac == NULL)
			return 0;
		pmac->Reset();

		IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
		DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

		DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
			AdapterInfo,							// [out] buffer to receive data
			&dwBufLen);								// [in] size of receive data buffer
		if(dwStatus != ERROR_SUCCESS)				// Verify return value is valid, no buffer overflow
			return 0;

		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
		do {
			pmac->m_mac.push_back(MACAddress(
				pAdapterInfo->Address[0],     
				pAdapterInfo->Address[1],     
				pAdapterInfo->Address[2],     
				pAdapterInfo->Address[3],     
				pAdapterInfo->Address[4],     
				pAdapterInfo->Address[5]));
			pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
		}
		while(pAdapterInfo);	
		// Terminate if last adapter
		return pmac->Size();
	}
}

std::ostream& operator<<(std::ostream& os, const net::MACAddress& data);
std::ostream& operator<<(std::ostream& os, const net::CMAC& data);

std::ostream& operator<<(std::ostream& os, const net::MACAddress& data)
{
	char buf[64];

#if defined( _MSC_VER ) && _MSC_VER <= 1310	 //兼容VC7.1及以前版本
	_snprintf(buf, sizeof(buf), "%02X-%02X-%02X-%02X-%02X-%02X",data.mac0, data.mac1, data.mac2, data.mac3, data.mac4, data.mac5);
#else
	_snprintf_s(buf, sizeof(buf), "%02X-%02X-%02X-%02X-%02X-%02X",data.mac0, data.mac1, data.mac2, data.mac3, data.mac4, data.mac5);
#endif
	os << buf;
	return os;
}

std::ostream& operator<<(std::ostream& os, const net::CMAC& data)
{
	const std::vector<net::MACAddress> mac = data.GetMACAddress();
	for(net::CMAC::const_iterator it = mac.begin(); it != mac.end(); ++it)
	{
		os << *it << std::endl;
	}
	return os;
}


void  ConvertToString(String & strOut,DWORD dwDiskData[256], int nFirstIndex, int nLastIndex);

// Win98
void ReadPhysicalDriveOnW9X(String & pSerList);

void  ReadPhysicalDriveOnNT(String & pSerList);
BOOL  DoIdentify(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
				 PSENDCMDOUTPARAMS pSCOP, BYTE btIDCmd,
				 BYTE btDriveNum, PDWORD lpcbBYTEsReturned);
//---------------------------------------------------------------------------
// ReadPhysicalDrive
void ReadPhysicalDrive(String  & pSerList)
{
	OSVERSIONINFO osvi;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bval=GetVersionEx(&osvi);
	if( !bval )
		return;

	switch(osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_WINDOWS:
		//ReadPhysicalDriveOnW9X(pSerList);
		break;
	case VER_PLATFORM_WIN32_NT:
		ReadPhysicalDriveOnNT(pSerList);
		return;
	default:
		break;
	}
	pSerList = HARDWARE_DEAULT_ID;
	return;
}

//---------------------------------------------------------------------------
// ConvertToString
void ConvertToString(String & strOut,DWORD dwDiskData[256], int nFirstIndex, int nLastIndex)
{
	char szResBuf[1024];
	int nIndex = 0;
	int nPosition = 0;

	// Each integer has two characters stored in it backwards
	for(nIndex = nFirstIndex; nIndex <= nLastIndex; nIndex++)
	{
		// Get high BYTE for 1st character
		szResBuf[nPosition] = (char)(dwDiskData[nIndex] / 256);
		nPosition++;

		// Get low BYTE for 2nd character
		szResBuf[nPosition] = (char)(dwDiskData[nIndex] % 256);
		nPosition++;
	}

	// End the string
	szResBuf[nPosition] = '\0';

	// Cut off the trailing blanks
	for(nIndex = nPosition - 1; nIndex > 0 && ' ' == szResBuf[nIndex]; nIndex--){
		szResBuf[nIndex] = '\0';
	}
	strOut = szResBuf;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ReadPhysicalDriveOnW9X(String & pSerList)
{
	DWORD dwSerialNum;
	if(GetVolumeInformation(_T("C:\\"),NULL,0,&dwSerialNum,NULL,NULL,NULL,0))
	{
		char szSerail[256];
		FormatString(szSerail,256,"%u",dwSerialNum);
		pSerList = szSerail;
	}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ReadPhysicalDriveOnNT
void ReadPhysicalDriveOnNT(String & pSerList)
{
	BYTE btIDOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];

	for(int nDrive=0; nDrive < MAX_IDE_DRIVES; nDrive++)
	{
		HANDLE hPhysicalDriveIOCTL;

		char szDriveName[64];
		FormatString(szDriveName,64,"\\\\.\\PhysicalDrive%d", nDrive);

		hPhysicalDriveIOCTL = CreateFileA(szDriveName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);

		if(hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			DWORD dwBytesReturned = 0;
			GETVERSIONOUTPARAMS gvopVersionParams;

			// Get the version, etc of PhysicalDrive IOCTL
			ZeroMemory(&gvopVersionParams, sizeof(GETVERSIONOUTPARAMS));

			if(!DeviceIoControl(hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL, 0, &gvopVersionParams, sizeof(gvopVersionParams),
				&dwBytesReturned, NULL))
			{
				continue;
			}

			//bool bGetOk = false;
			if(gvopVersionParams.bIDEDeviceMap > 0)
			{
				// IDE or ATAPI IDENTIFY cmd
				BYTE btIDCmd = 0;
				SENDCMDINPARAMS InParams;
				// Now, get the ID sector for all IDE devices in the system.
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
				// otherwise use the IDE_ATA_IDENTIFY command
				btIDCmd = (gvopVersionParams.bIDEDeviceMap >> nDrive & 0x10) ?
							IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
				ZeroMemory(&InParams, sizeof(SENDCMDINPARAMS));
				ZeroMemory(btIDOutCmd, sizeof(btIDOutCmd));

				if(DoIdentify(hPhysicalDriveIOCTL,
					&InParams, (PSENDCMDOUTPARAMS)btIDOutCmd,
					(BYTE)btIDCmd, (BYTE)nDrive, &dwBytesReturned))
				{
					DWORD dwDiskData[256];
					USHORT *pIDSector; 
				
					pIDSector = (USHORT*)((SENDCMDOUTPARAMS*)btIDOutCmd)->bBuffer;
					for(int i=0; i < 256; i++){
						dwDiskData[i] = pIDSector[i];
					}
		
					String strTemp; 
					ConvertToString(strTemp,dwDiskData, 10, 19);
					 
					//bGetOk = true;

					pSerList += strTemp;
				}
			}
			CloseHandle (hPhysicalDriveIOCTL);
		}
	}
}
//---------------------------------------------------------------------------
// DoIdentify
BOOL DoIdentify(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
				PSENDCMDOUTPARAMS pSCOP, BYTE btIDCmd, BYTE btDriveNum,
				PDWORD pdwBytesReturned)
{
	// Set up data structures for IDENTIFY command.
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg = 0;
	pSCIP->irDriveRegs.bSectorCountReg  = 1;
	pSCIP->irDriveRegs.bSectorNumberReg = 1;
	pSCIP->irDriveRegs.bCylLowReg  = 0;
	pSCIP->irDriveRegs.bCylHighReg = 0;

	pSCIP->irDriveRegs.bDriveHeadReg = (btDriveNum & 1) ? 0xB0 : 0xA0;

	// The command can either be IDE identify or ATAPI identify.
	pSCIP->irDriveRegs.bCommandReg = btIDCmd;
	pSCIP->bDriveNumber = btDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	return DeviceIoControl(hPhysicalDriveIOCTL, DFP_RCV_DRIVE_DATA,
		(LPVOID)pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID)pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		pdwBytesReturned, NULL);
}

///////////////////////////////////////////////////////////////////////
// MACADDRESS
///////////////////////////////////////////////////////////////////////


BOOL InternalGetCPUID(String & strCPUID)
{
	__try 
	{
		_asm 
		{
			xor eax, eax
				cpuid
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return FALSE;
	}

	union
	{
		char szSerial[9];
		struct 
		{
			DWORD  dw0;
			DWORD  dw1;
		} s;
	} Serial;

	_asm
	{
		push ebx
			push ecx
			push edx

			mov eax,0x3
			cpuid
			mov Serial.s.dw0,edx
			mov Serial.s.dw1,ecx

			pop edx
			pop ecx
			pop ebx
	}
	Serial.szSerial[8] = 0;
	strCPUID = Serial.szSerial;
 
	return TRUE;
}

BOOL GetCPUID(String & strOut)
{
	if(!InternalGetCPUID(strOut))
	{
		strOut =  CPU_DEAULT_ID;
		return FALSE;
	}
	return TRUE;
}

void GetMACaddress(String & strOutter)
{
	net::CMAC mac;
	if(net::CMAC::GetMAC(&mac) != 0)
	{
		const net::MACAddress& adr = mac.GetMACAddress()[0];

		char acMAC[32];	
		FormatString(acMAC, sizeof(acMAC),"%02X%02X%02X%02X%02X%02X",
			adr.mac0, 
			adr.mac1,
			adr.mac2, 
			adr.mac3, 
			adr.mac4, 
			adr.mac5);
		strOutter = acMAC;
	}
	else
		strOutter = MACADDRESS_DEAULT_ID;

}

char* FormatString(char* buffer, unsigned int count, const char* format...)
{
	va_list args;
	va_start(args, format);
#if _DEBUG
	buffer[count-1] = 0;
#endif
	_vsnprintf(buffer, count, format, args); 
#if defined _DEBUG && !defined GAME_TOOL
	if(buffer[count-1] != 0)
	{
		char buf[1024];
		_vsnprintf(buf, sizeof(buf), format, args); 
	}
#endif
	buffer[count-1] = 0;
	va_end(args);
	return buffer;
}

std::string GetUUID()
{
	std::string Result("");
	std::string temp("");

	GetCPUID(temp);
	Result += temp+"_";

	temp = "";
	GetMACaddress(temp);
	Result += temp+"_";

	temp = "";
	ReadPhysicalDrive(temp);
	Result += temp;

	return Result;
}
/**
*
*/
CString GetMac()
{
	TCHAR sid[4086]={0};
	string temp = "";
	GetMACaddress(temp);
#ifdef _UNICODE
	SHAnsiToUnicode(temp.c_str(),sid,4086);
	return sid;
#else
	return temp.c_str();
#endif
	
}


/**
* this topic is so common
* not use HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Cryptography\MachineGuid 
* in china,
* be care about smb,maybe some pc will update this table when running
* be care about 
*/
CString GenDeviceID()
{
	TCHAR sid[4086]={0};
	string temp = GetUUID();
#ifdef _UNICODE
	DWORD ulen = SHAnsiToUnicode(temp.c_str(),sid,4086);
	return PackMd5Hash((const BYTE*)sid,sizeof(TCHAR)*ulen);
#else
	return PackMd5Hash((const BYTE*)temp.c_str(),sizeof(TCHAR)*temp.length());
#endif
	//sid = PackMd5Hash((const BYTE*)(LPCTSTR)sid,sizeof(TCHAR)*sid.GetLength());
}