#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-

import sys
import xml.dom.minidom
from utils import *
from AgoXsdTypes import *
from config import *

XML_ENCODES= {
	"<"	:"&lt;",
	">"	:"&gt;",
	"&"	:"&amp;",
	"'"	:"&apos;",
	"\"":"&quot;",
}
def xmlEncode(text):
	res = ""
	length=len(text)
	index = 0
	while index < length:
		substr = text[index:index+1]
		if XML_ENCODES.has_key(substr):
			res += XML_ENCODES[substr]
		else:
			res += substr
		index += 1
	return res

if len(sys.argv)!=3:
	print "Usage:\n\t./xml2dat.py <xmlfile> <infoname>\n"
	sys.exit(0)

sys.path.append("../robot.py")
try:
	from AgoTypes import *
except ImportError:
	print "Can't import AgoTypes from robot.py"
	sys.exit(0)

FILE_NAME="%s/%s" % ( PATH_PROTO_SOURCE,sys.argv[1] )
INFO_NAME = sys.argv[2]
OUTPUT_NAME = "%s/%s.xsd" % (PATH_DATA_SOURCE,INFO_NAME)
pos=FILE_NAME.rfind("\\")
if pos==-1:
	pos=FILE_NAME.rfind("/")
pos += 1
PROTO_PATH = FILE_NAME[0:pos]
PROTO_NAME = FILE_NAME[pos:]

content="""<?xml version="1.0" encoding="GBK"?>
<xs:schema elementFormDefault="qualified" attributeFormDefault="unqualified" xmlns:xs="http://www.w3.org/2001/XMLSchema">
	<xs:include schemaLocation="AgoTypes.xsd"/>
	<xs:element name="data">
		<xs:annotation>
			<xs:documentation>Êý¾Ý</xs:documentation>
		</xs:annotation>
		<xs:complexType>
			<xs:sequence minOccurs="0" maxOccurs="unbounded">
				<xs:element name="info" type="%s"/>
			</xs:sequence>
		</xs:complexType>
	</xs:element>
""" % INFO_NAME

#ÀàÐÍ½áµã
type_nodes = {}
#ÔØÈëÀàÐÍ½áµã
def loadTypeNodes(filename):
	global type_nodes
	if type_nodes.has_key(filename):
		return
	try:
		dom = xml.dom.minidom.parse(PROTO_PATH+filename)
		proto = getElement(dom,"Proto")
		try:
			type_nodes[filename]=getElement(proto,"Types")
		except IndexError:
			type_nodes[filename]=None
		#¿ªÊ¼°üº¬µÄÆäËüÎÄ¼þ
		includes=getElements(getElement(proto,"Headers"),"Include")
		for include in includes:
			include_file = getText(include,XSD_CODE)
			if include_file[0:6]=="Proto_":
				loadTypeNodes(include_file.replace(".h",".xml"))
	except IOError:
		pass
loadTypeNodes(PROTO_NAME)

def handleProto(proto):
	global content
	nodInfo=findNode(INFO_NAME)
	if nodInfo is not None and nodInfo.localName=="Struct":
		res = handleStruct(nodInfo,1)
		content += res
	else:
		print "Can't find %s" % INFO_NAME
		sys.exit(0)
	content += "</xs:schema>\n"
	pass

def findNode(name):
	for type_node in type_nodes.values():
		if type_node is not None:
			nodes = type_node.childNodes
			for nod in nodes:
				if nod.hasChildNodes():
					if nod.localName=="Struct" or nod.localName=="Enum":
						if getText(getElement(nod,"Name"),XSD_CODE)==name:
							return nod
					elif nod.localName=="TypeAlias":
						if getText(getElement(nod,"Alias"),XSD_CODE)==name:
							return nod
	return None

def handleStruct(node,tabs,cond=""):
	try:
		name = getText(getElement(node,"Name"),XSD_CODE)
	except IndexError:
		name = ""
	try:
		desc = getText(getElement(node,"Description"),XSD_CODE)
	except IndexError:
		desc = ""
	res = ""
	if tabs==1:
		res += tabStr(tabs,"<xs:complexType name=\"%s\" mixed=\"true\">\n",name)
	else:
		res += tabStr(tabs,"<xs:complexType mixed=\"true\">\n")
	tabs += 1
	if desc!="":
		res += tabStr(tabs,"<xs:annotation>\n")
		res += tabStr(tabs+1,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
		res += tabStr(tabs,"</xs:annotation>\n")
	res += tabStr(tabs,"<xs:sequence>\n")
	tabs += 1
	members=getElements(node,"Member")
	for member in members:
		res += handleMember(member,tabs)
	tabs -= 1
	res += tabStr(tabs,"</xs:sequence>\n")
	tabs -= 1
	if cond!="":
		res += tabStr(tabs+1,"<xs:attribute name=\"condition\" type=\"xs:string\" use=\"required\" fixed=\"%s\"/>",cond)
	res += tabStr(tabs,"</xs:complexType>\n")
	return res

def handleUnion(node,tabs):
	try:
		name = getText(getElement(node,"Name"),XSD_CODE)
	except IndexError:
		name = ""
	try:
		desc = getText(getElement(node,"Description"),XSD_CODE)
	except IndexError:
		desc = ""
	res = ""
	if tabs==1:
		res += tabStr(tabs,"<xs:complexType name=\"%s\">\n",name)
	else:
		res += tabStr(tabs,"<xs:complexType>\n")
	tabs += 1
	if desc!="":
		res += tabStr(tabs,"<xs:annotation>\n")
		res += tabStr(tabs+1,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
		res += tabStr(tabs,"</xs:annotation>\n")
	res += tabStr(tabs,"<xs:choice minOccurs=\"0\">\n")
	tabs += 1
	members=getElements(node,"Member")
	for member in members:
		res += handleMember(member,tabs,True)
	tabs -= 1
	res += tabStr(tabs,"</xs:choice>\n")
	tabs -= 1
	res += tabStr(tabs,"</xs:complexType>\n")
	return res

def handleEnum(node,tabs):
	try:
		name = getText(getElement(node,"Name"),XSD_CODE)
	except IndexError:
		name = ""
	try:
		desc = getText(getElement(node,"Description"),XSD_CODE)
	except IndexError:
		desc = ""
	res = tabStr(tabs,"<xs:simpleType name=\"%s\">\n",name)
	tabs += 1
	if desc!="":
		res += tabStr(tabs,"<xs:annotation>\n")
		res += tabStr(tabs+1,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
		res += tabStr(tabs,"</xs:annotation>\n")
	res += tabStr(tabs,"<xs:restriction base=\"xs:string\">\n")
	tabs += 1
	items = getElements(node,"Item");
	for item in items:
		name = getText(getElement(item,"Name"),XSD_CODE)
		res += tabStr(tabs,"<xs:enumeration value=\"%s\">\n",name)
		desc = getText(getElement(item,"Description"),XSD_CODE)
		if desc!="":
			res += tabStr(tabs,"<xs:annotation>\n")
			res += tabStr(tabs+1,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
			res += tabStr(tabs,"</xs:annotation>\n")
		res += tabStr(tabs,"</xs:enumeration>\n")
	tabs -= 1
	res += tabStr(tabs,"</xs:restriction>\n")
	tabs -= 1
	res += tabStr(tabs,"</xs:simpleType>\n")
	return res

def getArrayType(name):
	res = []
	strip_name = name.strip()
	lt_index = strip_name.find("<")
	if lt_index == -1:
		return res
	if strip_name[-1:]!=">":
		return res
	temp_name = strip_name[0:lt_index]
	if temp_name == "Zoic::VarArray" :
		args = strip_name[lt_index+1:-1]
		comma_index = args.rfind(",")
		if comma_index == -1:
			return res
		res.append(args[0:comma_index])
		res.append(0)
		res.append(args[comma_index+1:].strip())
	elif temp_name == "Zoic::FixArray" :
		args = strip_name[lt_index+1:-1]
		comma_index = args.rfind(",")
		if comma_index == -1:
			return res
		res.append(args[0:comma_index])
		res.append(args[comma_index+1:].strip())
		res.append(args[comma_index+1:].strip())
	elif temp_name == "std::vector" :
		arg = strip_name[lt_index+1:-1].strip()
		res.append(arg)
		res.append(0)
		res.append(255)
	return res

#Éú³ÉArrayÀàÐÍ
def genArrayType(tabs,name,desc,element_type,minlen,maxlen):
	if name!="":
		res = tabStr(tabs,"<xs:complexType name=\"%s\">\n",name)
	else:
		res = tabStr(tabs,"<xs:complexType>\n")
	tabs += 1
	if desc!="":
		res += tabStr(tabs,"<xs:annotation>\n")
		tabs += 1
		res += tabStr(tabs,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
		tabs -= 1
		res += tabStr(tabs,"</xs:annotation>\n")

	exec "minOccurs=int(%s)" % minlen
	exec "maxOccurs=int(%s)" % maxlen
	res += tabStr(tabs,"<xs:sequence minOccurs=\"%d\" maxOccurs=\"%d\">\n",minOccurs,maxOccurs)
	res += genXsdElement(tabs+1,"item","",element_type,"","")
	res += tabStr(tabs,"</xs:sequence>\n")
	tabs -= 1
	res += tabStr(tabs,"</xs:complexType>\n")
	return res

#Éú³ÉStringÀàÐÍ
def genStringType(tabs,name,desc,maxlen):
	if name!="":
		res = tabStr(tabs,"<xs:simpleType name=\"%s\">\n",name)
	else:
		res = tabStr(tabs,"<xs:simpleType>\n")
	tabs += 1
	if desc!="":
		res += tabStr(tabs,"<xs:annotation>\n")
		tabs += 1
		res += tabStr(tabs,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
		tabs -= 1
		res += tabStr(tabs,"</xs:annotation>\n")

	exec "maxLength=int(%s)" % maxlen
	res += tabStr(tabs,"<xs:restriction base=\"xs:string\">\n")
	tabs += 1
	res += tabStr(tabs,"<xs:maxLength value=\"%s\"/>\n",maxLength)
	tabs -= 1
	res += tabStr(tabs,"</xs:restriction>\n")
	tabs -= 1
	res += tabStr(tabs,"</xs:simpleType>\n")
	return res

#Éú³ÉBinaryÀàÐÍ
def genBinaryType(tabs,name,desc,maxlen):
	if name!="":
		res = tabStr(tabs,"<xs:complexType name=\"%s\">\n",name)
	else:
		res = tabStr(tabs,"<xs:complexType>\n")
	tabs += 1
	if desc!="":
		res += tabStr(tabs,"<xs:annotation>\n")
		tabs += 1
		res += tabStr(tabs,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
		tabs -= 1
		res += tabStr(tabs,"</xs:annotation>\n")

	exec "maxLength=int(%s)" % maxlen
	res += tabStr(tabs,"<xs:choice>\n")
	tabs += 1
	res += tabStr(tabs,"<xs:sequence minOccurs=\"0\" maxOccurs=\"%d\">\n",maxLength)
	tabs += 1
	res += genXsdElement(tabs,"item","","char","","")
	tabs -= 1
	res += tabStr(tabs,"</xs:sequence>\n")
	res += genXsdElement(tabs,"Text","","Zoic::String<%d>" % (maxLength-1),"","")
	tabs -= 1
	res += tabStr(tabs,"</xs:choice>\n")
	tabs -= 1
	res += tabStr(tabs,"</xs:complexType>\n")
	return res

#Éú³ÉXSDÔªËØ
def genXsdElement(tabs,name,desc,type,enum,condition):
	res = ""
	type_name = getTypeName(type,enum).strip()
	#ÐÂÀàÐÍ¶¨Òå
	type_define = ""
	if type_name[0:13]=="Zoic::String<" and type_name[-1:]==">":
		maxlen=type_name[13:-1]
		res += tabStr(tabs,"<xs:element name=\"%s\"",name)
		type_define += genStringType(tabs+1,"","",maxlen)
	elif type_name[0:13]=="Zoic::Binary<" and type_name[-1:]==">":
		maxlen=type_name[13:-1]
		res += tabStr(tabs,"<xs:element name=\"%s\"",name)
		type_define += genBinaryType(tabs+1,"","",maxlen)
	else:
		array_type= getArrayType(type_name)
		if len(array_type)==3:
			res += tabStr(tabs,"<xs:element name=\"%s\"",name)
			type_define = genArrayType(tabs+1,"","",array_type[0],array_type[1],array_type[2])
		else:
			res += tabStr(tabs,"<xs:element name=\"%s\" type=\"%s\"",name,type_name)
	if condition!="":
		res += " minOccurs=\"0\""
	res += ">\n"
	if desc!="":
		tabs += 1
		res += tabStr(tabs,"<xs:annotation>\n")
		tabs += 1
		if condition!="":
			desc += "\n(" + condition +")"
		res += tabStr(tabs,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
		tabs -= 1
		res += tabStr(tabs,"</xs:annotation>\n")
		tabs -= 1
	if type_define!="":
		res += type_define
	res += tabStr(tabs,"</xs:element>\n")
	return res

def getTypeName(name,enum):
	global content
	if enum!="":
		if XSD_TYPES.has_key(enum):
			return XSD_TYPES[enum]
		node=findNode(enum)
		if node is not None and node.localName=="Enum":
			res = handleEnum(node,1)
			content += res
			XSD_TYPES[enum]=enum
			return enum
	if XSD_TYPES.has_key(name):
		return XSD_TYPES[name]
	elif EXT_TYPES.has_key(name):
		ext_type=EXT_TYPES[name][0]
		ext_desc=EXT_TYPES[name][1]
		if ext_type[0:13]=="Zoic::String<" and ext_type[-1:]==">":
			maxlen=ext_type[13:-1]
			res = genStringType(1,name,ext_desc,maxlen)
			content += res
			XSD_TYPES[name]=name
		if ext_type[0:13]=="Zoic::Binary<" and ext_type[-1:]==">":
			maxlen=ext_type[13:-1]
			res = genBinaryType(1,name,ext_desc,maxlen)
			content += res
			XSD_TYPES[name]=name
		else:
			array_param=getArrayType(ext_type)
			if len(array_param)==2:
				array_type=array_param[0]
				array_maxlen=array_param[1]
				res = genArrayType(1,name,ext_desc,array_type,array_maxlen)
				content += res
				XSD_TYPES[name]=name
	else:
		node = findNode(name)
		if node is not None:
			if node.localName=="Struct":
				res = handleStruct(node,1)
				content += res
				XSD_TYPES[name]=name
			elif node.localName=="TypeAlias":
				sourceName = getText( getElement(node,"SourceType"),XSD_CODE )
				return getTypeName(sourceName,"")
	return name

def handleMember(node,tabs,in_union=False):
	typenode=node.firstChild
	while not typenode.hasChildNodes():
		typenode=typenode.nextSibling
	tagName=typenode.localName
	try:
		name = getText(getElement(node,"Name"),XSD_CODE)
	except IndexError:
		name = ""
	try:
		descs = getText(getElement(node,"Description"),XSD_CODE).split("\n")
	except IndexError:
		descs = [""]
	if len(descs)==1:
		descs.append("")
	try:
		condition = getText(getElement(node,"Condition"),XSD_CODE)
	except IndexError:
		condition = ""
	type_name = ""

	if tagName=="Type":
		type_name=getText(typenode,XSD_CODE)
		return genXsdElement(tabs,name,descs[0],type_name,descs[1],condition)
	elif tagName=="Struct":
		element = tabStr(tabs,"<xs:element name=\"%s\">\n",name)
		if in_union:
			complex = handleStruct(typenode,tabs+1,condition)
		else:
			complex = handleStruct(typenode,tabs+1)
	elif tagName=="Union":
		element = tabStr(tabs,"<xs:element name=\"%s\">\n",name)
		complex = handleUnion(typenode,tabs+1)
	else:
		return ""
	if descs[0]!="":
		desc = descs[0]
		if condition!="":
			desc += "\n(" + condition + ")"
		annotation = tabStr(tabs+1,"<xs:annotation>\n")
		annotation += tabStr(tabs+2,"<xs:documentation>%s</xs:documentation>\n",xmlEncode(desc))
		annotation += tabStr(tabs+1,"</xs:annotation>\n")
	else:
		annotation = ""
	res = element + annotation + complex
	res += tabStr(tabs,"</xs:element>\n")
	return res

dom = xml.dom.minidom.parse(FILE_NAME)
handleProto(getElement(dom,"Proto"))

o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
try:
	o_flag |= os.O_BINARY
except AttributeError:
	pass
#print content
os.write(os.open(OUTPUT_NAME,o_flag),content)
