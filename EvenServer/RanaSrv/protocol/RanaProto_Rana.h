#ifndef __PROTO_RANA_H
#define __PROTO_RANA_H

/*
Rana�ű��������Э��
*/

#include <vector>
#include <Zoic/include/Message.h>
#include <Zoic/include/String.h>
#include <Zoic/include/StrCharArray.h>

namespace RanaProto
{
	typedef WORD ErrorType;
	static const ErrorType ERROR_OK								=0;		//0x0000 �ɹ�
	static const ErrorType ERROR_FAIL							=1;		//0x0001 ʧ��
	static const ErrorType ERROR_RANA_SPACE						=2;		//0x0002 �����Rana�ռ�
	static const ErrorType ERROR_RANA_TRIGGER					=3;		//0x0003 �����Rana������
	static const ErrorType ERROR_RANA_ACCESS					=4;		//0x0004 RanaȨ�޲���
	static const ErrorType ERROR_RANA_TRIGGER_ALREADY_EXISTS	=5;		//0x0005 Rana�������Ѵ���
	static const ErrorType ERROR_RANA_EVENT						=6;		//0x0006 �����Rana�¼�
}

namespace RanaProto
{//Э��������Ͷ���
	typedef		Zoic::String<100>		Rana_ScriptPath;	//Rana�ű�·��
	typedef		Zoic::String<3000>		Rana_ScriptContent;	//Rana�ű�����
	typedef		Zoic::String<64>		Rana_SpaceName;	//Rana�����ռ�����
	typedef		Zoic::String<64>		Rana_EventTypeCode;	//Rana�¼�������
	typedef		Zoic::String<64>		Rana_ScriptName;	//�ű�����
	typedef		Zoic::String<64>		Rana_TriggerName;	//Rana�ⷢ������
	typedef		Zoic::String<64>		Rana_EventName;	//Rana�¼�����
	typedef		Zoic::String<300>		Rana_TriggerEventParam;	//�¼�����
	typedef		Zoic::String<1000>		Rana_TriggerCondition;	//Rana����������
	typedef		Zoic::String<3000>		Rana_TriggerAction;	//Rana����������
	typedef		Zoic::String<32>		Rana_UserName;	//Rana�û���
	typedef		Zoic::String<32>		Rana_PassWord;	//Rana����
	typedef		DWORD		FileLength;	//�ű���������
	const int MAX_SCRIPT_LENGTH = 60000;	//���ű�����
	const WORD RANA_STRING_PARAM_LEN = 128;
	struct Rana_EventType
	{
		BYTE				useCode;	//�Ƿ�ʹ��code
		WORD				id;			//������ֵ (useCode==0)
		Rana_EventTypeCode	code;		//����ֵ (useCode!=0)
	};
}

namespace RanaProto
{//Э�鶨��
	//���������

	//����Ա��¼���� RanaClient->RanaService
	static const WORD TS_LOGIN_REQ=2;
	struct P_TS_Login_Req
	{
		Rana_UserName	username;	//�û���
		Rana_PassWord	password;	//����
	};

	//����Ա��¼��Ӧ RanaService->RanaClient
	static const WORD ST_LOGIN_ACK=1;
	struct P_ST_Login_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_FAIL
		*/
	};

	//�ϴ��ű���ʼ���� RanaClient->RanaService
	static const WORD TS_UPLOADSCRIPTBEGIN_REQ=3;
	struct P_TS_UploadScriptBegin_Req
	{
		Rana_ScriptPath	filepath;		//lua�ű�·��
		FileLength		scriptLength;	//lua�ű����ݳ���
		FileLength		saveFileLength;	//�浵���ݳ���
		Rana_ScriptName	name;			//�ű�����
		BYTE			type;			//�����ռ�����
	};

	//�ϴ��ű���ʼ��Ӧ RanaService->RanaClient
	static const WORD ST_UPLOADSCRIPTBEGIN_ACK=4;
	struct P_ST_UploadScriptBegin_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_FAIL
		*/
	};

	//�ϴ�lua�ű����� RanaClient->RanaService
	static const WORD TS_UPLOADSCRIPTCONTENT_NTF=5;
	struct P_TS_UploadScriptContent_Ntf
	{
		Rana_ScriptContent	content;	//�ű�����
	};

	//�ϴ��浵���� RanaClient->RanaService
	static const WORD TS_UPLOADSAVEFILECONTENT_NTF=6;
	struct P_TS_UploadSaveFileContent_Ntf
	{
		Rana_ScriptContent	content;	//�浵����
	};

	//�ϴ��ű��������� RanaClient->RanaService
	static const WORD TS_UPLOADSCRIPTEND_REQ=7;
	struct P_TS_UploadScriptEnd_Req
	{
	};

	//�ϴ��ű�������Ӧ RanaService->RanaClient
	static const WORD ST_UPLOADSCRIPTEND_ACK=8;
	struct P_ST_UploadScriptEnd_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_FAIL
		*/
	};

	//���ؽű����� RanaClient->RanaService
	static const WORD TS_DOWNLOADSCRIPT_REQ=9;
	struct P_TS_DownloadScript_Req
	{
		Rana_ScriptPath	filepath;	//�ļ���
	};

	//���ؽű���Ӧ RanaService->RanaClient
	static const WORD ST_DOWNLOADSCRIPT_ACK=10;
	struct P_ST_DownloadScript_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_FAIL
		*/
		FileLength	length;	//���ݳ��� (code==ERROR_OK)
	};

	//���ؽű����� RanaClient->RanaService
	static const WORD TS_DOWNLOADSCRIPTCONTENT_NTF=11;
	struct P_TS_DownloadScriptContent_Ntf
	{
		Rana_ScriptContent	content;	//�ű�����
	};

	//����װ�ؽű����� RanaClient->RanaService
	static const WORD TS_RELOADSPACE_REQ=12;
	struct P_TS_ReloadSpace_Req
	{
		WORD			handle;		//�����ռ�����
		Rana_ScriptPath	filepath;	//�ļ���
		BYTE			failedMethod;	//ʧ�ܴ���ʽ:0.�����ɿտռ�;1.�رտռ�;2.�ָ�Ϊԭ�пռ�(��ʱ��֧��)
	};

	//����װ�ؽű���Ӧ RanaService->RanaClient
	static const WORD ST_RELOADSPACE_ACK=13;
	struct P_ST_ReloadSpace_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
	};

	//�г�Rana�ռ����� RanaClient->RanaService
	static const WORD TS_LISTSPACE_REQ=14;
	struct P_TS_ListSpace_Req
	{
		BYTE	type;	//�����ռ�����
	};

	//�г�Rana�ռ���Ӧ RanaService->RanaClient
	static const WORD ST_LISTSPACE_ACK=15;
	struct P_ST_ListSpace_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
		*/
		BYTE		type;	//�����ռ�����
		WORD		total;	//�����������ռ����� (code==ERROR_OK)
	};

	//Rana�ռ��б����� RanaService->RanaClient
	static const WORD ST_LISTSPACEBODY_NTF=16;
	struct P_ST_ListSpaceBody_Ntf
	{
		struct info	//�����ռ���Ϣ
		{
			WORD			handle;		//�����ռ���
			Rana_SpaceName	name;		//�����ռ�����
			Rana_ScriptPath	filepath;	//�����ռ�����Ӧ�Ĵ浵·��
		};
		std::vector<info>	infos;	//�����ռ���Ϣ
	};

	//��ͣRana�ռ����� RanaClient->RanaService
	static const WORD TS_SUSPENDSPACE_REQ=17;
	struct P_TS_SuspendSpace_Req
	{
		WORD	handle;	//�����ռ�����
	};

	//��ͣRana�ռ���Ӧ RanaService->RanaClient
	static const WORD ST_SUSPENDSPACE_ACK=18;
	struct P_ST_SuspendSpace_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
	};

	//����Rana�ռ����� RanaClient->RanaService
	static const WORD TS_RESUMESPACE_REQ=19;
	struct P_TS_ResumeSpace_Req
	{
		WORD	handle;	//�����ռ�����
	};

	//����Rana�ռ���Ӧ RanaService->RanaClient
	static const WORD ST_RESUMESPACE_ACK=20;
	struct P_ST_ResumeSpace_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
	};

	//�г����������� RanaClient->RanaService
	static const WORD TS_LISTTRIGGER_REQ=21;
	struct P_TS_ListTrigger_Req
	{
		WORD	space;	//�����ռ�����
	};

	//�г���������Ӧ RanaService->RanaClient
	static const WORD ST_LISTTRIGGER_ACK=22;
	struct P_ST_ListTrigger_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
		WORD		space;	//�����ռ�����
		WORD		total;	//���������� (code==ERROR_OK)
	};

	//�������б����� RanaService->RanaClient
	static const WORD ST_LISTTRIGGERBODY_NTF=23;
	struct P_ST_ListTriggerBody_Ntf
	{
		struct info	//��������Ϣ
		{
			WORD				handle;	//���������
			Rana_TriggerName	name;	//����������
			BYTE				isOn;	//�������Ƿ���
		};
		std::vector<info>	infos;	//��������Ϣ
	};

	//��ͣ���������� RanaClient->RanaService
	static const WORD TS_SUSPENDTRIGGER_REQ=24;
	struct P_TS_SuspendTrigger_Req
	{
		WORD	space;	//�����ռ�����
		WORD	handle;	//����������
	};

	//��ͣ��������Ӧ RanaService->RanaClient
	static const WORD ST_SUSPENDTRIGGER_ACK=25;
	struct P_ST_SuspendTrigger_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
	};

	//�������������� RanaClient->RanaService
	static const WORD TS_RESUMETRIGGER_REQ=26;
	struct P_TS_ResumeTrigger_Req
	{
		WORD	space;	//�����ռ�����
		WORD	handle;	//����������
	};

	//������������Ӧ RanaService->RanaClient
	static const WORD ST_RESUMETRIGGER_ACK=27;
	struct P_ST_ResumeTrigger_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
	};

	//�������������� RanaClient->RanaService
	static const WORD TS_CREATETRIGGER_REQ=28;
	struct P_TS_CreateTrigger_Req
	{
		WORD				space;	//�����ռ�����
		Rana_TriggerName	name;	//����������
	};

	//������������Ӧ RanaService->RanaClient
	static const WORD ST_CREATETRIGGER_ACK=29;
	struct P_ST_CreateTrigger_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER_ALREADY_EXISTS
			ERROR_FAIL
		*/
		WORD		space;	//�����ռ�����
		WORD		handle;	//�����Ĵ��������� (code==ERROR_OK)
	};

	//ɾ������������ RanaClient->RanaService
	static const WORD TS_DELETETRIGGER_REQ=30;
	struct P_TS_DeleteTrigger_Req
	{
		WORD	space;	//�����ռ�����
		WORD	handle;	//����������
	};

	//ɾ����������Ӧ RanaService->RanaClient
	static const WORD ST_DELETETRIGGER_ACK=31;
	struct P_ST_DeleteTrigger_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
	};

	//��ô������������� RanaClient->RanaService
	static const WORD TS_GETTRIGGERDETAIL_REQ=32;
	struct P_TS_GetTriggerDetail_Req
	{
		WORD	space;	//�����ռ�����
		WORD	handle;	//����������
	};

	//��ô�����������Ӧ RanaService->RanaClient
	static const WORD ST_GETTRIGGERDETAIL_ACK=33;
	struct P_ST_GetTriggerDetail_Ack
	{
		ErrorType			code;			//������
		/*
			ERROR_OK
			ERERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
		std::vector<WORD>	eventhandles;	//��֮�����������¼���� (code==ERROR_OK)
	};

	//�޸Ĵ������������� RanaClient->RanaService
	static const WORD TS_MODIFYTRIGGERCONDITION_REQ=34;
	struct P_TS_ModifyTriggerCondition_Req
	{
		WORD					space;		//�����ռ�����
		WORD					handle;		//����������
		Rana_TriggerCondition	condition;	//����������
	};

	//�޸Ĵ�����������Ӧ RanaService->RanaClient
	static const WORD ST_MODIFYTRIGGERCONDITION_ACK=35;
	struct P_ST_ModifyTriggerCondition_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
	};

	//�޸Ĵ������������ݿ�ʼ���� RanaClient->RanaService
	static const WORD TS_MODIFYTRIGGERACTIONBEGIN_REQ=36;
	struct P_TS_ModifyTriggerActionBegin_Req
	{
		WORD	space;			//�����ռ�����
		WORD	handle;			//����������
		DWORD	actionLength;	//�����������ű�����
	};

	//�޸Ĵ������������ݿ�ʼ��Ӧ RanaService->RanaClient
	static const WORD ST_MODIFYTRIGGERACTIONBEGIN_ACK=37;
	struct P_ST_ModifyTriggerActionBegin_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
	};

	//�޸Ĵ������������� RanaClient->RanaService
	static const WORD TS_MODIFYTRIGGERACTION_NTF=38;
	struct P_TS_ModifyTriggerAction_Ntf
	{
		Rana_TriggerAction	action;	//����������
	};

	//�ϴ������������������� RanaClient->RanaService
	static const WORD TS_MODIFYTRIGGERACTIONEND_REQ=39;
	struct P_TS_ModifyTriggerActionEnd_Req
	{
	};

	//�޸Ĵ���������������Ӧ RanaService->RanaClient
	static const WORD ST_MODIFYTRIGGERACTION_ACK=40;
	struct P_ST_ModifyTriggerAction_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
	};

	//�г��¼����� RanaClient->RanaService
	static const WORD TS_LISTEVENT_REQ=41;
	struct P_TS_ListEvent_Req
	{
		WORD	space;	//�����ռ�����
	};

	//�г��¼���Ӧ RanaService->RanaClient
	static const WORD ST_LISTEVENT_ACK=42;
	struct P_ST_ListEvent_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
		WORD		space;	//�����ռ�����
		WORD		total;	//�¼����� (code==ERROR_OK)
	};

	//�¼��б����� RanaService->RanaClient
	static const WORD ST_LISTEVENTBODY_NTF=43;
	struct P_ST_ListEventBody_Ntf
	{
		struct info	//�¼���Ϣ
		{
			WORD			handle;			//�¼����
			Rana_EventName	eventname;		//�¼�����
			Rana_EventType	type;			//�¼�����
			WORD			triggerHandle;	//��Ӧ�Ĵ��������
			BYTE			isOn;			//�¼��Ƿ���
		};
		std::vector<info>	infos;	//��������Ϣ
	};

	//��ͣ�¼����� RanaClient->RanaService
	static const WORD TS_SUSPENDEVENT_REQ=44;
	struct P_TS_SuspendEvent_Req
	{
		WORD	space;	//�����ռ�����
		WORD	handle;	//�¼�����
	};

	//��ͣ�¼���Ӧ RanaService->RanaClient
	static const WORD ST_SUSPENDEVENT_ACK=45;
	struct P_ST_SuspendEvent_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
		*/
	};

	//�����¼����� RanaClient->RanaService
	static const WORD TS_RESUMEEVENT_REQ=46;
	struct P_TS_ResumeEvent_Req
	{
		WORD	space;	//�����ռ�����
		WORD	handle;	//�¼�����
	};

	//�����¼���Ӧ RanaService->RanaClient
	static const WORD ST_RESUMEEVENT_ACK=47;
	struct P_ST_ResumeEvent_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
		*/
	};

	//�����¼����� RanaClient->RanaService
	static const WORD TS_CREATEEVENT_REQ=48;
	struct P_TS_CreateEvent_Req
	{
		WORD								space;			//�����ռ�����
		Rana_EventName						eventname;		//�¼�����
		Rana_EventType						eventType;		//�¼�
		std::vector<Rana_TriggerEventParam>	eventParams;	//�¼�����
		WORD								triggerHandle;	//��Ӧ���������
	};

	//�����¼���Ӧ RanaService->RanaClient
	static const WORD ST_CREATEEVENT_ACK=49;
	struct P_ST_CreateEvent_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
		WORD		space;	//�����ռ�����
		WORD		handle;	//�������¼����� (code==ERROR_OK)
	};

	//ɾ���¼����� RanaClient->RanaService
	static const WORD TS_DELETEEVENT_REQ=50;
	struct P_TS_DeleteEvent_Req
	{
		WORD	space;	//�����ռ�����
		WORD	handle;	//�¼�����
	};

	//ɾ���¼���Ӧ RanaService->RanaClient
	static const WORD ST_DELETEEVENT_ACK=51;
	struct P_ST_DeleteEvent_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
		*/
	};

	//����¼��������� RanaClient->RanaService
	static const WORD TS_GETEVENTDETAIL_REQ=52;
	struct P_TS_GetEventDetail_Req
	{
		WORD	space;	//�����ռ�����
		WORD	handle;	//�¼�����
	};

	//����¼�������Ӧ RanaService->RanaClient
	static const WORD ST_GETEVENTDETAIL_ACK=53;
	struct P_ST_GetEventDetail_Ack
	{
		ErrorType		code;			//������
		/*
			ERROR_OK
			ERERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
		*/
		Rana_EventName	eventname;		//�¼����� (code==ERROR_OK)
		Rana_EventType	type;			//�¼����� (code==ERROR_OK)
		BYTE			isOn;			//�¼��Ƿ��� (code==ERROR_OK)
		WORD			triggerHandle;	//��Ӧ��������� (code==ERROR_OK)
	};

	//ִ�нű����� RanaClient->RanaService
	static const WORD TS_EXECUTESCRIPT_REQ=56;
	struct P_TS_ExecuteScript_Req
	{
		WORD				space;		//�����ռ�����
		Rana_ScriptContent	content;	//�ű�����
	};

	//ִ�нű���Ӧ RanaService->RanaClient
	static const WORD ST_EXECUTESCRIPT_ACK=57;
	struct P_ST_ExecuteScript_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_FAIL
		*/
	};

	//�г��ű����� RanaClient->RanaService
	static const WORD TS_LISTSCRIPT_REQ=58;
	struct P_TS_ListScript_Req
	{
		BYTE	type;	//�����ռ�����
	};

	//�г��ű���Ӧ RanaService->RanaClient
	static const WORD ST_LISTSCRIPT_ACK=59;
	struct P_ST_ListScript_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
		*/
		BYTE		type;	//�����ռ�����
		WORD		total;	//�����������ռ�ű����� (code==ERROR_OK)
	};

	//�г��ű��б����� RanaService->RanaClient
	static const WORD ST_LISTSCRIPTBODY_NTF=60;
	struct P_ST_ListScriptBody_Ntf
	{
		struct info	//�ű���Ϣ
		{
			Rana_ScriptPath	filepath;	//�����ռ�����Ӧ��·��
			Rana_ScriptName	name;		//�ű�����
			BYTE			type;		//�ռ�����
		};
		std::vector<info>	infos;	//�����ռ���Ϣ
	};

	//���ش浵���� RanaClient->RanaService
	static const WORD TS_DOWNLOADSAVEFILE_REQ=61;
	struct P_TS_DownloadSaveFile_Req
	{
		Rana_ScriptPath	filepath;	//�ļ���
	};

	//���ش浵��Ӧ RanaService->RanaClient
	static const WORD ST_DOWNLOADSAVEFILE_ACK=62;
	struct P_ST_DownloadSaveFile_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_FAIL
		*/
		FileLength	length;	//���ݳ��� (code==ERROR_OK)
	};

	//���ش浵���� RanaService->RanaClient
	static const WORD ST_DOWNLOADSAVEFILECONTENT_NTF=63;
	struct P_ST_DownloadSaveFileContent_Ntf
	{
		Rana_ScriptContent	content;	//�ű�����
	};

	//���Rana�ռ��������� RanaClient->RanaService
	static const WORD TS_GETSPACEDETAIL_REQ=64;
	struct P_TS_GetSpaceDetail_Req
	{
		WORD	space;	//�����ռ�����
	};

	//���Rana�ռ�������Ӧ RanaService->RanaClient
	static const WORD ST_GETSPACEDETAIL_ACK=65;
	struct P_ST_GetSpaceDetail_Ack
	{
		ErrorType		code;			//������
		/*
			ERROR_OK
			ERERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
		Rana_ScriptPath	startScript;	//�������صĽű� (code==ERROR_OK)
	};

	//�޸�Rana�ռ����� RanaClient->RanaService
	static const WORD TS_MODIFYSPACE_REQ=66;
	struct P_TS_ModifySpace_Req
	{
		WORD			space;			//�����ռ�����
		Rana_ScriptPath	startScript;	//�������صĽű�
	};

	//�޸�Rana�ռ���Ӧ RanaService->RanaClient
	static const WORD ST_MODIFYSPACE_ACK=67;
	struct P_ST_ModifySpace_Ack
	{
		ErrorType	code;	//������
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
	};


}

namespace RanaProto
{//�������
	inline Zoic::Message & operator << ( Zoic::Message &m,const Rana_EventType & p )
	{
		m << p.useCode;
		if(p.useCode==0)
		{
			m << p.id;
		}
		if(p.useCode!=0)
		{
			m << p.code;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,Rana_EventType & p )
	{
		m >> p.useCode;
		if(p.useCode==0)
		{
			m >> p.id;
		}
		if(p.useCode!=0)
		{
			m >> p.code;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_Login_Req & p )
	{
		m << p.username;
		m << p.password;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_Login_Req & p )
	{
		m >> p.username;
		m >> p.password;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_Login_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_Login_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_UploadScriptBegin_Req & p )
	{
		m << p.filepath;
		m << p.scriptLength;
		m << p.saveFileLength;
		m << p.name;
		m << p.type;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_UploadScriptBegin_Req & p )
	{
		m >> p.filepath;
		m >> p.scriptLength;
		m >> p.saveFileLength;
		m >> p.name;
		m >> p.type;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_UploadScriptBegin_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_UploadScriptBegin_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_UploadScriptContent_Ntf & p )
	{
		m << p.content;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_UploadScriptContent_Ntf & p )
	{
		m >> p.content;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_UploadSaveFileContent_Ntf & p )
	{
		m << p.content;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_UploadSaveFileContent_Ntf & p )
	{
		m >> p.content;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_UploadScriptEnd_Req & p )
	{
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_UploadScriptEnd_Req & p )
	{
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_UploadScriptEnd_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_UploadScriptEnd_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_DownloadScript_Req & p )
	{
		m << p.filepath;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_DownloadScript_Req & p )
	{
		m >> p.filepath;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_DownloadScript_Ack & p )
	{
		m << p.code;
		if(p.code==ERROR_OK)
		{
			m << p.length;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_DownloadScript_Ack & p )
	{
		m >> p.code;
		if(p.code==ERROR_OK)
		{
			m >> p.length;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_DownloadScriptContent_Ntf & p )
	{
		m << p.content;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_DownloadScriptContent_Ntf & p )
	{
		m >> p.content;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ReloadSpace_Req & p )
	{
		m << p.handle;
		m << p.filepath;
		m << p.failedMethod;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ReloadSpace_Req & p )
	{
		m >> p.handle;
		m >> p.filepath;
		if(m.getRemainingRData() > 0)
		{
			m >> p.failedMethod;
		}
		else
		{
			p.failedMethod = 0;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ReloadSpace_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ReloadSpace_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ListSpace_Req & p )
	{
		m << p.type;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ListSpace_Req & p )
	{
		m >> p.type;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListSpace_Ack & p )
	{
		m << p.code;
		m << p.type;
		if(p.code==ERROR_OK)
		{
			m << p.total;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListSpace_Ack & p )
	{
		m >> p.code;
		m >> p.type;
		if(p.code==ERROR_OK)
		{
			m >> p.total;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListSpaceBody_Ntf::info & p )
	{
		m << p.handle;
		m << p.name;
		m << p.filepath;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListSpaceBody_Ntf::info & p )
	{
		m >> p.handle;
		m >> p.name;
		m >> p.filepath;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListSpaceBody_Ntf & p )
	{
		m << p.infos;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListSpaceBody_Ntf & p )
	{
		m >> p.infos;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_SuspendSpace_Req & p )
	{
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_SuspendSpace_Req & p )
	{
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_SuspendSpace_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_SuspendSpace_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ResumeSpace_Req & p )
	{
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ResumeSpace_Req & p )
	{
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ResumeSpace_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ResumeSpace_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ListTrigger_Req & p )
	{
		m << p.space;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ListTrigger_Req & p )
	{
		m >> p.space;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListTrigger_Ack & p )
	{
		m << p.code;
		m << p.space;
		if(p.code==ERROR_OK)
		{
			m << p.total;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListTrigger_Ack & p )
	{
		m >> p.code;
		m >> p.space;
		if(p.code==ERROR_OK)
		{
			m >> p.total;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListTriggerBody_Ntf::info & p )
	{
		m << p.handle;
		m << p.name;
		m << p.isOn;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListTriggerBody_Ntf::info & p )
	{
		m >> p.handle;
		m >> p.name;
		m >> p.isOn;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListTriggerBody_Ntf & p )
	{
		m << p.infos;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListTriggerBody_Ntf & p )
	{
		m >> p.infos;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_SuspendTrigger_Req & p )
	{
		m << p.space;
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_SuspendTrigger_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_SuspendTrigger_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_SuspendTrigger_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ResumeTrigger_Req & p )
	{
		m << p.space;
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ResumeTrigger_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ResumeTrigger_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ResumeTrigger_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_CreateTrigger_Req & p )
	{
		m << p.space;
		m << p.name;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_CreateTrigger_Req & p )
	{
		m >> p.space;
		m >> p.name;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_CreateTrigger_Ack & p )
	{
		m << p.code;
		m << p.space;
		if(p.code==ERROR_OK)
		{
			m << p.handle;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_CreateTrigger_Ack & p )
	{
		m >> p.code;
		m >> p.space;
		if(p.code==ERROR_OK)
		{
			m >> p.handle;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_DeleteTrigger_Req & p )
	{
		m << p.space;
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_DeleteTrigger_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_DeleteTrigger_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_DeleteTrigger_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_GetTriggerDetail_Req & p )
	{
		m << p.space;
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_GetTriggerDetail_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_GetTriggerDetail_Ack & p )
	{
		m << p.code;
		if(p.code==ERROR_OK)
		{
			m << p.eventhandles;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_GetTriggerDetail_Ack & p )
	{
		m >> p.code;
		if(p.code==ERROR_OK)
		{
			m >> p.eventhandles;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ModifyTriggerCondition_Req & p )
	{
		m << p.space;
		m << p.handle;
		m << p.condition;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ModifyTriggerCondition_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		m >> p.condition;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ModifyTriggerCondition_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ModifyTriggerCondition_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ModifyTriggerActionBegin_Req & p )
	{
		m << p.space;
		m << p.handle;
		m << p.actionLength;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ModifyTriggerActionBegin_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		m >> p.actionLength;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ModifyTriggerActionBegin_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ModifyTriggerActionBegin_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ModifyTriggerAction_Ntf & p )
	{
		m << p.action;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ModifyTriggerAction_Ntf & p )
	{
		m >> p.action;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ModifyTriggerActionEnd_Req & p )
	{
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ModifyTriggerActionEnd_Req & p )
	{
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ModifyTriggerAction_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ModifyTriggerAction_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ListEvent_Req & p )
	{
		m << p.space;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ListEvent_Req & p )
	{
		m >> p.space;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListEvent_Ack & p )
	{
		m << p.code;
		m << p.space;
		if(p.code==ERROR_OK)
		{
			m << p.total;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListEvent_Ack & p )
	{
		m >> p.code;
		m >> p.space;
		if(p.code==ERROR_OK)
		{
			m >> p.total;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListEventBody_Ntf::info & p )
	{
		m << p.handle;
		m << p.eventname;
		m << p.type;
		m << p.triggerHandle;
		m << p.isOn;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListEventBody_Ntf::info & p )
	{
		m >> p.handle;
		m >> p.eventname;
		m >> p.type;
		m >> p.triggerHandle;
		m >> p.isOn;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListEventBody_Ntf & p )
	{
		m << p.infos;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListEventBody_Ntf & p )
	{
		m >> p.infos;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_SuspendEvent_Req & p )
	{
		m << p.space;
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_SuspendEvent_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_SuspendEvent_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_SuspendEvent_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ResumeEvent_Req & p )
	{
		m << p.space;
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ResumeEvent_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ResumeEvent_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ResumeEvent_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_CreateEvent_Req & p )
	{
		m << p.space;
		m << p.eventname;
		m << p.eventType;
		m << p.eventParams;
		m << p.triggerHandle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_CreateEvent_Req & p )
	{
		m >> p.space;
		m >> p.eventname;
		m >> p.eventType;
		m >> p.eventParams;
		m >> p.triggerHandle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_CreateEvent_Ack & p )
	{
		m << p.code;
		m << p.space;
		if(p.code==ERROR_OK)
		{
			m << p.handle;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_CreateEvent_Ack & p )
	{
		m >> p.code;
		m >> p.space;
		if(p.code==ERROR_OK)
		{
			m >> p.handle;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_DeleteEvent_Req & p )
	{
		m << p.space;
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_DeleteEvent_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_DeleteEvent_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_DeleteEvent_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_GetEventDetail_Req & p )
	{
		m << p.space;
		m << p.handle;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_GetEventDetail_Req & p )
	{
		m >> p.space;
		m >> p.handle;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_GetEventDetail_Ack & p )
	{
		m << p.code;
		if(p.code==ERROR_OK)
		{
			m << p.eventname;
			m << p.type;
			m << p.isOn;
			m << p.triggerHandle;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_GetEventDetail_Ack & p )
	{
		m >> p.code;
		if(p.code==ERROR_OK)
		{
			m >> p.eventname;
			m >> p.type;
			m >> p.isOn;
			m >> p.triggerHandle;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ExecuteScript_Req & p )
	{
		m << p.space;
		m << p.content;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ExecuteScript_Req & p )
	{
		m >> p.space;
		m >> p.content;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ExecuteScript_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ExecuteScript_Ack & p )
	{
		m >> p.code;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ListScript_Req & p )
	{
		m << p.type;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ListScript_Req & p )
	{
		m >> p.type;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListScript_Ack & p )
	{
		m << p.code;
		m << p.type;
		if(p.code==ERROR_OK)
		{
			m << p.total;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListScript_Ack & p )
	{
		m >> p.code;
		m >> p.type;
		if(p.code==ERROR_OK)
		{
			m >> p.total;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListScriptBody_Ntf::info & p )
	{
		m << p.filepath;
		m << p.name;
		m << p.type;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListScriptBody_Ntf::info & p )
	{
		m >> p.filepath;
		m >> p.name;
		m >> p.type;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ListScriptBody_Ntf & p )
	{
		m << p.infos;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ListScriptBody_Ntf & p )
	{
		m >> p.infos;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_DownloadSaveFile_Req & p )
	{
		m << p.filepath;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_DownloadSaveFile_Req & p )
	{
		m >> p.filepath;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_DownloadSaveFile_Ack & p )
	{
		m << p.code;
		if(p.code==ERROR_OK)
		{
			m << p.length;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_DownloadSaveFile_Ack & p )
	{
		m >> p.code;
		if(p.code==ERROR_OK)
		{
			m >> p.length;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_DownloadSaveFileContent_Ntf & p )
	{
		m << p.content;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_DownloadSaveFileContent_Ntf & p )
	{
		m >> p.content;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_GetSpaceDetail_Req & p )
	{
		m << p.space;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_GetSpaceDetail_Req & p )
	{
		m >> p.space;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_GetSpaceDetail_Ack & p )
	{
		m << p.code;
		if(p.code==ERROR_OK)
		{
			m << p.startScript;
		}
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_GetSpaceDetail_Ack & p )
	{
		m >> p.code;
		if(p.code==ERROR_OK)
		{
			m >> p.startScript;
		}
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_TS_ModifySpace_Req & p )
	{
		m << p.space;
		m << p.startScript;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_TS_ModifySpace_Req & p )
	{
		m >> p.space;
		m >> p.startScript;
		return m;
	}

	inline Zoic::Message & operator << ( Zoic::Message &m,const P_ST_ModifySpace_Ack & p )
	{
		m << p.code;
		return m;
	}

	inline Zoic::Message & operator >> ( Zoic::Message &m,P_ST_ModifySpace_Ack & p )
	{
		m >> p.code;
		return m;
	}

}

#endif
