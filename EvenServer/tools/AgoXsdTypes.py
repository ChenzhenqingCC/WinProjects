# -*- coding: iso-8859-1 -*-
XSD_TYPES= \
{
	"char"		:"xs:byte",
	"BYTE"		:"xs:unsignedByte",
	"short"		:"xs:short",
	"WORD"		:"xs:unsignedShort",
	"int"		:"xs:int",
	"long"		:"xs:int",
	"DWORD"		:"xs:unsignedInt",
	"QWORD"		:"xs:unsignedLong",
	"TIME_SEC"	:"xs:int",
	"TIME_MSEL"	:"xs:long",
	"POS2D_W"		:"POS2D_W",
	"POS2D_S"		:"POS2D_S",
	"GRADS_C"		:"GRADS_C",
}

LENGTH_USERNAME			=12			#Íæ¼ÒÕËºÅ³¤¶È
LENGTH_PASSWORD			=32			#ÃÜÂë³¤¶È
LENGTH_NICKNAME			=16			#êÇ³Æ/Ãû×Ö³¤¶È(Éè¶¨ÉÏÍ³Ò»)
LENGTH_ITEMNAME			=24			#µÀ¾ßÃû³Æ³¤¶È
LENGTH_ITEMCODE			=24			#µÀ¾ß±àÂë³¤¶È
LENGTH_FILEPATH			=255			#ÎÄ¼þÂ·¾¶³¤¶È
LENGTH_RANASCRIPT		=3000			#Rana½Å±¾³¤¶È

EXT_TYPES = \
{
	"UserName"			:["Zoic::String<%d>" % LENGTH_USERNAME,	"Íæ¼ÒÕËºÅ"],
	"PassWord"			:["Zoic::String<%d>" % LENGTH_PASSWORD,	"ÃÜÂë"],
	"NickName"			:["Zoic::String<%d>" % LENGTH_NICKNAME,	"½ÇÉ«(°üÀ¨Íæ¼Ò)êÇ³Æ"],
	"ItemName"			:["Zoic::String<%d>" % LENGTH_ITEMNAME,	"µÀ¾ßÃû³Æ"],
	"ItemCode"			:["Zoic::String<%d>" % LENGTH_ITEMCODE,	"µÀ¾ß±àÂë"],
	"FilePath"			:["Zoic::String<%d>" % LENGTH_FILEPATH,	"ÎÄ¼þÂ·¾¶"],
	"RanaScript"			:["Zoic::String<%d>" % LENGTH_RANASCRIPT,	"ÎÄ¼þÂ·¾¶"],
}
