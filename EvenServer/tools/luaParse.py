#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
import xml.dom.minidom
import os
import re
import glob
from utils import *
from config import *

#ÔØÈëÔ­ÏÈµÄÊý¾Ý¶¨Òå
import sys
sys.path.append(PATH_PROTO_SOURCE)
try:
	from cmd_def import *
except ImportError:
	sys.exit(0)
try:
	from err_def import *
except ImportError:
	sys.exit(0)

#´¥·¢Æ÷º¯Êý
trigger_function=""
#´¥·¢Æ÷×¢²á
trigger_register=""
#ÏûÏ¢½âÎöº¯Êý
parse_function=""
#³£Á¿¶¨Òå
const_vars=""
#³£Á¿¶¨Òå
alias_types=""

#µ±Ç°ÃüÁî¶¨Òå

def handleProto(proto):
	handleTypes(getElement(proto,"Types"))
	#ÏûÏ¢´¦Àí
	messages=getElements(getElement(proto,"Messages"),"Message")
	for message in messages:
		handleMessage(message)

def handleMessage(message):
	descs=getText(getElement(message,"Description")).split("\n");
	name=getText(getElement(message,"Name"))
	type=getText(getElement(message,"Type"))
	fr_hosts=getText(getElement(message,"From")).replace("\n",",")
	to_hosts=getText(getElement(message,"To")).replace("\n",",")
	hosts_info=getHostLink(fr_hosts,to_hosts)
	fr=hosts_info[0]
	to=hosts_info[1]
	global trigger_function
	global trigger_register
	cmd = HOST_TYPE[fr] + SEPARATOR_HOST + HOST_TYPE[to] + SEPARATOR_PROTO + name.upper() + SEPARATOR_ACTION + ACTION_TYPE[type].upper()
	p_name = "P" + SEPARATOR_PACKET + HOST_TYPE[fr] + SEPARATOR_HOST + HOST_TYPE[to] + SEPARATOR_PROTO + name + SEPARATOR_ACTION + ACTION_TYPE[type]

	#´¥·¢Æ÷×¢²á
	try:
		trigger_register+=tabStr(1,"TriggerRegisterParserMsgEvent(\"on%s\",\"parse_%d\",%d);\n",cmd,CMDS[cmd],CMDS[cmd])
	except KeyError:
		print "Can't find %s" % cmd
		sys.exit(0)
	#´¥·¢Æ÷º¯Êý
	trigger_function+=tabStr(0,"on%s_Conditions=True;\n",cmd);
	trigger_function+=tabStr(0,"function on%s_Actions()\n",cmd);
	trigger_function+=tabStr(1,"Action(\"%s\",\"%s\",parse%s);\n",p_name,"%s\\n%s->%s" % (descs[0],fr_hosts,hosts_info),p_name);
	trigger_function+=tabStr(0,"end\n\n");
	#½âÎöº¯Êý
	handleMembers(getElement(message,"Content"),p_name)

def expandName(type_name,sub_types):
	if sub_types.has_key(type_name):
		return sub_types[type_name]
	else:
		return type_name


def pyParseTypeName(type_name,sub_types):
	#std::vector<class>
	if type_name[0:11]=="std_vector_" and type_name[-1:]=="_":
		#vectorÐÎÊ½£¬²»¿ÉÄÜ´¦ÔÚin_union·½Ê½ÏÂ,²»Ö§³Ö¶àÖØvector
		type_name=expandName(type_name[11:-1],sub_types)
		return ["parseVector", [ "parse%s" % type_name ] ]
	elif type_name[0:14]=="Zoic_VarArray_" and type_name[-1:]=="_":
		#ArrayÐÎÊ½£¬²»¿ÉÄÜ´¦ÔÚin_union·½Ê½ÏÂ,²»Ö§³Ö¶àÖØArray
		type_name=type_name[14:-1].split(",")[0]
		type_name=expandName(type_name,sub_types)
		return ["parseVarArray", [ "parse%s" % type_name ] ]
	elif type_name[0:14]=="Zoic_FixArray_" and type_name[-1:]=="_":
		splitParams = type_name[14:-1].split(",")
		count_name = splitParams[1]
		type_name=expandName(splitParams[0],sub_types)
		return ["parseFixArray", [ "parse%s" % type_name,count_name ] ]
	elif type_name[0:12]=="Zoic_String_" and type_name[-1:]=="_":
		#StringÐÎÊ½£¬²»¿ÉÄÜ´¦ÔÚin_union·½Ê½ÏÂ
		return ["parseString", [] ]
	elif type_name[0:12]=="Zoic_Binary_" and type_name[-1:]=="_":
		#BinaryÐÎÊ½£¬²»¿ÉÄÜ´¦ÔÚin_union·½Ê½ÏÂ
		return ["parseBinary", [] ]
	return ["parse%s" % type_name, [] ]


#´¦Àí½á¹¹»òÁªºÏÄÚ³ÉÔ±ÁÐ±í
def handleMembers(node,t_name,top=1,types=None,names=None,descs=None,conds=None,s_types=None):
	#(½áµã,ÀàÐÍÃû³Æ,ÊÇ·ñ¶¥²ãÐ­Òé,³ÉÔ±ÀàÐÍÁÐ±í,³ÉÔ±Ãû³ÆÁÐ±í,³ÉÔ±ÃèÊöÁÐ±í,³ÉÔ±Ìõ¼þÁÐ±í,×ÓÀàÐÍ¶¨Òå)
	members=getElements(node,"Member")
	if types is None:
		types=[]	#ÀàÐÍÁÐ±í
	if names is None:
		names=[]	#Ãû³ÆÁÐ±í
	if conds is None:
		conds=[]	#Ìõ¼þÁÐ±í
	if descs is None:
		descs=[]	#ÃèÊöÁÐ±í
	sub_types={}	#ÔÚ±¾²ãÃüÃû¿Õ¼äÖÐµÄ×ÓÀàÐÍ¶¨Òå
	if s_types is not None:
		#½«ÉÏ²ã¿Õ¼ä¶¨ÒåÒýÈëµ½±¾²ã
		s_items=s_types.items()
		for k,v in s_items:
			sub_types[k]=v
	enums=[]	#Ã¶¾ÙÖµÁÐ±í
	for member in members:
		handleMember(member,types,names,descs,conds,enums,t_name,sub_types)

	#½«ÏàÁÚµÄÏàÍ¬Ìõ¼þºÏ²¢ÔÚÒ»Æð
	name_conds=[]	#[Ìõ¼þ,[ÀàÐÍ,Ãû³Æ,ÃèÊö]]
	names_len = len(names)
	if names_len >0:
		name_cond= []
		name_cond.append(conds[0])
		name_cond.append([[types[0],names[0],descs[0]]])
		cur_cond=conds[0]
		i=1
		while i < names_len:
			if conds[i]==cur_cond:
				name_cond[1].append([types[i],names[i],descs[i]])
			else:
				name_conds.append(name_cond)
				name_cond = []
				name_cond.append(conds[i])
				name_cond.append([[types[i],names[i],descs[i]]])
				cur_cond=conds[i]
			i += 1
		name_conds.append(name_cond)

	#½«±äÁ¿Ãû³ÆÓëÖµÉèÎªºÏÊÊµÄÖµ
	for name_cond in name_conds:
		for enum in enums:
			name_cond[0]=name_cond[0].replace(enum,t_name+"_"+enum)
		name_cond[0]=adjustCondition(names,name_cond[0])
		name_cond[0]=name_cond[0].replace("||"," or ")
		name_cond[0]=name_cond[0].replace("&&"," and ")
		name_cond[0]=name_cond[0].replace("!=","~=")

	#×Ô¶¯Éú³É½âÎö´úÂë
	if node.localName!="Union":
		global parse_function
		#code µ±Ç°ÀàÐÍ´úÂë
		if top:
			code = tabStr(0,"function parse%s(parent)\n",t_name)
		else:
			code = tabStr(0,"function parse%s(parent,name,desc)\n",t_name)
			code += tabStr(1,"parent=makeStructNode(parent,name,desc);\n")
		code += tabStr(1,"local res={};\n")
		in_union=0
		for name_cond in name_conds:
			if name_cond[0]!="":
				have_if=1
				nametab=2
			else:
				have_if=0
				nametab=1
			if have_if:
				code += tabStr(1,"if (%s) then\n",name_cond[0])
			for item in name_cond[1]:
				if item[0]=="?U+":		#ÁªºÏ¿ªÊ¼
					union_name=item[1]
					in_union=1
					code += tabStr(nametab,"res.%s={};\n",union_name);
					code += tabStr(nametab,"local uNode=makeStructNode(parent,\"%s\",\"%s\");\n",union_name,item[2]);
				elif item[0]=="?U-":	#ÁªºÏ½áÊø
					in_union=0
				else:
					type_name=item[0]	#ÀàÐÍÃû
					#½«ÀàÐÍÀ©Õ¹ÎªÕæÕýµÄÃû³Æ
					type_name=expandName(type_name,sub_types)
					typeNameOutput = pyParseTypeName(type_name,sub_types)
					py_paramcontent = ""
					for paramname in typeNameOutput[1]:
						py_paramcontent += ","
						py_paramcontent += paramname
					if in_union:
						code += tabStr(nametab,"res.%s.%s=%s(uNode,\"%s\",\"%s\"%s);\n",union_name,item[1],typeNameOutput[0],item[1],item[2],py_paramcontent);
					else:
						code += tabStr(nametab,"res.%s=%s(parent,\"%s\",\"%s\"%s);\n",item[1],typeNameOutput[0],item[1],item[2],py_paramcontent);
			if have_if:
				code += tabStr(1,"end\n")
		code += tabStr(1,"return res;\n")
		code += tabStr(0,"end\n\n")
		parse_function += code

#µ÷Õûµ¥´Ê
def adjustWord(names,word):
	result = word
	for name in names:
		if word==name:
			result="res."+word
			break
	return result

#µ÷ÕûÌõ¼þ
def adjustCondition(names,condition):
	word=""
	index=0
	result=""
	length=len(condition)
	while index < length:
		ch = condition[index:index+1]
		if ch.isalnum() or ch =="_":
			word+=ch
		else:
			#µ¥´Ê½áÊø
			if word!="":
				result+=adjustWord(names,word)
			result+=ch
			word=""
		index +=1
	if word!="":
		result+=adjustWord(names,word)
	return result

def transferTypeName(code_typename):
	return code_typename.replace("::","_").replace("<","_").replace(">","_").replace(" ","_").replace("*","_")

def handleMember(node,types,names,descs,conds,enums,t_name,s_types):
	#ÀàÐÍ¶¨Òå
	typenode=node.firstChild
	while not typenode.hasChildNodes():
		typenode=typenode.nextSibling
	tagName=typenode.localName
	type_name = ""
	if tagName=="Type":
		type_name=getText(typenode)
	elif tagName=="Struct":
		type_name=handleStruct(typenode,t_name,s_types)
	elif tagName=="Union":
		u_types=[]	#ÁªºÏÄÚµÄ³ÉÔ±ÀàÐÍ¶¨Òå
		u_names=[]	#ÁªºÏÄÚµÄ³ÉÔ±Ãû³Æ¶¨Òå
		u_conds=[]	#ÁªºÏÄÚµÄÌõ¼þ¶¨Òå
		u_descs=[]	#ÁªºÏÄÚµÄ³ÉÔ±ÃèÊö
		handleUnion(typenode,t_name,u_types,u_names,u_descs,u_conds,s_types)
	elif tagName=="Enum":
		global const_vars
		_enums=[]
		res=handleEnum(typenode,_enums)
		i=0
		data=[]
		for enum in _enums:
			data.append([t_name+"_"+enum,"=%d;" % i])
			i +=1
			enums.append(enum)
		const_vars+=tabData(0,data)
	try:
		name = getText(getElement(node,"Name"))
	except IndexError:
		name = ""
	try:
		_descs = getText(getElement(node,"Description")).split("\n")
	except IndexError:
		_descs = [""]
	try:
		condition = getText(getElement(node,"Condition"))
	except IndexError:
		condition = ""

	#Ãû³Æ
	if name!="":
		if tagName=="Union":
			u_names_len=len(u_names)
			i = 0
			names.append(name)
			conds.append("")
			types.append("?U+")
			descs.append(_descs[0])
			while i < u_names_len:
				names.append(u_names[i])
				conds.append(u_conds[i])
				types.append(u_types[i])
				descs.append(u_descs[i])
				i += 1
			names.append(name)
			conds.append("")
			types.append("?U-")
			descs.append("")
		else:
			names.append(name)
			conds.append(condition)
			types.append( transferTypeName(type_name) )
			descs.append(_descs[0])

def handleStruct(node,p_name,s_types=None):
	try:
		name = getText(getElement(node,"Name"))
	except IndexError:
		name = ""
	if p_name!="":
		if s_types is not None:
			s_types[name]=p_name+"_"+name
		name=p_name+"_"+name
	handleMembers(node,name,0,None,None,None,None,s_types)
	return name

def handleUnion(node,p_name,types=None,names=None,descs=None,conds=None,s_types=None):
	try:
		name = getText(getElement(node,"Name"))
	except IndexError:
		name = ""
	if p_name!="":
		name=p_name+"_"+name
	handleMembers(node,name,0,types,names,descs,conds,s_types)

def handleTypes(types):
	for node in types.childNodes:
		if node.hasChildNodes():
			tagName=node.localName
			if tagName=="TypeAlias":
				global alias_types
				aliases = []
				handleTypeAlias(node,"",aliases)
				for alias in aliases:
					typeNameOutput = pyParseTypeName(alias[0],{})
					if len(typeNameOutput[1]) == 0:
						alias_types += "parse%s\t\t\t\t= %s;\n" % ( alias[1],typeNameOutput[0] )
					else:
						py_paramcontent = ""
						for paramname in typeNameOutput[1]:
							py_paramcontent += "," + paramname
						alias_types += "\n"
						alias_types += "function parse%s(parent,name,desc)\n" % ( alias[1] )
						alias_types += "\treturn %s(parent,name,desc%s);\n" % ( typeNameOutput[0],py_paramcontent )
						alias_types += "end\n"
						alias_types += "\n"
			elif tagName=="Struct":
				handleStruct(node,"")
				pass
			elif tagName=="Union":
				handleUnion(node,"")
				pass
			elif tagName=="Enum":
				global const_vars
				enums=[]
				handleEnum(node,enums)
				i=0
				data=[]
				for enum in enums:
					data.append([enum,"=%d;" % i])
					i +=1
				const_vars+=tabData(0,data)
				pass

def handleTypeAlias(node,p_name,aliases = None):
	sourceName = getText(getElement(node,"SourceType"))
	aliaName = getText(getElement(node,"Alias"))
	try:
		desc = getText(getElement(node,"Description"))
	except IndexError:
		desc = ""
	if p_name != "":
		return #²»Ç°²»Ö§³Ö
	type_name = transferTypeName(sourceName)
	if aliases is not None:
		aliases.append( [type_name,aliaName] )

def handleEnum(node,enums=None):
	try:
		name = getText(getElement(node,"Name"))
	except IndexError:
		name = ""
	try:
		desc = getText(getElement(node,"Description"))
	except IndexError:
		desc = ""
	items = getElements(node,"Item");
	for item in items:
		name = getText(getElement(item,"Name"))
		if enums is not None:
			enums.append(name)

files=glob.glob("%s/Proto_*.xml" % PATH_PROTO_SOURCE)
files.sort()
for file in files:
	dom = xml.dom.minidom.parse(file)
	handleProto(getElement(dom,"Proto"))

content=""

#Éú³É´íÎóºÅ¶¨Òå
data=[]
errs_items=ERRS.items()
for k,v in errs_items:
	data.append(["%s" % k,"=%d;" % v])
const_vars += tabData(0,data)

content+=const_vars
if const_vars != "":
	content += "\n"
content+=alias_types
if alias_types != "":
	content += "\n"
content+=parse_function
content+=trigger_function
content+="""
function main()
	InitSystemTriggers();
"""
content+=trigger_register
content+="end"

o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
try:
	o_flag |= os.O_BINARY
except AttributeError:
	pass

filename = "%s/MessageParse/config/parse.lua" % PATH_TOOL_TARGET
os.write(os.open(filename,o_flag),content)

filename = "%s/Client/parse.lua" % PATH_PROTO_TARGET
os.write(os.open(filename,o_flag),content)
