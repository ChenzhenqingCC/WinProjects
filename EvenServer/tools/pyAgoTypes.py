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
#sys.path.append(PATH_PROTO_SOURCE)
#try:
#	from cmd_def import *
#except ImportError:
#	sys.exit(0)
#try:
#	from err_def import *
#except ImportError:
#	sys.exit(0)

#Àà¶¨Òå
class_defs=""
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
	for h in hosts_info[2]:
		if h=="Client" or h=="Admin":
			p_name = "P" + SEPARATOR_PACKET + HOST_TYPE[fr] + SEPARATOR_HOST + HOST_TYPE[to] + SEPARATOR_PROTO + name + SEPARATOR_ACTION + ACTION_TYPE[type]
			#½âÎöº¯Êý
			handleMembers(getElement(message,"Content"),p_name)

def expandName(type_name,sub_types):
	if sub_types.has_key(type_name):
		return sub_types[type_name]
	else:
		return type_name

#Éú³ÉPythonÀàÃû³Æ
def pyClassName(t_name):
	if t_name=="char":
		return "CHAR"
	elif t_name=="int":
		return "INT"
	elif t_name=="short":
		return "SHORT"
	elif t_name=="long":
		return "LONG"
	elif t_name=="float":
		return "FLOAT"
	elif t_name=="Zoic_TinyBinary":
		return "TinyBinary"
	else:
		return t_name

def pyParseTypeName(type_name,sub_types):
	#std::vector<class>
	if type_name[0:11]=="std_vector_" and type_name[-1:]=="_":
		type_name=expandName(type_name[11:-1],sub_types)
		return ["VECTOR", [ pyClassName(type_name) ] ]
	elif type_name[0:14]=="Zoic_VarArray_" and type_name[-1:]=="_":
		type_name_list=type_name[14:-1].split(",")
		type_name=type_name_list[0]
		type_name=expandName(type_name,sub_types)
		size_type_name="WORD"
		if len(type_name_list)==3:
			size_type_name=expandName(type_name_list[2],sub_types)
		_subtype = pyParseTypeName(type_name,sub_types)
		if len( _subtype[1] ):
			raise
		return ["VAR_ARRAY", [ pyClassName(type_name) , pyClassName(size_type_name) ] ]
	elif type_name[0:14]=="Zoic_FixArray_" and type_name[-1:]=="_":
		splitSize = type_name.find(",")
		count_name = type_name[splitSize+1:-1]
		type_name=expandName(type_name[14:splitSize],sub_types)
		return ["FIX_ARRAY", [ pyClassName(type_name),count_name ] ]
	elif type_name[0:12]=="Zoic_Binary_" and type_name[-1:]=="_":
		return ["Binary", [] ]
	elif type_name[0:12]=="Zoic_String_" and type_name[-1:]=="_":
		return ["String", [] ]
	elif type_name[0:13]=="Zoic_UString_" and type_name[-1:]=="_":
		return ["String", [] ]
	return [pyClassName(type_name), [] ]


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
	
	#×Ô¶¯Éú³É½âÎö´úÂë
	
	global class_defs
	#Àà¶¨Òå
	code = tabStr(0,"class %s(STRUCT):\n",t_name)
	#__init__
	code += tabStr(1,"def __init__(self):\n")
	code += tabStr(2,"STRUCT.__init__(self)\n")
	i=0
	in_union=0
	while i < names_len:
		type_name=""
		if types[i][0:3]=="?U+":#ÁªºÏ¿ªÊ¼
			in_union=1
			type_name=types[i][3:]
		elif types[i]=="?U-":	#ÁªºÏ½áÊø
			in_union=0
		elif in_union==0:
			type_name=types[i]
		if type_name!="":
			typeNameOutput = pyParseTypeName(type_name,sub_types)
			py_paramcontent = ""
			for paramname in typeNameOutput[1]:
				if py_paramcontent != "":
					py_paramcontent += ","
				py_paramcontent += paramname
			code += tabStr(2,"self.setData(\"%s\",%s(%s))\n",names[i],typeNameOutput[0],py_paramcontent)
		i+= 1
	if node.localName!="Union":
		#pack
		code += tabStr(1,"def pack(self):\n")
		code += tabStr(2,"res = \"\"\n")
		in_union=0
		for name_cond in name_conds:
			if name_cond[0]!="":
				have_if=1
				nametab=3
			else:
				have_if=0
				nametab=2
			if have_if:
				code += tabStr(2,"if %s:\n",name_cond[0])
			for item in name_cond[1]:
				if item[0][0:3]=="?U+":		#ÁªºÏ¿ªÊ¼
					union_name=item[1]
					in_union=1
				elif item[0]=="?U-":	#ÁªºÏ½áÊø
					in_union=0
				else:
					type_name=item[0]	#ÀàÐÍÃû
					#½«ÀàÐÍÀ©Õ¹ÎªÕæÕýµÄÃû³Æ
					type_name=expandName(type_name,sub_types)
					if in_union:
						code += tabStr(nametab,"res += self.%s.%s.pack()\n",union_name,item[1]);
					else:
						code += tabStr(nametab,"res += self.%s.pack()\n",item[1]);
		code += tabStr(2,"return res\n")
		#unpack
		code += tabStr(1,"def unpack(self,data):\n")
		code += tabStr(2,"index=0\n")
		in_union=0
		for name_cond in name_conds:
			if name_cond[0]!="":
				have_if=1
				nametab=3
			else:
				have_if=0
				nametab=2
			if have_if:
				code += tabStr(2,"if %s:\n",name_cond[0])
			for item in name_cond[1]:
				if item[0][0:3]=="?U+":		#ÁªºÏ¿ªÊ¼
					union_name=item[1]
					in_union=1
				elif item[0]=="?U-":	#ÁªºÏ½áÊø
					in_union=0
				else:
					type_name=item[0]	#ÀàÐÍÃû
					#½«ÀàÐÍÀ©Õ¹ÎªÕæÕýµÄÃû³Æ
					type_name=expandName(type_name,sub_types)
					if in_union:
						code += tabStr(nametab,"index += self.%s.%s.unpack(data[index:])\n",union_name,item[1]);
					else:
						code += tabStr(nametab,"index += self.%s.unpack(data[index:])\n",item[1]);
		code += tabStr(2,"return index\n")
	class_defs += code

#µ÷Õûµ¥´Ê
def adjustWord(names,word):
	result = word
	for name in names:
		if word==name:
			result="self."+word
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
		type_name=handleUnion(typenode,t_name,u_types,u_names,u_descs,u_conds,s_types)
	elif tagName=="Enum":
		global const_vars
		_enums=[]
		res=handleEnum(typenode,_enums)
		data=[]
		last_enum_name = ""
		for enum in _enums:
			enum_value = enum["value"]
			if enum["value"] == "":
				if last_enum_name != "":
					enum_value = last_enum_name + " + 1"
				else:
					enum_value = "0"
			last_enum_name = enum["name"]
			
			try:
				data.append([t_name+"_"+last_enum_name,"= %d" % int(enum_value)])
			except TypeError:
				data.append([t_name+"_"+last_enum_name,"= %s" % (t_name+"_"+enum_value)])
				
			enums.append(last_enum_name)
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
			types.append("?U+"+type_name)
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
	return name

g_enumvaluelast = 0
g_enumvalue = 0
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
						alias_types += "class %s\t\t\t\t(%s):pass\n" % ( alias[1],typeNameOutput[0] )
					else:
						py_paramcontent = ""
						for paramname in typeNameOutput[1]:
							py_paramcontent += "," + paramname
						alias_types += "\n"
						alias_types += "class %s(%s):\n" % ( alias[1],typeNameOutput[0] )
						alias_types += "\tdef __init__(self):\n"
						alias_types += "\t\t%s.__init__(self%s)\n" % ( typeNameOutput[0],py_paramcontent )
						alias_types += "\n"
				pass
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
				data=[]
				last_enum_name = ""
				for enum in enums:
					enum_value = enum["value"]
					if enum["value"] == "":
						if last_enum_name != "":
							enum_value = last_enum_name + " + 1"
						else:
							enum_value = "0"
					last_enum_name = enum["name"]
					data.append([last_enum_name,"= %s" % enum_value])
						
				const_vars+=tabData(0,data)
				pass
			elif tagName=="EnumImport":
				enums=[]
				handleEnumImport(node,enums)
				data=[]				
				last_enum_name = ""
				for enum in enums:
					enum_value = enum["value"]
					if enum["value"] == "":
						if last_enum_name != "":
							enum_value = last_enum_name + " + 1"
						else:
							enum_value = "0"
					last_enum_name = enum["name"]
					data.append([last_enum_name,"= %s" % enum_value])	

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
#	try:
#		name = getText(getElement(node,"Name"))
#	except IndexError:
#		name = ""
#	try:
#		desc = getText(getElement(node,"Description"))
#	except IndexError:
#		desc = ""
#	items = getElements(node,"Item");
#	for item in items:
#		name = getText(getElement(item,"Name"))
#		if enums is not None:
#			enums.append(name)
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
		try:
			value = getText(getElement(item,"Value"))
		except IndexError:
			value = ""
		if enums is not None:
			enums.append({"name":name,"value":value})

def handleEnumImport(node,enums):
	fileName = getText(getElement(node,"File"))
	enumName = getText(getElement(node,"EnumName"))
	dom = xml.dom.minidom.parse("%s/%s" % (PATH_PROTO_SOURCE, fileName))
	protoNode = getElement(dom,"Proto")
	typesNode = getElement(protoNode, "Types")
	enumNodes = getElements(typesNode, "Enum")
	MatchedEnumNode = None
	for enumNode in enumNodes:
		name = getText(getElement(enumNode,"Name"))
		if name == enumName:
			MatchedEnumNode = enumNode
			break
	if MatchedEnumNode <> None:
		handleEnum(MatchedEnumNode, enums)
	else:
		print "Cannot find importing enum : %s in %s" % (enumName, fileName)
		raise

def handleConstant(node, constants=None):
	name = getText(getElement(node,"Name"))
	value = getText(getElement(node,"Value"))
	try:
		desc = getText(getElement(node,"Description"))
	except IndexError:
		desc = ""
	if constants is not None:
		constants.append({"name":name,"value":value})

def ImportConstantsFromProtoErrorAndProtoCommand():
	sys.path.append("..\\source_code\\proto_types")
	from cmd_def import CMDS
	from err_def import ERRS
	CmdConsts = "#proto commands\n"
	for CmdName in CMDS:
		CmdConsts += "%s = %d\n" % (CmdName, CMDS[CmdName])
	CmdConsts += "\n"
	ErrConsts = "#Error codes\n"
	for ErrName in ERRS:
		ErrConsts += "%s = %d\n" % (ErrName, ERRS[ErrName])
	ErrConsts += "\n"
	global const_vars
	const_vars = CmdConsts + ErrConsts + const_vars
	
files=glob.glob("%s/*.xml" % PATH_PROTO_SOURCE)
files.sort()
fileMap = {}
fileDepend = []
for file in files:
	dom = xml.dom.minidom.parse(file)
	sizelen = len(file) - 1
	while sizelen >= 0:
		if file[sizelen] == "/" or file[sizelen] == "\\":
			break
		sizelen = sizelen - 1
	fileName = file[sizelen+1:]
	fileMap[fileName] = True
	fileDepend.append( [getElement(dom,"Proto"),[],False,fileName,file ] )

for attributs in fileDepend:
	proto = attributs[0]
	node = getElement(proto,"Headers")
	includes=getElements(node,"Include")
	for includeFile in includes:
		filename = getText(includeFile)
		if filename[-2:] == ".h":
			xmlFileName = filename[:-2] + ".xml"
			if xmlFileName in fileMap:
				attributs[1].append(xmlFileName)

maxCount = int( len( fileDepend ) )
parsedCounts = int(0)
parsedNames = {}

while parsedCounts<maxCount:
	DEPEND_INDEX_NODE = 0
	DEPEND_INDEX_DEP = 1
	DEPEND_INDEX_PARSED = 2
	DEPEND_INDEX_NAME = 3
	DEPEND_INDEX_PATH = 4
	orgLen = parsedCounts
	for attributs in fileDepend:
		if attributs[2]:
			continue
		canParse = True
		for xmlFileName in attributs[DEPEND_INDEX_DEP]:
			if not( xmlFileName in parsedNames ):
				canParse = False
		if canParse:
			handleProto( attributs[DEPEND_INDEX_NODE] )
			attributs[DEPEND_INDEX_PARSED] = True
			parsedNames[ attributs[DEPEND_INDEX_NAME] ] = True
			parsedCounts = parsedCounts + 1
	if orgLen >= parsedCounts:
		print "Check XML Dependence Failed! Failed Files(%d/%d):\n" % (maxCount - parsedCounts,maxCount)
		for attributs in fileDepend:
			if attributs[DEPEND_INDEX_PARSED]:
				continue
			print "\t%s\n" % attributs[DEPEND_INDEX_PATH]
		exit(1)

ImportConstantsFromProtoErrorAndProtoCommand()

content="""# -*- coding: iso-8859-1 -*-
from _AgoTypes import *
#from AgoError import *
"""

content+=const_vars
if const_vars != "":
	content += "\n"
content+=alias_types
if alias_types != "":
	content += "\n"
content+=class_defs

o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
try:
	o_flag |= os.O_BINARY
except AttributeError:
	pass
filename = "%s/AgoTypes.py" % PATH_ROBOT_TARGET
os.write(os.open(filename,o_flag),content)
