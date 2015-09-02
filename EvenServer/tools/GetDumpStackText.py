#!/usr/bin/env python
#coding=utf-8
import os
import sys
import codecs
import re
        
dumpPath = sys.argv[1]
dumplogPath = sys.argv[2]
tmplogPath = dumplogPath + "\\tmp"
 

def AnalyzeOne(path):
	bname =  os.path.basename(path)
	root, extension = os.path.splitext(bname)
	commandtxt = "cdb -z " + path + " -logo "+ tmplogPath + "\\" + root + ".txt" +" -lines -c \"!analyze -v;q\""
	os.system(commandtxt)

def AnalyzeDumpfiles(srcpath):
        if os.path.isfile(srcpath):
                root, extension = os.path.splitext(srcpath)
                if(extension == ".dmp"):
                        AnalyzeOne(srcpath)
        elif os.path.isdir(srcpath):
                for item in os.listdir(srcpath):
                        itemsrc = os.path.join(srcpath, item)
                        AnalyzeDumpfiles(itemsrc)

def GetStackText(filePath):
	root, filename = os.path.split(filePath)
	wf = os.path.join(dumplogPath, filename)
	readf = codecs.open(filePath,"r","utf-8")
	readlines = {}
	try:
		readlines = readf.readlines()
	except:
		readf.close()
		readf = codecs.open(filePath,"r", "gbk")
		readlines = readf.readlines()
	writef = codecs.open(wf, "w", "utf-8")
	writelines = {}
	idx = 0;
	p = re.compile('STACK_TEXT:')
	for rl in readlines:
		pmatch = p.search(readlines[idx])
		if pmatch:
			break
		idx = idx + 1
	
	idx1 = 0
	p1 = re.compile('STACK_COMMAND:')
	while True:
		if idx >= len(readlines):
			break
		pmatch1 = p1.search(readlines[idx])
		if pmatch1:
			break
		writelines[idx1] = readlines[idx]
		idx1 = idx1 + 1
		idx = idx + 1
	idx = 0
	for wr in writelines:
		writef.write(writelines[idx])
		idx = idx + 1
	readf.close()
	writef.close()
	
def GetStackTextFiles(srcpath):
	if os.path.isfile(srcpath):
                root, extension = os.path.splitext(srcpath)
                if(extension == ".txt"):
                        GetStackText(srcpath)
        elif os.path.isdir(srcpath):
                for item in os.listdir(srcpath):
                        itemsrc = os.path.join(srcpath, item)
                        GetStackText(itemsrc)
if __name__ == "__main__":
		commandtxt = "rmdir " + tmplogPath + " /S /Q"
		os.system(commandtxt)
		os.mkdir(tmplogPath)
		AnalyzeDumpfiles(dumpPath)
		GetStackTextFiles(tmplogPath)
		commandtxt = "rmdir " + tmplogPath + " /S /Q"
		os.system(commandtxt)



        
