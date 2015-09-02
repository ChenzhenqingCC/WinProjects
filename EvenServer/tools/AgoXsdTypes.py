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

LENGTH_USERNAME			=12			#����˺ų���
LENGTH_PASSWORD			=32			#���볤��
LENGTH_NICKNAME			=16			#�ǳ�/���ֳ���(�趨��ͳһ)
LENGTH_ITEMNAME			=24			#�������Ƴ���
LENGTH_ITEMCODE			=24			#���߱��볤��
LENGTH_FILEPATH			=255			#�ļ�·������
LENGTH_RANASCRIPT		=3000			#Rana�ű�����

EXT_TYPES = \
{
	"UserName"			:["Zoic::String<%d>" % LENGTH_USERNAME,	"����˺�"],
	"PassWord"			:["Zoic::String<%d>" % LENGTH_PASSWORD,	"����"],
	"NickName"			:["Zoic::String<%d>" % LENGTH_NICKNAME,	"��ɫ(�������)�ǳ�"],
	"ItemName"			:["Zoic::String<%d>" % LENGTH_ITEMNAME,	"��������"],
	"ItemCode"			:["Zoic::String<%d>" % LENGTH_ITEMCODE,	"���߱���"],
	"FilePath"			:["Zoic::String<%d>" % LENGTH_FILEPATH,	"�ļ�·��"],
	"RanaScript"			:["Zoic::String<%d>" % LENGTH_RANASCRIPT,	"�ļ�·��"],
}
