#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
import os

#Ö÷»ú¶¨Òå
HOST_TYPE= \
{
	"RanaClient"		:"T",
	"RanaService"			:"S",
}
#Ö÷»ú¹ØÁ´¶¨Òå
HOST_LINK= \
{
	"RanaClient"		:["Client"],
	"RanaService"			:["rana"],
}
#Ö÷»ú»îÔ¾ÐÔ·ÖÀà(²»»îÔ¾µÄÖ÷»úÓëÆäÏà¹ØÐ­ÒéºÅÓÅÏÈ±£Áô£¬½öÓÃÓÚÔÚ·¢Éú´òÂÒÐ­Òé²Ù×÷ºó¾ø¶Ô²»Ó¦·¢ÉúÓë¸üÐÂ²Ù×÷ÊÂ¼þÏà¹ØµÄÖ÷»ú)
HOST_ACTIVITY= \
{
	"RanaClient"					:False,
	"RanaService"				:False,
}

