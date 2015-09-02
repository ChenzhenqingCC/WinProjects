/*! @file
	@ingroup network
	@brief ������������ģ��
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

	/*! @brief ����������
		@ingroup network
	*/
	struct CommandItem
	{
		WORD			command;	//!< ��������
		MESSAGE_FUNC 	function;	//!< ������
	};

	typedef std::list<CommandItem> CommandClass;
	typedef ClassStorage<CommandClass> CommandStorage;
	inline CommandStorage & getCommandStorage()
	{
		static CommandStorage storage;
		return storage;
	}

	/*! @brief ��������ģ��
		@ingroup network
		@param T ������
	*/
	template<class T>
	class SessionCommand
	{
	public:
		//!���캯��
		SessionCommand(
			WORD command,			//!< ������ϢID
			MESSAGE_FUNC function	//!< ������
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
