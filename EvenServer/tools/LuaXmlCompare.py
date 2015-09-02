#!/usr/bin/env python
#coding=utf-8
import os
import sys
import re
import codecs
 
uuidMap={}

def GetUUIDFromLua(srcpath):
	readf = codecs.open(srcpath,"r","utf-8")
	readlines = {}
	try:
		readlines = readf.readlines()
	except:
		readf.close()
		print srcpath + " : GetUUIDFromLua close"
		return
	p = re.compile('getUUID()')
	idx = 0
	for rl in readlines:
		pmatch = p.search(readlines[idx])
		if pmatch:
			tagline = readlines[idx + 1]
			pos1 = tagline.index("\"")
			pos2 = tagline.rindex("\"")
			return tagline[pos1+1:pos2]
		idx = idx + 1
	return ""
	
def GetUUIDMapFromLua(srcpath):
	if os.path.isfile(srcpath):
		bname =  os.path.basename(srcpath)
		root, extension = os.path.splitext(bname)
		if(extension == ".lua"):
			#print "lua"
			uuid = GetUUIDFromLua(srcpath)
			#print srcpath
			if uuid == None:
				print "uuid=null"
				return
			if len(uuid) > 0:
				uuidMap[root.upper()] = uuid
				print root + ":" + str(uuidMap[root.upper()])
	elif os.path.isdir(srcpath):
		for item in os.listdir(srcpath):
			if item != ".svn":
				itemsrc = os.path.join(srcpath, item)
				GetUUIDMapFromLua(itemsrc)
	
def CheckXML(srcpath):	
	readf = codecs.open(srcpath,"r","utf-8")
	readlines = {}
	bname =  os.path.basename(srcpath)
	root, extension = os.path.splitext(bname)
	try:
		readlines = readf.readlines()
	except:
		readf.close()
		print srcpath + ": close"
		return
	p = re.compile('uuid=')
	idx = 0
	for rl in readlines:
		pmatch = p.search(readlines[idx])
		if pmatch:
			tagline = readlines[idx]
			pos1 = tagline.index("uuid=")
			pos2 = tagline.index("\"", pos1 + 7)
			xmluuid = tagline[pos1+6:pos2]
			luauuid = ""
			if uuidMap.has_key(root.upper()):
				luauuid = uuidMap[root.upper()]
			if str(xmluuid) != str(luauuid):
				print root + " uuid error!!!!!!"
				print "xmluuid:" + xmluuid
				print "luauuid:" + luauuid
				while True:
					a = 0
			#print tagline[pos1+6:pos2]
			#return tagline[pos1+6:pos2]
		idx = idx + 1
		
def CheckXMLs(srcpath):
	if os.path.isfile(srcpath):
		bname =  os.path.basename(srcpath)
		root, extension = os.path.splitext(bname)
		if(extension == ".xml"):
			CheckXML(srcpath)
	elif os.path.isdir(srcpath):
		for item in os.listdir(srcpath):
			if item != ".svn":
				itemsrc = os.path.join(srcpath, item)
				CheckXMLs(itemsrc)
		

if __name__ == "__main__":
	SrcPath = sys.argv[1]
	print SrcPath
	uuidMap={}
	print "---------------Check Lua And Xml Begin---------------"
	GetUUIDMapFromLua(SrcPath)
	CheckXMLs(SrcPath)
	print "---------------Check Lua And Xml Finish---------------"

