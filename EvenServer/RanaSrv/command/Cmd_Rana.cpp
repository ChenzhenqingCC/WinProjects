#include <Zoic/include/VectorMessage.h>
#include <Zoic/include/Message.h>
#include "rana/RanaHeader.h"
#include <Zoic/include/SessionCommand.h>
#include "../include/RanaClient.h"
#include "../include/RanaService.h"
#include "../include/AccessMask.h"
#include "rana/RanaSpaceBank.h"
#include "protocol/RanaProto_Rana.h"

using namespace RanaProto;
using namespace Zoic;

namespace RanaSrv
{
	BYTE link_Cmd_Rana;
}

static void pushListScriptBody(std::vector<P_ST_ListScriptBody_Ntf::info> & vec,RanaScriptManager::RanaScriptType* const& pRanaScriptType)
{
	P_ST_ListScriptBody_Ntf::info info;
	info.filepath =pRanaScriptType->path;
	info.name =pRanaScriptType->scriptName;
	info.type =pRanaScriptType->spaceType;
	vec.push_back(info);
}

static void pushListTriggerBody(std::vector<P_ST_ListTriggerBody_Ntf::info> & vec,RanaTrigger* const & pRanaTrigger)
{
	P_ST_ListTriggerBody_Ntf::info info;
	info.handle = pRanaTrigger->getHandle();
	info.name = pRanaTrigger->getName();
	info.isOn = pRanaTrigger->getOnOff();
	vec.push_back(info);
}

static void pushListSpaceBody(std::vector<P_ST_ListSpaceBody_Ntf::info> & vec,const std::map<RanaSpaceName,RanaSpace*>::value_type & mappair)
{
	RanaSpace* pRanaSpace = mappair.second;
	P_ST_ListSpaceBody_Ntf::info tempInfo;
	tempInfo.handle = pRanaSpace->getHandle().handle;
	tempInfo.name = pRanaSpace->getSpaceName();
	tempInfo.filepath = RanaClient::swithPath2ScriptPath(pRanaSpace->getFileName());
	vec.push_back(tempInfo);
}

static void pushListEventBody(std::vector<P_ST_ListEventBody_Ntf::info> & vec,RanaEvent* const & pRanaEvent)
{
	P_ST_ListEventBody_Ntf::info tempInfo;
	tempInfo.handle = pRanaEvent->getHandle();
	tempInfo.eventname = pRanaEvent->getName();
	const char* pEventTypeCode = g_RanaService->getStrategy()->GetEventCodeFromType( pRanaEvent->getType() );
	if(pEventTypeCode && strcmp(pEventTypeCode,"") )
	{
		tempInfo.type.useCode = 1;
		tempInfo.type.code = pEventTypeCode;
	}
	else
	{
		tempInfo.type.useCode = 0;
		tempInfo.type.id = pRanaEvent->getType();
	}
	RanaTrigger* pTrigger = pRanaEvent->getTrigger();
	tempInfo.triggerHandle = pTrigger ? pTrigger->getHandle() : ~0;
	tempInfo.isOn = pRanaEvent->getOnOff();
	vec.push_back(tempInfo);
}
/*
//生成事件参数
static bool makeEventArgs(const std::vector<RanaVariant> & src_args,std::vector<Rana_Event::Argument> & dest_args)
{
	Rana_Event::Argument arg;
	std::vector<RanaVariant>::const_iterator it;
	for(it = src_args.begin();it!=src_args.end();++it)
	{
		const RanaVariant& variantData = *it;
		switch(it->type)
		{
			case RanaVariant::VT_BOOL:
				arg.type=Rana_Event::Argument::AT_BOOL;
				arg.body.bVal=variantData.body.bVal;
				break;
			case RanaVariant::VT_INTEGER:
				arg.type=Rana_Event::Argument::AT_INTEGER;
				arg.body.iVal=variantData.body.iVal;
				break;
			case RanaVariant::VT_STRING:
				arg.type=Rana_Event::Argument::AT_STRING;
				{
					int len = static_cast<int>( strlen( variantData.body.sVal ) );
					if( len > arg.body.sVal.getLengthLimit() )
					{
						return false;
					}
					arg.body.sVal.setString(variantData.body.sVal);
				}
				break;
			default:
				return false;
		}
		dest_args.push_back(arg);
	}
	return true;
}
*/

SESSION_COMMAND(RanaClient, PL_LOGIN_REQ)
{
	RanaClient * self = static_cast<RanaClient *>(pSession);
	P_PL_Login_Req pkt;
	msg >> pkt;
	
	return true;
}

SESSION_COMMAND(RanaClient,TS_LOGIN_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_Login_Req pkt;
	msg >> pkt;
	DWORD dwUserAccess;
	int rs = g_RanaService->getStrategy()->certifcate(pkt.username, pkt.password,&dwUserAccess);
	if(rs<0)
	{
		return true;
	}
	if(rs == 0)
	{
		self->setRanaAccess(dwUserAccess);
		self->sendLoginAck(ERROR_OK);
	}
	else
	{
		self->sendLoginAck(ERROR_FAIL);
	}
	return true;
}

SESSION_COMMAND(RanaClient,TS_UPLOADSCRIPTBEGIN_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_UploadScriptBegin_Req pkt;
	msg >> pkt;
	P_ST_UploadScriptBegin_Ack pktSend;
	if( self->checkAccess(RSAM_SCRIPT_UPLOAD) )
	{
		int rs = self->scriptWriteBegin(pkt.filepath,pkt.scriptLength,pkt.saveFileLength,pkt.name,pkt.type);
		if(rs == 0)
		{
			pktSend.code = ERROR_OK;
		}
		else
		{
			pktSend.code = ERROR_FAIL;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_UPLOADSCRIPTBEGIN_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);
	return true;
}

SESSION_COMMAND(RanaClient,TS_UPLOADSCRIPTCONTENT_NTF)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_UploadScriptContent_Ntf pkt;
	msg >> pkt;
	if( self->checkAccess(RSAM_SCRIPT_UPLOAD) )
	{
		self->scriptWriteScriptAppend(pkt.content);
	}
	return true;
}

SESSION_COMMAND(RanaClient,TS_UPLOADSAVEFILECONTENT_NTF)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_UploadSaveFileContent_Ntf pkt;
	msg >> pkt;
	if( self->checkAccess(RSAM_SCRIPT_UPLOAD) )
	{
		self->scriptWriteSaveFileAppend(pkt.content);
	}
	return true;
}

SESSION_COMMAND(RanaClient,TS_UPLOADSCRIPTEND_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_UploadScriptEnd_Req pkt;
	msg >> pkt;
	P_ST_UploadScriptBegin_Ack pktSend;
	if( self->checkAccess(RSAM_SCRIPT_UPLOAD) )
	{
		int rs = self->scriptWriteSave();
		if(rs == 0)
		{
			pktSend.code = ERROR_OK;
		}
		else
		{
			pktSend.code = ERROR_FAIL;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_UPLOADSCRIPTEND_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);
	return true;
}

SESSION_COMMAND(RanaClient,TS_DOWNLOADSCRIPT_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_DownloadScript_Req pkt;
	msg >> pkt;
	P_ST_DownloadScript_Ack pktSend;
	if( self->checkAccess(RSAM_SCRIPT_DOWNLOAD) )
	{
		char pbuf[1024];
		strcpy(pbuf,pkt.filepath);
		strcat(pbuf,LUA_SCRIPT_EXT);
		int rs = self->fileLoad(pbuf);
		if(rs == 0)
		{
			pktSend.code = ERROR_OK;
			pktSend.length = self->fileLoadGetLength();
		}
		else
		{
			pktSend.code = ERROR_FAIL;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_DOWNLOADSCRIPT_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);
	if(pktSend.code == ERROR_OK)
	{
		self->fileLoadSend(TS_DOWNLOADSCRIPTCONTENT_NTF);
	}
	return true;
}

SESSION_COMMAND(RanaClient,TS_DOWNLOADSAVEFILE_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_DownloadSaveFile_Req pkt;
	msg >> pkt;
	P_ST_DownloadSaveFile_Ack pktSend;
	if( self->checkAccess(RSAM_SCRIPT_DOWNLOAD) )
	{
		char pbuf[1024];
		strcpy(pbuf,pkt.filepath);
		strcat(pbuf,LUA_SAVEFILE_EXT);
		int rs = self->fileLoad(pbuf);
		if(rs == 0)
		{
			pktSend.code = ERROR_OK;
			pktSend.length = self->fileLoadGetLength();
		}
		else
		{
			pktSend.code = ERROR_FAIL;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_DOWNLOADSAVEFILE_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);
	if(pktSend.code == ERROR_OK)
	{
		self->fileLoadSend(ST_DOWNLOADSAVEFILECONTENT_NTF);
	}
	return true;
}


SESSION_COMMAND(RanaClient,TS_RELOADSPACE_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ReloadSpace_Req pkt;
	msg >> pkt;
	P_ST_ReloadSpace_Ack pktSend;
	if( self->checkAccess(RSAM_SPACE_RELOAD) )
	{
		std::string strFilePath = "";
		const char* pBasePath = g_RanaService->getStrategy()->GetBaseScriptPath();
		if(pBasePath)
		{
			strFilePath = pBasePath;
		}
		strFilePath += pkt.filepath;
		strFilePath += ".lua";
		RanaSpace* pSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.handle );
		if( pSpace && RanaSpaceBank::getInstance()->reloadSpace( pSpace->getHandle(), strFilePath.c_str(),pkt.failedMethod != 0 ) == RANA_OK )
		{
			pktSend.code = ERROR_OK;
		}
		else
		{
			pktSend.code = ERROR_RANA_SPACE;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_RELOADSPACE_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);
	return true;
}

SESSION_COMMAND(RanaClient,TS_LISTSPACE_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ListSpace_Req pkt;
	msg >> pkt;
	P_ST_ListSpace_Ack pktSend;
	pktSend.type = pkt.type;
	const std::map<RanaSpaceName,RanaSpace*>& spaceList = RanaSpaceBank::getInstance()->listSpaces(pkt.type);
	if( self->checkAccess(RSAM_QUERY) )
	{
		pktSend.total = (WORD)spaceList.size();
		pktSend.code = ERROR_OK;
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_LISTSPACE_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);
	
	if(pktSend.code==ERROR_OK)
	{
		sendVectorMessage<P_ST_ListSpaceBody_Ntf::info>(
			MessageSender<RanaClient>(self,&RanaClient::sendMessage),
			ST_LISTSPACEBODY_NTF,
			spaceList,
			&pushListSpaceBody);
	}

	return true;
}

SESSION_COMMAND(RanaClient,TS_SUSPENDSPACE_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_SuspendSpace_Req pkt;
	msg >> pkt;
	P_ST_SuspendSpace_Ack pktSend;
	if( self->checkAccess(RSAM_TRIGGER_MANTAIN) )
	{
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.handle );
		if(pRanaSpace)
		{
			RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
			if(pRanaTriggerBank)
			{
				std::vector<RanaTrigger*> triggers;
				pRanaTriggerBank->listTriggers(triggers);
				for(size_t i = 0; i<triggers.size();++i)
				{
					triggers[i]->setOnOff(false);
				}
				pktSend.code = ERROR_OK;
			}
			else
			{
				pktSend.code = ERROR_RANA_SPACE;
			}
		}
		else
		{
			pktSend.code = ERROR_RANA_SPACE;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_SUSPENDSPACE_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);

	return true;
}

SESSION_COMMAND(RanaClient,TS_RESUMESPACE_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ResumeSpace_Req pkt;
	msg >> pkt;
	P_ST_ResumeSpace_Ack pktSend;
	if( self->checkAccess(RSAM_TRIGGER_MANTAIN) )
	{
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.handle );
		if(pRanaSpace)
		{
			RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
			if(pRanaTriggerBank)
			{
				std::vector<RanaTrigger*> triggers;
				pRanaTriggerBank->listTriggers(triggers);
				for(size_t i = 0; i<triggers.size();++i)
				{
					triggers[i]->setOnOff(true);
				}
				pktSend.code = ERROR_OK;
			}
			else
			{
				pktSend.code = ERROR_RANA_SPACE;
			}
		}
		else
		{
			pktSend.code = ERROR_RANA_SPACE;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_RESUMESPACE_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);

	return true;
}

SESSION_COMMAND(RanaClient,TS_LISTTRIGGER_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ListTrigger_Req pkt;
	msg >> pkt;
	P_ST_ListTrigger_Ack pktSend;
	pktSend.space = pkt.space;

	Message msgSend;
	if( !self->checkAccess(RSAM_QUERY) )
	{
		pktSend.code = ERROR_RANA_ACCESS;
		msgSend.ID(ST_LISTTRIGGER_ACK) << pktSend;
		self->sendMessage(msgSend);
		return true;
	}
	RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.space );
	if(!pRanaSpace)
	{
		pktSend.code = ERROR_RANA_SPACE;
		msgSend.ID(ST_LISTTRIGGER_ACK) << pktSend;
		self->sendMessage(msgSend);
		return true;
	}
	RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
	if(!pRanaTriggerBank)
	{
		pktSend.code = ERROR_RANA_SPACE;
		msgSend.ID(ST_LISTTRIGGER_ACK) << pktSend;
		self->sendMessage(msgSend);
		return true;
	}
	std::vector<RanaTrigger*> output;
	pRanaTriggerBank->listTriggers(output);
	pktSend.total = (WORD)output.size();
	pktSend.code = ERROR_OK;
	msgSend.ID(ST_LISTTRIGGER_ACK) << pktSend;
	self->sendMessage(msgSend);
	
	sendVectorMessage<P_ST_ListTriggerBody_Ntf::info>(
		MessageSender<RanaClient>(self,&RanaClient::sendMessage),
		ST_LISTTRIGGERBODY_NTF,
		output,
		&pushListTriggerBody);

	return true;
}

SESSION_COMMAND(RanaClient,TS_SUSPENDTRIGGER_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_SuspendTrigger_Req pkt;
	msg >> pkt;
	P_ST_SuspendTrigger_Ack pktSend;
	if( self->checkAccess(RSAM_TRIGGER_MANTAIN) )
	{
		pktSend.code = ERROR_RANA_SPACE;
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.space );
		if(pRanaSpace)
		{
			RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
			if(pRanaTriggerBank)
			{
				RanaTrigger* pTrigger = pRanaTriggerBank->getTrigger(pkt.handle);
				if( pTrigger )
				{
					pTrigger->setOnOff(false);
					pktSend.code = ERROR_OK;
				}
				else
				{
					pktSend.code = ERROR_RANA_TRIGGER;
				}
			}
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_SUSPENDTRIGGER_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);

	return true;
}

SESSION_COMMAND(RanaClient,TS_RESUMETRIGGER_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ResumeTrigger_Req pkt;
	msg >> pkt;
	P_ST_ResumeTrigger_Ack pktSend;
	if( self->checkAccess(RSAM_TRIGGER_MANTAIN) )
	{
		pktSend.code = ERROR_RANA_SPACE;
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.space );
		if(pRanaSpace)
		{
			RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
			if(pRanaTriggerBank)
			{
				RanaTrigger* pTrigger = pRanaTriggerBank->getTrigger(pkt.handle);
				if( pTrigger )
				{
					pTrigger->setOnOff(true);
					pktSend.code = ERROR_OK;
				}
				else
				{
					pktSend.code = ERROR_RANA_TRIGGER;
				}
			}
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_RESUMETRIGGER_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);

	return true;
}

SESSION_COMMAND(RanaClient,TS_CREATETRIGGER_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_CreateTrigger_Req pkt;
	msg >> pkt;
	self->createTrigger(pkt.space,pkt.name);
	return true;
}

SESSION_COMMAND(RanaClient,TS_DELETETRIGGER_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_DeleteTrigger_Req pkt;
	msg >> pkt;
	self->deleteTrigger(pkt.space, pkt.handle);

	return true;
}

SESSION_COMMAND(RanaClient,TS_GETTRIGGERDETAIL_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_GetTriggerDetail_Req pkt;
	msg >> pkt;
	P_ST_GetTriggerDetail_Ack pktSend;
	if( self->checkAccess(RSAM_QUERY) )
	{
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.space );
		if(pRanaSpace)
		{
			RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
			if(pRanaTriggerBank )
			{
				RanaTrigger* pRanaTrigger = pRanaTriggerBank->getTrigger(pkt.handle);
//				pktSend.condition = pRanaTrigger->getConditionContent();
//				pktSend.action = pRanaTrigger->getActionContent();
				std::vector<RanaEvent*> events;
				pRanaTriggerBank->listEventByTrigger(pkt.handle,events);
				for(size_t i=0;i<events.size();++i)
				{
					pktSend.eventhandles.push_back(events[i]->getHandle());
				}
				pktSend.code = ERROR_OK;
			}
			else
			{
				pktSend.code = ERROR_RANA_SPACE;
			}
		}
		else
		{
			pktSend.code = ERROR_RANA_SPACE;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_GETTRIGGERDETAIL_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);

	return true;
}

SESSION_COMMAND(RanaClient,TS_MODIFYTRIGGERCONDITION_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ModifyTriggerCondition_Req pkt;
	msg >> pkt;
	self->modifyTriggerCondition(pkt.space,pkt.handle,pkt.condition);

	return true;
}



SESSION_COMMAND(RanaClient,TS_MODIFYTRIGGERACTIONBEGIN_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ModifyTriggerActionBegin_Req pkt;
	msg >> pkt;
	P_ST_ModifyTriggerActionBegin_Ack pktSend;
	if( self->checkAccess(RSAM_TRIGGER_DELETE) )
	{
		int rs = self->triggerActionReceiveBegin(pkt.space,pkt.handle,pkt.actionLength);
		if(rs == 0)
		{
			pktSend.code = ERROR_OK;
		}
		else
		{
			pktSend.code = ERROR_FAIL;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_MODIFYTRIGGERACTIONBEGIN_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);
	return true;
}

SESSION_COMMAND(RanaClient,TS_MODIFYTRIGGERACTION_NTF)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ModifyTriggerAction_Ntf pkt;
	msg >> pkt;
	if( self->checkAccess(RSAM_TRIGGER_DELETE) )
	{
		self->triggerActionReceiveAppend(pkt.action);
	}
	return true;
}

SESSION_COMMAND(RanaClient,TS_MODIFYTRIGGERACTIONEND_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ModifyTriggerActionEnd_Req pkt;
	msg >> pkt;
	P_ST_ModifyTriggerAction_Ack pktSend;
	if( self->checkAccess(RSAM_TRIGGER_DELETE) )
	{
		int rs = self->triggerActionCommit();
		if(rs == 0)
		{
			pktSend.code = ERROR_OK;
		}
		else
		{
			pktSend.code = ERROR_FAIL;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_MODIFYTRIGGERACTION_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);
	return true;
}

SESSION_COMMAND(RanaClient,TS_LISTEVENT_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ListEvent_Req pkt;
	msg >> pkt;
	P_ST_ListEvent_Ack pktSend;
	pktSend.space = pkt.space;
	Message msgSend;
	if( !self->checkAccess(RSAM_QUERY) )
	{
		pktSend.code = ERROR_RANA_ACCESS;
		msgSend.ID(ST_LISTEVENT_ACK) << pktSend;
		self->sendMessage(msgSend);
		return true;
	}
	RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.space );
	if(!pRanaSpace)
	{
		pktSend.code = ERROR_RANA_SPACE;
		msgSend.ID(ST_LISTEVENT_ACK) << pktSend;
		self->sendMessage(msgSend);
		return true;
	}
	RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
	if(!pRanaTriggerBank)
	{
		pktSend.code = ERROR_RANA_SPACE;
		msgSend.ID(ST_LISTEVENT_ACK) << pktSend;
		self->sendMessage(msgSend);
		return true;
	}
	std::vector<RanaEvent*> output;
	pRanaTriggerBank->listEvents(output);
	pktSend.total = (WORD)output.size();
	pktSend.code = ERROR_OK;

	msgSend.ID(ST_LISTEVENT_ACK) << pktSend;
	self->sendMessage(msgSend);
	
	if(pktSend.code==ERROR_OK)
	{
		sendVectorMessage<P_ST_ListEventBody_Ntf::info>(
			MessageSender<RanaClient>(self,&RanaClient::sendMessage),
			ST_LISTEVENTBODY_NTF,
			output,
			&pushListEventBody);
		std::map< RanaWORD,RanaEvent*>::const_iterator it;
	}

	return true;
}

SESSION_COMMAND(RanaClient,TS_SUSPENDEVENT_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_SuspendEvent_Req pkt;
	msg >> pkt;
	P_ST_SuspendEvent_Ack pktSend;
	if( self->checkAccess(RSAM_EVENT_MANTAIN) )
	{
		pktSend.code = ERROR_RANA_SPACE;
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.space );
		if(pRanaSpace)
		{
			RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
			if(pRanaTriggerBank)
			{
				RanaEvent* pEvent = pRanaTriggerBank->getEvent( pkt.handle );
				if( pEvent )
				{
					pEvent->setOnOff(false);
					pktSend.code = ERROR_OK;
				}
				else
				{
					pktSend.code = ERROR_RANA_TRIGGER;
				}
			}
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_SUSPENDEVENT_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);

	return true;
}

SESSION_COMMAND(RanaClient,TS_RESUMEEVENT_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ResumeEvent_Req pkt;
	msg >> pkt;
	P_ST_ResumeEvent_Ack pktSend;
	if( self->checkAccess(RSAM_EVENT_MANTAIN) )
	{
		pktSend.code = ERROR_RANA_SPACE;
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.space );
		if(pRanaSpace)
		{
			RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
			if(pRanaTriggerBank)
			{
				RanaEvent* pEvent = pRanaTriggerBank->getEvent( pkt.handle );
				if( pEvent )
				{
					pEvent->setOnOff(true);
					pktSend.code = ERROR_OK;
				}
				else
				{
					pktSend.code = ERROR_RANA_TRIGGER;
				}
			}
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_RESUMEEVENT_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);

	return true;
}

SESSION_COMMAND(RanaClient,TS_CREATEEVENT_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_CreateEvent_Req pkt;
	msg >> pkt;
	self->createEvent(pkt);

	return true;
}

SESSION_COMMAND(RanaClient,TS_DELETEEVENT_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_DeleteEvent_Req pkt;
	msg >> pkt;
	self->deleteEvent(pkt.space, pkt.handle);

	return true;
}

SESSION_COMMAND(RanaClient,TS_GETEVENTDETAIL_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_GetEventDetail_Req pkt;
	msg >> pkt;
	P_ST_GetEventDetail_Ack pktSend;
	if( self->checkAccess(RSAM_QUERY) )
	{
		RanaSpace* pRanaSpace = RanaSpaceBank::getInstance()->forceFindSpaceByIndex( pkt.space );
		if(pRanaSpace)
		{
			RanaTriggerBank* pRanaTriggerBank = pRanaSpace->getTriggerBank();
			if(pRanaTriggerBank)
			{
				RanaEvent* pRanaEvent = pRanaTriggerBank->getEvent(pkt.handle);
				if(pRanaEvent)
				{
					RanaTrigger* pTrigger = pRanaEvent->getTrigger();
					pktSend.triggerHandle = pTrigger ? pTrigger->getHandle() : ~0;
					pktSend.eventname = pRanaEvent->getName();
					pktSend.isOn = pRanaEvent->getOnOff();
					const char* pEventTypeCode = g_RanaService->getStrategy()->GetEventCodeFromType( pRanaEvent->getType() );
					if(pEventTypeCode && strcmp(pEventTypeCode,"") )
					{
						pktSend.type.useCode = 1;
						pktSend.type.code = pEventTypeCode;
					}
					else
					{
						pktSend.type.useCode = 0;
						pktSend.type.id = pRanaEvent->getType();
					}
//					std::vector<RanaVariant> EvntArgs;
//					pRanaEvent->getArgument(EvntArgs);
//					makeEventArgs(EvntArgs,pktSend.event.args);
					pktSend.code = ERROR_OK;
				}
				else
				{
					pktSend.code = ERROR_RANA_EVENT;
				}
				
			}
			else
			{
				pktSend.code = ERROR_RANA_SPACE;
			}
		}
		else
		{
			pktSend.code = ERROR_RANA_SPACE;
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	Message msgSend;
	msgSend.ID(ST_GETEVENTDETAIL_ACK);
	msgSend << pktSend;
	self->sendMessage(msgSend);

	return true;
}

SESSION_COMMAND(RanaClient,TS_EXECUTESCRIPT_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ExecuteScript_Req pkt;
	msg >> pkt;
	self->executeScript(pkt.space,pkt.content);

	return true;
}


SESSION_COMMAND(RanaClient,TS_LISTSCRIPT_REQ)
{
	RanaClient * self=static_cast<RanaClient *>(pSession);
	P_TS_ListScript_Req pkt;
	msg >> pkt;
	P_ST_ListScript_Ack pktSend;
	P_ST_ListScriptBody_Ntf pktSendBody;
	pktSend.type = pkt.type;
	Message msgSend;
	if( self->checkAccess(RSAM_QUERY) )
	{
		const RanaScriptManager::SCRIPT_SET * pScriptSet = g_RanaScriptManager->getPaths(pkt.type);
		pktSend.code = ERROR_OK;
		if(pScriptSet)
		{
			pktSend.total=(WORD)pScriptSet->size();
			msgSend.ID(ST_LISTSCRIPT_ACK) << pktSend;
			self->sendMessage(msgSend);

			sendVectorMessage<P_ST_ListScriptBody_Ntf::info>(
				MessageSender<RanaClient>(self,&RanaClient::sendMessage),
				ST_LISTSCRIPTBODY_NTF,
				*pScriptSet,
				&pushListScriptBody);
		}
		else
		{
			pktSend.total = 0;
			msgSend.ID(ST_LISTSCRIPT_ACK) << pktSend;
			self->sendMessage(msgSend);
		}
	}
	else
	{
		pktSend.code = ERROR_RANA_ACCESS;
	}
	return true;
}

