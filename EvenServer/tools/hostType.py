#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
import os

#��������
HOST_TYPE= \
{
	"Client"			:"C",
	"Gate"				:"G",
	"Zone"				:"Z",
	"Manager"			:"M",
	"Login"				:"L",
	"AIServer"			:"I",
	"Social"			:"S",
	"Admin"				:"D",
	"Daemon"			:"E",
	"History"			:"H",
	"Hotfix"			:"X",
	"Assistant"			:"A",
	"ManagerDBAgent"	:"T",
	"GlobalNpc"			:"N",
	"Stress"			:"Y",
	"Compress"		:"O"
}
#������������
HOST_LINK= \
{
	"Client"			:["Client","Gate","Stress"],
	"Gate"				:["Gate"],
	"Zone"				:["Zone"],
	"Manager"			:["Manager"],
	"Login"				:["Login","Manager"],
	"AIServer"			:["AIServer"],
	"Social"			:["Social"],
	"Admin"				:["Admin"],
	"Daemon"			:["Daemon"],
	"History"			:["History"],
	"Hotfix"			:["Hotfix"],
	"Assistant"			:["Assistant"],
	"ManagerDBAgent"	:["ManagerDBAgent"],
	"GlobalNpc"			:["GlobalNpc"],
	"Stress"			:["Stress","Gate"],
	"Compress"		:["Compress"],
}
#������Ծ�Է���(����Ծ�������������Э������ȱ������������ڷ�������Э���������Բ�Ӧ��������²����¼���ص�����)
HOST_ACTIVITY= \
{
	"Client"				:True,
	"Gate"					:True,
	"Zone"					:True,
	"Manager"				:True,
	"AIServer"				:True,
	"Login"					:True,
	"Social"				:True,
	"Admin"					:False,
	"Daemon"				:False,
	"History"				:True,
	"Hotfix"				:True,
	"Assistant"				:True,
	"ManagerDBAgent"		:False,
	"GlobalNpc"				:True,
	"Stress"				:False,
	"Compress"			:True,
}

