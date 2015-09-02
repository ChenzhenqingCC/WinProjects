/*! @file
	@ingroup network
	@brief �����������������
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
	
	/*! @brief �������������
		@ingroup network
	*/
	class CommandFactory:
		public NoCopy
	{
	public:
		typedef RunTimeContext<Session> RUNTIME_CHECKER;
		//!������Ϣ
		bool parseMessage(
			Session *pSession,	//!< ���Ӷ���
			Message &msg		//!< ������Ϣ
			);
	protected:
		CommandFactory(MESSAGE_FUNC defaultFunc = onDummyMessage);
		//!ע�����紦������
		void registerCommands(
			const std::type_info &typeinfo	//���������ص�������Ϣ
			);
		CommandTable m_commands;	//!< ���紦���
		void registerRunTimeCheck(
			RUNTIME_CHECKER* pChecker
			);
		RUNTIME_CHECKER* m_runTimeChecker;

		/*! @brief δע���������
		@exception Zoic::Exception �׳�δ�����쳣
		*/
		static bool onDummyMessage(
			Session *pSession,	//!< ���Ӷ���
			Message &msg		//!< ������Ϣ
			);
	private:
		//!ע�ᵥ��������Ϣ��������
		void registerCommand(
			WORD cmd,				//!< ������ϢID
			MESSAGE_FUNC func		//!< ������
			);
	};
}

#define REGISTER_COMMAND(cmd) registerCommand(cmd,onMessage<cmd>)

#endif
