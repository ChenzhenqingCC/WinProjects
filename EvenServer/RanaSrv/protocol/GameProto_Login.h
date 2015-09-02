#ifndef __PROTO_RANA_H
#define __PROTO_RANA_H

/*
Rana脚本服务相关协议
*/

#include <vector>
#include <Zoic/include/Message.h>
#include <Zoic/include/String.h>
#include <Zoic/include/StrCharArray.h>

namespace RanaProto
{
	typedef WORD ErrorType;
	static const ErrorType ERROR_OK								=0;		//0x0000 成功
	static const ErrorType ERROR_FAIL							=1;		//0x0001 失败
	static const ErrorType ERROR_RANA_SPACE						=2;		//0x0002 错误的Rana空间
	static const ErrorType ERROR_RANA_TRIGGER					=3;		//0x0003 错误的Rana触发器
	static const ErrorType ERROR_RANA_ACCESS					=4;		//0x0004 Rana权限不足
	static const ErrorType ERROR_RANA_TRIGGER_ALREADY_EXISTS	=5;		//0x0005 Rana触发器已存在
	static const ErrorType ERROR_RANA_EVENT						=6;		//0x0006 错误的Rana事件
}

namespace RanaProto
{//协议相关类型定义
	typedef		Zoic::String<100>		Rana_ScriptPath;	//Rana脚本路径
	typedef		Zoic::String<3000>		Rana_ScriptContent;	//Rana脚本内容
	typedef		Zoic::String<64>		Rana_SpaceName;	//Rana命名空间名称
	typedef		Zoic::String<64>		Rana_EventTypeCode;	//Rana事件类型码
	typedef		Zoic::String<64>		Rana_ScriptName;	//脚本名称
	typedef		Zoic::String<64>		Rana_TriggerName;	//Rana解发器名称
	typedef		Zoic::String<64>		Rana_EventName;	//Rana事件名称
	typedef		Zoic::String<300>		Rana_TriggerEventParam;	//事件参数
	typedef		Zoic::String<1000>		Rana_TriggerCondition;	//Rana触发器条件
	typedef		Zoic::String<3000>		Rana_TriggerAction;	//Rana触发器动作
	typedef		Zoic::String<32>		Rana_UserName;	//Rana用户名
	typedef		Zoic::String<32>		Rana_PassWord;	//Rana口令
	typedef		DWORD		FileLength;	//脚本长度类型
	const int MAX_SCRIPT_LENGTH = 60000;	//最大脚本长度
	const WORD RANA_STRING_PARAM_LEN = 128;
	struct Rana_EventType
	{
		BYTE				useCode;	//是否使用code
		WORD				id;			//布尔型值 (useCode==0)
		Rana_EventTypeCode	code;		//整型值 (useCode!=0)
	};
}

namespace RanaProto
{//协议定义
	//错误号声明

	//管理员登录请求 RanaClient->RanaService
	static const WORD TS_LOGIN_REQ=2;
	struct P_TS_Login_Req
	{
		Rana_UserName	username;	//用户名
		Rana_PassWord	password;	//密码
	};

	//管理员登录响应 RanaService->RanaClient
	static const WORD ST_LOGIN_ACK=1;
	struct P_ST_Login_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_FAIL
		*/
	};

	//上传脚本开始请求 RanaClient->RanaService
	static const WORD TS_UPLOADSCRIPTBEGIN_REQ=3;
	struct P_TS_UploadScriptBegin_Req
	{
		Rana_ScriptPath	filepath;		//lua脚本路径
		FileLength		scriptLength;	//lua脚本内容长度
		FileLength		saveFileLength;	//存档内容长度
		Rana_ScriptName	name;			//脚本名称
		BYTE			type;			//命名空间类型
	};

	//上传脚本开始响应 RanaService->RanaClient
	static const WORD ST_UPLOADSCRIPTBEGIN_ACK=4;
	struct P_ST_UploadScriptBegin_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_FAIL
		*/
	};

	//上传lua脚本内容 RanaClient->RanaService
	static const WORD TS_UPLOADSCRIPTCONTENT_NTF=5;
	struct P_TS_UploadScriptContent_Ntf
	{
		Rana_ScriptContent	content;	//脚本内容
	};

	//上传存档内容 RanaClient->RanaService
	static const WORD TS_UPLOADSAVEFILECONTENT_NTF=6;
	struct P_TS_UploadSaveFileContent_Ntf
	{
		Rana_ScriptContent	content;	//存档内容
	};

	//上传脚本结束请求 RanaClient->RanaService
	static const WORD TS_UPLOADSCRIPTEND_REQ=7;
	struct P_TS_UploadScriptEnd_Req
	{
	};

	//上传脚本结束响应 RanaService->RanaClient
	static const WORD ST_UPLOADSCRIPTEND_ACK=8;
	struct P_ST_UploadScriptEnd_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_FAIL
		*/
	};

	//下载脚本请求 RanaClient->RanaService
	static const WORD TS_DOWNLOADSCRIPT_REQ=9;
	struct P_TS_DownloadScript_Req
	{
		Rana_ScriptPath	filepath;	//文件名
	};

	//下载脚本响应 RanaService->RanaClient
	static const WORD ST_DOWNLOADSCRIPT_ACK=10;
	struct P_ST_DownloadScript_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_FAIL
		*/
		FileLength	length;	//内容长度 (code==ERROR_OK)
	};

	//下载脚本内容 RanaClient->RanaService
	static const WORD TS_DOWNLOADSCRIPTCONTENT_NTF=11;
	struct P_TS_DownloadScriptContent_Ntf
	{
		Rana_ScriptContent	content;	//脚本内容
	};

	//重新装载脚本请求 RanaClient->RanaService
	static const WORD TS_RELOADSPACE_REQ=12;
	struct P_TS_ReloadSpace_Req
	{
		WORD			handle;		//命名空间名柄
		Rana_ScriptPath	filepath;	//文件名
		BYTE			failedMethod;	//失败处理方式:0.开启成空空间;1.关闭空间;2.恢复为原有空间(暂时不支持)
	};

	//重新装载脚本响应 RanaService->RanaClient
	static const WORD ST_RELOADSPACE_ACK=13;
	struct P_ST_ReloadSpace_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
	};

	//列出Rana空间请求 RanaClient->RanaService
	static const WORD TS_LISTSPACE_REQ=14;
	struct P_TS_ListSpace_Req
	{
		BYTE	type;	//命名空间类型
	};

	//列出Rana空间响应 RanaService->RanaClient
	static const WORD ST_LISTSPACE_ACK=15;
	struct P_ST_ListSpace_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
		*/
		BYTE		type;	//命名空间类型
		WORD		total;	//该类型命名空间总数 (code==ERROR_OK)
	};

	//Rana空间列表内容 RanaService->RanaClient
	static const WORD ST_LISTSPACEBODY_NTF=16;
	struct P_ST_ListSpaceBody_Ntf
	{
		struct info	//命名空间信息
		{
			WORD			handle;		//命名空间句柄
			Rana_SpaceName	name;		//命名空间名称
			Rana_ScriptPath	filepath;	//命名空间所对应的存档路径
		};
		std::vector<info>	infos;	//命名空间信息
	};

	//暂停Rana空间请求 RanaClient->RanaService
	static const WORD TS_SUSPENDSPACE_REQ=17;
	struct P_TS_SuspendSpace_Req
	{
		WORD	handle;	//命名空间名柄
	};

	//暂停Rana空间响应 RanaService->RanaClient
	static const WORD ST_SUSPENDSPACE_ACK=18;
	struct P_ST_SuspendSpace_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
	};

	//启动Rana空间请求 RanaClient->RanaService
	static const WORD TS_RESUMESPACE_REQ=19;
	struct P_TS_ResumeSpace_Req
	{
		WORD	handle;	//命名空间名柄
	};

	//启动Rana空间响应 RanaService->RanaClient
	static const WORD ST_RESUMESPACE_ACK=20;
	struct P_ST_ResumeSpace_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
	};

	//列出触发器请求 RanaClient->RanaService
	static const WORD TS_LISTTRIGGER_REQ=21;
	struct P_TS_ListTrigger_Req
	{
		WORD	space;	//命名空间名柄
	};

	//列出触发器响应 RanaService->RanaClient
	static const WORD ST_LISTTRIGGER_ACK=22;
	struct P_ST_ListTrigger_Ack
	{
		ErrorType	code;	//操作码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
		WORD		space;	//命名空间名柄
		WORD		total;	//触发器总数 (code==ERROR_OK)
	};

	//触发器列表内容 RanaService->RanaClient
	static const WORD ST_LISTTRIGGERBODY_NTF=23;
	struct P_ST_ListTriggerBody_Ntf
	{
		struct info	//触发器信息
		{
			WORD				handle;	//触发器句柄
			Rana_TriggerName	name;	//触发器名称
			BYTE				isOn;	//触发器是否开启
		};
		std::vector<info>	infos;	//触发器信息
	};

	//暂停触发器请求 RanaClient->RanaService
	static const WORD TS_SUSPENDTRIGGER_REQ=24;
	struct P_TS_SuspendTrigger_Req
	{
		WORD	space;	//命名空间名柄
		WORD	handle;	//触发器名柄
	};

	//暂停触发器响应 RanaService->RanaClient
	static const WORD ST_SUSPENDTRIGGER_ACK=25;
	struct P_ST_SuspendTrigger_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
	};

	//启动触发器请求 RanaClient->RanaService
	static const WORD TS_RESUMETRIGGER_REQ=26;
	struct P_TS_ResumeTrigger_Req
	{
		WORD	space;	//命名空间名柄
		WORD	handle;	//触发器名柄
	};

	//启动触发器响应 RanaService->RanaClient
	static const WORD ST_RESUMETRIGGER_ACK=27;
	struct P_ST_ResumeTrigger_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
	};

	//创建触发器请求 RanaClient->RanaService
	static const WORD TS_CREATETRIGGER_REQ=28;
	struct P_TS_CreateTrigger_Req
	{
		WORD				space;	//命名空间名柄
		Rana_TriggerName	name;	//触发器名称
	};

	//创建触发器响应 RanaService->RanaClient
	static const WORD ST_CREATETRIGGER_ACK=29;
	struct P_ST_CreateTrigger_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER_ALREADY_EXISTS
			ERROR_FAIL
		*/
		WORD		space;	//命名空间名柄
		WORD		handle;	//新增的触发器名柄 (code==ERROR_OK)
	};

	//删除触发器请求 RanaClient->RanaService
	static const WORD TS_DELETETRIGGER_REQ=30;
	struct P_TS_DeleteTrigger_Req
	{
		WORD	space;	//命名空间名柄
		WORD	handle;	//触发器名柄
	};

	//删除触发器响应 RanaService->RanaClient
	static const WORD ST_DELETETRIGGER_ACK=31;
	struct P_ST_DeleteTrigger_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
	};

	//获得触发器详情请求 RanaClient->RanaService
	static const WORD TS_GETTRIGGERDETAIL_REQ=32;
	struct P_TS_GetTriggerDetail_Req
	{
		WORD	space;	//命名空间名柄
		WORD	handle;	//触发器名柄
	};

	//获得触发器详情响应 RanaService->RanaClient
	static const WORD ST_GETTRIGGERDETAIL_ACK=33;
	struct P_ST_GetTriggerDetail_Ack
	{
		ErrorType			code;			//返回码
		/*
			ERROR_OK
			ERERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
		std::vector<WORD>	eventhandles;	//与之关联的所有事件句柄 (code==ERROR_OK)
	};

	//修改触发器条件请求 RanaClient->RanaService
	static const WORD TS_MODIFYTRIGGERCONDITION_REQ=34;
	struct P_TS_ModifyTriggerCondition_Req
	{
		WORD					space;		//命名空间名柄
		WORD					handle;		//触发器名柄
		Rana_TriggerCondition	condition;	//触发器条件
	};

	//修改触发器条件响应 RanaService->RanaClient
	static const WORD ST_MODIFYTRIGGERCONDITION_ACK=35;
	struct P_ST_ModifyTriggerCondition_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
	};

	//修改触发器动作内容开始请求 RanaClient->RanaService
	static const WORD TS_MODIFYTRIGGERACTIONBEGIN_REQ=36;
	struct P_TS_ModifyTriggerActionBegin_Req
	{
		WORD	space;			//命名空间名柄
		WORD	handle;			//触发器名柄
		DWORD	actionLength;	//触发器动作脚本长度
	};

	//修改触发器动作内容开始响应 RanaService->RanaClient
	static const WORD ST_MODIFYTRIGGERACTIONBEGIN_ACK=37;
	struct P_ST_ModifyTriggerActionBegin_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
	};

	//修改触发器动作内容 RanaClient->RanaService
	static const WORD TS_MODIFYTRIGGERACTION_NTF=38;
	struct P_TS_ModifyTriggerAction_Ntf
	{
		Rana_TriggerAction	action;	//触发器动作
	};

	//上传触发器动作结束请求 RanaClient->RanaService
	static const WORD TS_MODIFYTRIGGERACTIONEND_REQ=39;
	struct P_TS_ModifyTriggerActionEnd_Req
	{
	};

	//修改触发器动作内容响应 RanaService->RanaClient
	static const WORD ST_MODIFYTRIGGERACTION_ACK=40;
	struct P_ST_ModifyTriggerAction_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
	};

	//列出事件请求 RanaClient->RanaService
	static const WORD TS_LISTEVENT_REQ=41;
	struct P_TS_ListEvent_Req
	{
		WORD	space;	//命名空间名柄
	};

	//列出事件响应 RanaService->RanaClient
	static const WORD ST_LISTEVENT_ACK=42;
	struct P_ST_ListEvent_Ack
	{
		ErrorType	code;	//操作码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
		*/
		WORD		space;	//命名空间名柄
		WORD		total;	//事件总数 (code==ERROR_OK)
	};

	//事件列表内容 RanaService->RanaClient
	static const WORD ST_LISTEVENTBODY_NTF=43;
	struct P_ST_ListEventBody_Ntf
	{
		struct info	//事件信息
		{
			WORD			handle;			//事件句柄
			Rana_EventName	eventname;		//事件名称
			Rana_EventType	type;			//事件类型
			WORD			triggerHandle;	//对应的触发器句柄
			BYTE			isOn;			//事件是否开启
		};
		std::vector<info>	infos;	//触发器信息
	};

	//暂停事件请求 RanaClient->RanaService
	static const WORD TS_SUSPENDEVENT_REQ=44;
	struct P_TS_SuspendEvent_Req
	{
		WORD	space;	//命名空间名柄
		WORD	handle;	//事件名柄
	};

	//暂停事件响应 RanaService->RanaClient
	static const WORD ST_SUSPENDEVENT_ACK=45;
	struct P_ST_SuspendEvent_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
		*/
	};

	//启动事件请求 RanaClient->RanaService
	static const WORD TS_RESUMEEVENT_REQ=46;
	struct P_TS_ResumeEvent_Req
	{
		WORD	space;	//命名空间名柄
		WORD	handle;	//事件名柄
	};

	//启动事件响应 RanaService->RanaClient
	static const WORD ST_RESUMEEVENT_ACK=47;
	struct P_ST_ResumeEvent_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
		*/
	};

	//创建事件请求 RanaClient->RanaService
	static const WORD TS_CREATEEVENT_REQ=48;
	struct P_TS_CreateEvent_Req
	{
		WORD								space;			//命名空间名柄
		Rana_EventName						eventname;		//事件名称
		Rana_EventType						eventType;		//事件
		std::vector<Rana_TriggerEventParam>	eventParams;	//事件参数
		WORD								triggerHandle;	//对应触发器句柄
	};

	//创建事件响应 RanaService->RanaClient
	static const WORD ST_CREATEEVENT_ACK=49;
	struct P_ST_CreateEvent_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
			ERROR_RANA_TRIGGER
			ERROR_FAIL
		*/
		WORD		space;	//命名空间名柄
		WORD		handle;	//新增的事件名柄 (code==ERROR_OK)
	};

	//删除事件请求 RanaClient->RanaService
	static const WORD TS_DELETEEVENT_REQ=50;
	struct P_TS_DeleteEvent_Req
	{
		WORD	space;	//命名空间名柄
		WORD	handle;	//事件名柄
	};

	//删除事件响应 RanaService->RanaClient
	static const WORD ST_DELETEEVENT_ACK=51;
	struct P_ST_DeleteEvent_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
		*/
	};

	//获得事件详情请求 RanaClient->RanaService
	static const WORD TS_GETEVENTDETAIL_REQ=52;
	struct P_TS_GetEventDetail_Req
	{
		WORD	space;	//命名空间名柄
		WORD	handle;	//事件名柄
	};

	//获得事件详情响应 RanaService->RanaClient
	static const WORD ST_GETEVENTDETAIL_ACK=53;
	struct P_ST_GetEventDetail_Ack
	{
		ErrorType		code;			//返回码
		/*
			ERROR_OK
			ERERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_EVENT
		*/
		Rana_EventName	eventname;		//事件名称 (code==ERROR_OK)
		Rana_EventType	type;			//事件类型 (code==ERROR_OK)
		BYTE			isOn;			//事件是否开启 (code==ERROR_OK)
		WORD			triggerHandle;	//对应触发器句柄 (code==ERROR_OK)
	};

	//执行脚本请求 RanaClient->RanaService
	static const WORD TS_EXECUTESCRIPT_REQ=56;
	struct P_TS_ExecuteScript_Req
	{
		WORD				space;		//命名空间名柄
		Rana_ScriptContent	content;	//脚本内容
	};

	//执行脚本响应 RanaService->RanaClient
	static const WORD ST_EXECUTESCRIPT_ACK=57;
	struct P_ST_ExecuteScript_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_FAIL
		*/
	};

	//列出脚本请求 RanaClient->RanaService
	static const WORD TS_LISTSCRIPT_REQ=58;
	struct P_TS_ListScript_Req
	{
		BYTE	type;	//命名空间类型
	};

	//列出脚本响应 RanaService->RanaClient
	static const WORD ST_LISTSCRIPT_ACK=59;
	struct P_ST_ListScript_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
		*/
		BYTE		type;	//命名空间类型
		WORD		total;	//该类型命名空间脚本总数 (code==ERROR_OK)
	};

	//列出脚本列表内容 RanaService->RanaClient
	static const WORD ST_LISTSCRIPTBODY_NTF=60;
	struct P_ST_ListScriptBody_Ntf
	{
		struct info	//脚本信息
		{
			Rana_ScriptPath	filepath;	//命名空间所对应的路径
			Rana_ScriptName	name;		//脚本名称
			BYTE			type;		//空间类型
		};
		std::vector<info>	infos;	//命名空间信息
	};

	//下载存档请求 RanaClient->RanaService
	static const WORD TS_DOWNLOADSAVEFILE_REQ=61;
	struct P_TS_DownloadSaveFile_Req
	{
		Rana_ScriptPath	filepath;	//文件名
	};

	//下载存档响应 RanaService->RanaClient
	static const WORD ST_DOWNLOADSAVEFILE_ACK=62;
	struct P_ST_DownloadSaveFile_Ack
	{
		ErrorType	code;	//返回码
		/*
			ERROR_OK
			ERROR_RANA_ACCESS
			ERROR_FAIL
		*/
		FileLength	length;	//内容长度 (code==ERROR_OK)
	};

	//下载存档内容 RanaService->RanaClient
	static const WORD ST_DOWNLOADSAVEFILECONTENT_NTF=63;
	struct P_ST_DownloadSaveFileContent_Ntf
	{
		Rana_ScriptContent	content;	//脚本内容
	};

	//获得Rana空间详情请求 RanaClient->RanaService
	static const WORD TS_GETSPACEDETAIL_REQ=64;
	struct P_TS_GetSpaceDetail_Req
	{
		WORD	space;	//命名空间名柄
	};

	//获得Rana空间详情响应 RanaService->RanaClient
	static const WORD ST_GETSPACEDETAIL_ACK=65;
	struct P_ST_GetSpaceDetail_Ack
	{
		ErrorType		code;			//返回码
		/*
			ERROR_OK
			ERERROR_RANA_ACCESS
			ERROR_RANA_SPACE
			ERROR_RANA_TRIGGER
		*/
		Rana_ScriptPath	startScript;	//启动加载的脚本 (code==ERROR_OK)
	};

	//修改Rana空间请求 RanaClient->RanaService
	static const WORD TS_MODIFYSPACE_REQ=66;
	struct P_TS_ModifySpace_Req
	{
		WORD			space;			//命名空间名柄
		Rana_ScriptPath	startScript;	//启动加载的脚本
	};

	//修改Rana空间响应 RanaService->RanaClient
	static const WORD ST_MODIFYSPACE_ACK=67;
	struct P_ST_ModifySpace_Ack
	{
		ErrorType	code;	//返回码
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
{//封包函数
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
