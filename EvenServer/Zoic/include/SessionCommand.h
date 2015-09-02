/*! @file
	@ingroup network
	@brief 定义连接命令模板
*/

#ifndef __ZOIC_SESSIONCOMMAND_H
#define __ZOIC_SESSIONCOMMAND_H

#include <list>
#include "ClassStorage.h"

namespace Zoic
{
	class Session;
	class Message;
	typedef bool (*MESSAGE_FUNC)(Session *pSession,Message &msg);

	/*! @brief 连接命令项
		@ingroup network
	*/
	struct CommandItem
	{
		WORD			command;	//!< 网络命令
		MESSAGE_FUNC 	function;	//!< 处理函数
	};

	typedef std::list<CommandItem> CommandClass;
	typedef ClassStorage<CommandClass> CommandStorage;
	inline CommandStorage & getCommandStorage()
	{
		static CommandStorage storage;
		return storage;
	}

	/*! @brief 连接命令模板
		@ingroup network
		@param T 连接类
	*/
	template<class T>
	class SessionCommand
	{
	public:
		//!构造函数
		SessionCommand(
			WORD command,			//!< 网络消息ID
			MESSAGE_FUNC function	//!< 处理函数
			);
	};
	
	template<class T> 
	SessionCommand<T>::SessionCommand(WORD command,MESSAGE_FUNC function)
	{
		CommandClass & commandclass=getCommandStorage().getClassInfo(typeid(T));
		CommandItem item;
		item.command=command;
		item.function=function;
		commandclass.push_back(item);
	}
}

#define SESSION_COMMAND(CLASS,COMMAND)	\
	template<> \
	bool CLASS::onMessage<COMMAND>(Zoic::Session *pSession,Zoic::Message &msg); \
	static Zoic::SessionCommand<CLASS> _##CLASS##_##COMMAND(COMMAND,CLASS::onMessage<COMMAND>); \
	template<> \
	bool CLASS::onMessage<COMMAND>(Zoic::Session *pSession,Zoic::Message &msg)

#define SESSION_COMMANDEX(CLASS,COMMAND,FUNCTION)	\
	static Zoic::SessionCommand<CLASS> _##CLASS##_##COMMAND(COMMAND,FUNCTION); 

#endif
