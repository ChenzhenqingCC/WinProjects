#ifndef __RANASRV_RANACLIENT_H
#define __RANASRV_RANACLIENT_H
#include <Zoic/include/Types.h>
#include <Zoic/include/SessionS.h>
#include "RanaScriptManager.h"
#include "protocol/Rana/RanaProto_Rana.h"

using namespace RanaProto;

#define LUA_SCRIPT_EXT ".lua"
#define LUA_SAVEFILE_EXT ".xml"

class RanaSendInfo;

const DWORD RANA_MAX_FILE_LENGTH	= 200000000;
const DWORD RANA_MAX_BUFFER_LENGTH	= 200000000;

class RanaClient:public Zoic::SessionS
{
	friend class RanaService;
	friend class RanaServicesStrategy;
public:
	enum LOGIN_STATE
	{
		LS_NONE,		//δ��¼
		LS_LOGING,		//����¼
		LS_LOGED		//�ѵ�¼
	};
	ON_MESSAGE2
	template <const WORD CMD> bool onMessage(Zoic::Message &msg);
	bool parseMessage(Zoic::Message &msg);
	bool sendMessage(const Zoic::Message &msg);
	bool open(SOCKET sock,const sockaddr_in &addr);
	void close();
	//��֤�Ƿ����ĳȨ��
	bool checkAccess(DWORD access);
	//����RanaȨ��
	void setRanaAccess(DWORD access);
	//���͵�¼��Ӧ
	void sendLoginAck(RanaProto::ErrorType code);

	int scriptWriteBegin( const char* path, DWORD scriptLength, DWORD saveFileLength ,const char* scriptName, BYTE scriptType);
	void scriptWriteScriptAppend( const Rana_ScriptContent& content);
	void scriptWriteSaveFileAppend( const Rana_ScriptContent& content);
	int scriptWriteSave();
	int fileLoad( const char* filename );
	DWORD fileLoadGetLength();
	void fileLoadSend( WORD protoCode );

	int createTrigger( WORD space, const char*  triggerName );
	int deleteTrigger( WORD space, WORD handle);
	int modifyTriggerCondition( WORD space, WORD handle, const char* condition);// const P_TS_ModifyTrigger_Req&  modifyContext 
	int modifyTriggerAction( WORD space, WORD handle, const char* action);

	int triggerActionReceiveBegin(WORD space, WORD handle, DWORD actionLength);
	void triggerActionReceiveAppend( const Rana_TriggerAction& action);
	int triggerActionCommit();

	void createEvent( const P_TS_CreateEvent_Req&  eventContext );
	int deleteEvent( WORD space, WORD handle);

	int executeScript( DWORD spaceHandle, const Rana_ScriptContent& content);

	static RanaProto::Rana_ScriptPath swithPath2ScriptPath(const char* pPath);

	DWORD getRanaScriptLength() const;
	const BYTE* getRanaScriptBuf() const;
protected:
	bool openClientDefault(SOCKET sock,const sockaddr_in &addr);
	void closeClientDefault();

private:
	RanaClient();
	~RanaClient();

	//!< ��֤Ŀ¼��Ч��
	bool validatePath(
		const char* path,		//!< Ŀ¼����
		const char* scriptName	//!< �ű�����
		);

	DWORD		m_ranaAccess;	//RanaȨ��
	LOGIN_STATE m_loginState;

	std::string m_curWriteScriptPath;
	std::string m_curWriteSaveFilePath;
	RanaScriptManager::RanaScriptType m_curWriteScript;
	DWORD m_curWriteScriptLength;
	DWORD m_curWriteScriptPosition;
	BYTE* m_pCurWriteScriptBuf;
	DWORD m_curWriteSaveFileLength;
	DWORD m_curWriteSaveFilePosition;
	BYTE* m_pCurWriteSaveFileBuf;

	std::string m_curLoadScriptPath;
	BYTE* m_pCurLoadScriptBuf;
	DWORD m_curLoadScriptLength;
//	bool m_curScriptActive;

	char* m_curReceiveActionBuf;
	DWORD m_curReceiveActionLength;
	DWORD m_curReceiveActionPosition;
	WORD m_curReceiveActionSpace;
	WORD m_curReceiveActionHandle;

	std::string	m_netlogName;
};

inline bool RanaClient::checkAccess(DWORD access)
{
	return static_cast<bool>(m_ranaAccess && access);
}

inline void RanaClient::setRanaAccess(DWORD access)
{
	m_ranaAccess=access;
}

inline DWORD RanaClient::getRanaScriptLength() const
{
	return m_curLoadScriptLength;
}

inline const BYTE* RanaClient::getRanaScriptBuf() const
{
	return m_pCurLoadScriptBuf;
}

#endif 
