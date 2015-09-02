/*! @file
	@ingroup network
	@brief 定义网络命令处理工厂类
*/

#ifndef __ZOIC_COMMANDFACTORY_H
#define __ZOIC_COMMANDFACTORY_H

#include "Types.h"
#include <utility>
#include <list>
#include "NoCopy.h"
#include "RunTimeContext.h"

namespace std
{
	class type_info;
}

namespace Zoic
{
	class Session;
	class Message;

	typedef bool (*MESSAGE_FUNC)(Session *pSession,Message &msg);
	typedef MESSAGE_FUNC CommandTable[0x10000];
	
	/*! @brief 网络命令处理工厂类
		@ingroup network
	*/
	class CommandFactory:
		public NoCopy
	{
	public:
		typedef RunTimeContext<Session> RUNTIME_CHECKER;
		//!解析消息
		bool parseMessage(
			Session *pSession,	//!< 连接对象
			Message &msg		//!< 网络消息
			);
	protected:
		CommandFactory(MESSAGE_FUNC defaultFunc = onDummyMessage);
		//!注册网络处理命令
		void registerCommands(
			const std::type_info &typeinfo	//与命令工厂相关的类型信息
			);
		CommandTable m_commands;	//!< 网络处理表
		void registerRunTimeCheck(
			RUNTIME_CHECKER* pChecker
			);
		RUNTIME_CHECKER* m_runTimeChecker;

		/*! @brief 未注册命令处理函数
		@exception Zoic::Exception 抛出未处理异常
		*/
		static bool onDummyMessage(
			Session *pSession,	//!< 连接对象
			Message &msg		//!< 网络消息
			);
	private:
		//!注册单个网络消息处理命令
		void registerCommand(
			WORD cmd,				//!< 网络消息ID
			MESSAGE_FUNC func		//!< 处理函数
			);
	};
}

#define REGISTER_COMMAND(cmd) registerCommand(cmd,onMessage<cmd>)

#endif
