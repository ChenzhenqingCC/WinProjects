#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-

import sys
import xml.dom.minidom
from utils import *
from config import *

if len(sys.argv) < 3:
	print "Usage:"
	print "\t./xml2dat.py <xmlfile> <OutputPath>"
	print "\t./xml2dat.py <xmlfile> <OutputPath> <bForceUpdate?>"
	print "\t./xml2dat.py <xmlfile> <OutputPath> <bForceUpdate?> <classname>"
	sys.exit(0)

from AgoTypesEx import *

FILE_NAME=sys.argv[1]
BASE_NAME = GetFileBaseName(FILE_NAME)
OUT_PATH = sys.argv[2]
OUTPUT_NAME = "%s/%s.dat" % (OUT_PATH,BASE_NAME)

bNeedUpdate = False
xml_time = os.path.getmtime(FILE_NAME)
try:
	dat_time = os.path.getmtime(OUTPUT_NAME)
	if dat_time < xml_time:
		bNeedUpdate = True
except:
	bNeedUpdate = True
if len(sys.argv) >= 4 and sys.argv[3].upper == "TRUE":
	bNeedUpdate = True

if len(sys.argv) >= 5:
	CLASS = sys.argv[4]
else:
	CLASS = BASE_NAME

infos = {}
info_stack=[]

HAS_ID=True

def handleData(node):
	global HAS_ID
	exec "info=%s()" % CLASS
	try:
		tmp=int(info.id)
	except AttributeError:
		HAS_ID=False
	except TypeError:
		print "id is not a number,ignore id unique checking"
		HAS_ID=False
	infos=getElements(node,"info")
	for info in infos:
		handleInfo(info)
def handleInfo(node):
	global infos
	global HAS_ID
	exec "info=%s()" % CLASS
	handleNode(info,node)
	if HAS_ID:
		if infos.has_key(int(info.id)):
			print "Duplicated id:%s in file:%s" % (str(info.id),FILE_NAME)
			sys.exit(1)
		else:
			infos[int(info.id)]=info
	else:
		infos[len(infos)]=info

#µÃµ½Ò»¸öÓï¾äÖÐµÄÈ«²¿µ¥´Ê
def getWordList(text):
	words = {}
	word = ""
	text_len = len(text)
	index = 0
	while index < text_len:
		ch = text[index]
		if ch.isalnum() or ch=="_":
			word += ch
		else:
			if word!="":
				words[word]=1
				word = ""
		index += 1
	if word!="":
		words[word]=1
	return words.keys()

#µ÷ÕûÌõ¼þÓï¾ä
def adjustCondition(cond):
	words = getWordList(cond)
	cond = cond.replace("||"," or ")
	cond = cond.replace("&&"," and ")
	for word in words:
		info_len=len(info_stack)
		i = info_len - 1
		while i >= 0:
			info = info_stack[i]
			if hasattr(info,word):
				cond = cond.replace(word,("info_stack[%d]." % i) +word)
				break
			i -= 1
	return cond

#¼ì²âUnionÊÇ·ñÆ¥Åä
def check(cond):
	res = False
	exec "res=%s" % adjustCondition(cond)
	return res

def handleNode(info,node):
	info_stack.append(info)
	#¼òµ¥Êý¾ÝÀàÐÍ
	if isinstance(info,Simple):
		val=getText(node,DAT_CODE)
		try:
			info.setData(val)
		except ValueError:
			#³¢ÊÔÓÃENUMÖµ
			exec "info.setData(%s)" % val
	elif isinstance(info,TinyBinary):
		val=getText(node,DAT_CODE)
		info.setData(val)
	elif isinstance(info,Binary):
		try:
			textNode = getElement(node,"Text")
			val=getText(textNode,DAT_CODE)
			info.setData(val)
		except IndexError:
			items=getElements(node,"item")
			for item in items:
				it=info.element()
				handleNode(it,item)
				info.append(it)
			info.append(0)
	elif isinstance(info,VECTOR):
		items=getElements(node,"item")
		for item in items:
			it=info.element()
			handleNode(it,item)
			info.append(it)
	else:
		nodes = node.childNodes
		for nod in nodes:
			if nod.hasChildNodes():
				cond=nod.getAttribute("condition")
				if cond!="" and not check(cond):
					print "check condition failed of id:%d Error:%s" % (info_stack[0].id,cond)
					sys.exit(0)
				exec "handleNode(info.%s,nod)" % nod.localName
	del info_stack[-1:]


if bNeedUpdate :
	print "¿ªÊ¼µ¼³ö%s" % FILE_NAME
	dom = xml.dom.minidom.parse(FILE_NAME)
	handleData(getElement(dom,"data"))

	count=DWORD(len(infos))

	content = Message()
	content << count

	infos_val=infos.values()
	for v in infos_val:
		content << v

	o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
	try:
		o_flag |= os.O_BINARY
	except AttributeError:
		pass
	os.write(os.open(OUTPUT_NAME,o_flag),str(content))
	print "Éú³ÉÁË%s" % OUTPUT_NAME