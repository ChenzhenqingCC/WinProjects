#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
import os

#Ö÷»ú¶¨Òå
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
#Ö÷»ú¹ØÁ´¶¨Òå
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
#Ö÷»ú»îÔ¾ÐÔ·ÖÀà(²»»îÔ¾µÄÖ÷»úÓëÆäÏà¹ØÐ­ÒéºÅÓÅÏÈ±£Áô£¬½öÓÃÓÚÔÚ·¢Éú´òÂÒÐ­Òé²Ù×÷ºó¾ø¶Ô²»Ó¦·¢ÉúÓë¸üÐÂ²Ù×÷ÊÂ¼þÏà¹ØµÄÖ÷»ú)
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

