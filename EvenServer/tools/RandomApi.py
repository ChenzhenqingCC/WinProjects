#!/usr/bin/env python
#coding=utf-8
import os
import sys
import random
import xml.dom.minidom
import re
import codecs
from utils import *
from config import *
from randomdata import *

ReRandom = False
xmlFilesPath = r'..\..\Src\RanaApiGen\ClientCreate\xml'
srcCppFilesPath = r'RandomApi'
tagCppFilesPath = r'..\..\Src\FengYunOLGame\RanaCli\decl'
#srcLuaFilesPath = r'..\..\..\FengYunOLGame\Content\RanaScripts'
srcLuaFilesPath = sys.argv[1]
tagLuaFilesPath = r'..\..\..\FengYunOLGame\Content\RanaScriptsRandom'

#srcSpacehFilePath = GameRootPath + r'\trunk\Development\Src\RanaApiGen\ClientCreate\RandomApi'
#tagSpacehFilePath = GameRootPath + r'\trunk\Development\Src\FYServer\rana'

#xmlFilesPath = r'F:\test'
#srcCppFilesPath = r'F:\test'
#tagCppFilesPath = r'F:\tagcpp'
#srcLuaFilesPath = r'F:\test'
#tagLuaFilesPath = r'F:\taglua'

def IsInRandomList(tag):
        for rd in randomListMap:
                if(randomListMap[rd] == str(tag)):
                        return rd
        return "0"
def getRandomNum():
        while True:
                randNum = int(random.random() * 100000)
                if( IsInRandomList(randNum) == "0"):
                        return str(randNum)
                
def handleDatafunc(node):
        groups=getElements(node,"Group")
        for group in groups:
                funcs = getElements(group,"Func")
                for func in funcs:
                        names = getElements(func,"Name")
                        for name in names:
                                nameVal = getText(name,DAT_CODE)
                                if not randomListMap.has_key(nameVal):
                                        randomListMap[nameVal] = getRandomNum()
                                #if randomNameExist(nameVal) == False:
                                        #l = len(funcNameList)
                                        #funcNameList[l] = nameVal
                                        #getRandomNum(1)

#def randomNameExist(name):
        #for idx in funcNameList:
                #if funcNameList[idx] == name:
                       # return True
        #return False
def handleDataevent(node):
        groups=getElements(node,"Group")
        for group in groups:
                funcs = getElements(group,"Event")
                for func in funcs:
                        names = getElements(func,"Name")
                        for name in names:
                                nameVal = getText(name,DAT_CODE)
                                nameVal = "TriggerRegister" + nameVal + "Event"
                                if not randomListMap.has_key(nameVal):
                                        randomListMap[nameVal] = getRandomNum()
                                #if randomNameExist("TriggerRegister" + nameVal + "Event") == False:
                                        #l = len(funcNameList)
                                        #funcNameList[l] = "TriggerRegister" + nameVal + "Event"
                                        #getRandomNum(1)

def create_random_file(path):
        dom = xml.dom.minidom.parse(path)
        filename = os.path.basename(path)
        if filename == "RanaType.xml":
                return
        if filename == "RanaEvent.xml":
                handleDataevent(getElement(dom,"Events"))
        else:
                handleDatafunc(getElement(dom,"RanaFuncs"))
        if not randomListMap.has_key("StartTimer"):
                randomListMap["StartTimer"] = getRandomNum()
        if not randomListMap.has_key("InitGlobals"):
                randomListMap["InitGlobals"] = getRandomNum()
        if not randomListMap.has_key("InitCustomTriggers"):
                randomListMap["InitCustomTriggers"] = getRandomNum()
        
        #if randomNameExist("StartTimer") == False:
        #        funcNameList[len(funcNameList)] = "StartTimer"
         #       getRandomNum(1)
        #if randomNameExist("InitGlobals") == False:
         #       funcNameList[len(funcNameList)] = "InitGlobals"
         #       getRandomNum(1)
        #if randomNameExist("InitCustomTriggers") == False:
         #       funcNameList[len(funcNameList)] = "InitCustomTriggers"
          #      getRandomNum(1)
          
def replace_fun_to_random(srcpath, tagpath,filetype, rootpath):
        root, filename = os.path.split(srcpath)
        wf = os.path.join(tagpath, filename)
        readf = codecs.open(srcpath,"r","utf-8")
        readlines = {}
        try:
                readlines = readf.readlines()
        except:
                readf.close()
                readf = codecs.open(srcpath,"r", "gbk")
                readlines = readf.readlines()
        writef = codecs.open(wf, "w", "utf-8")
        writelines = readlines
        print "begin encode:" + wf

        idx = 0;
        for wr in writelines:
                fpname = "[^a-z0-9A-Z_]" 
                p = re.compile(fpname)
                p1 = re.compile("_func")
                ps = p.split(writelines[idx])
                replaceStr = ""
                ps.sort(key=len)
                ps.reverse()
                for funName in ps:
                        replaceStr = funName 
                        if len(funName) > 0:
                                if  filetype == 0:
                                        if p1.search(funName):
                                                ps2 = p1.split(funName)
                                                funName = ps2[0]
                                                replaceStr = funName + "_func"
                                                if randomListMap.has_key(funName) and len(replaceStr) > 0:
                                                        p2 = re.compile(replaceStr)
                                                        writelines[idx] = p2.sub("func_" + str(randomListMap[funName]),writelines[idx])
                                                        break
                                else:
                                        if randomListMap.has_key(funName) and len(replaceStr) > 0:
                                                replaceStr = replaceStr + "\("
                                                p2 = re.compile(replaceStr)
                                                writelines[idx] = p2.sub("func_" + str(randomListMap[funName]) + "(",writelines[idx])
                if filetype == 1:
                                p3 = re.compile('--.*')
                                p4 = re.compile('"--')
                                pmatch = p4.search(writelines[idx])
                                #if not pmatch:
                                        #writelines[idx] = p3.sub("",writelines[idx] ) 
                                                
                idx = idx + 1
        idx = 0
        for wr in writelines:
                writef.write(writelines[idx])
                idx = idx + 1
        readf.close()
        writef.close()
        print "finished encode:" + wf

def replace_random_to_spaceh(srcpath, tagpath):
        if os.path.isfile(srcpath):
                root, extension = os.path.splitext(srcpath)
                if(extension == ".h"):
                        replace_fun_to_random(srcpath, tagpath, 2, srcLuaFilesPath)
        elif os.path.isdir(srcpath):
                for item in os.listdir(srcpath):
                        itemsrc = os.path.join(srcpath, item)
                        changpath = tagpath
                        if os.path.isdir(itemsrc):
                                changpath = os.path.join(tagpath, item)
                        replace_random_to_spaceh(itemsrc, changpath)

def replace_random_to_lua(srcpath, tagpath):
        if os.path.isfile(srcpath):
                root, extension = os.path.splitext(srcpath)
                if(extension == ".lua"):
                        replace_fun_to_random(srcpath, tagpath, 1, srcLuaFilesPath)
        elif os.path.isdir(srcpath):
                for item in os.listdir(srcpath):
						if item != ".svn":
							itemsrc = os.path.join(srcpath, item)
							changpath = tagpath
							if os.path.isdir(itemsrc):
									changpath = os.path.join(tagpath, item)
									if not os.path.exists(changpath):
										os.mkdir(changpath)
							replace_random_to_lua(itemsrc, changpath)
                    
def replace_random_to_cpp(srcpath, tagpath):
        if os.path.isfile(srcpath):
                root, extension = os.path.splitext(srcpath)
                if(extension == ".cpp"):
                        replace_fun_to_random(srcpath, tagpath, 0, srcCppFilesPath)
        elif os.path.isdir(srcpath):
                for item in os.listdir(srcpath):
                    itemsrc = os.path.join(srcpath, item)
                    changpath = tagpath
                    if os.path.isdir(item):
                            changpath = os.path.join(tagpath, item)
                    replace_random_to_cpp(itemsrc, changpath)
        
def random_convert_all_file(path):
    if os.path.isfile(path):
        root, extension = os.path.splitext(path)
        if(extension == ".xml"):
                create_random_file(path)
    elif os.path.isdir(path):
        for item in os.listdir(path):
            itemsrc = os.path.join(path, item)
            random_convert_all_file(itemsrc)

def check_all_random():
        for rd in randomListMap:
                randNum = int(randomListMap[rd])
                rdRes = IsInRandomList(randNum)
                if( rdRes != "0" and rdRes != rd):
                        print "Exist Random Api:" + rd
                        randomListMap[rd] = getRandomNum()
                
            
def save_all_randomName():
        writef = open(r'Internal\ConfigDataExporter\tools\randomdata.py', "w")
        #writef.write("funcNameList = {}\n")
       # writef.write("randomList = {}\n")
        writef.write("randomListMap = {}\n")
        for idx in randomListMap:
                rdstr = "randomListMap[\"" + str(idx) + "\"] = " + "\"" + str(randomListMap[idx]) + "\"\n"
                writef.write(rdstr)
        #for idx in funcNameList:
               # rdstr = "funcNameList[" + str(idx) + "] = " + "\"" + str(funcNameList[idx]) + "\"\n"
               # writef.write(rdstr)
        #for idx in randomList:
                #rdl = "randomList[" + str(idx) + "] = " + "\"" + str(randomList[idx]) + "\"\n"
                #writef.write(rdl)
        writef.close()
        

if __name__ == "__main__":
        #funcNameList[0] = "TriggerRegisterTimerEvent"
        #funcNameList[1] = "TriggerRegisterPeriodsTimerEvent"
        #funcNameList[2] = "RanaSetTimerRepeat"
        #funcNameList[3] = "RanaSetTimerOnOff"
        #funcNameList[4] = "RanaResetTimer"
        #funcNameList[5] = "RanaSetTriggerOnOff"
        #funcNameList[0] = "StartTimer"
        #funcNameList[7] = "TriggerRegisterTimerEvent"
        #funcNameList[8] = "ConvertFocusUnitTypeToInt"
        #funcNameList[1] = "InitGlobals"
        #funcNameList[2] = "InitCustomTriggers"
        #getRandomNum(3)
        #funcNameList[11] = "RanaRegisterTimeEvent"
        #funcNameList[12] = "RanaCreateTimer"
        #funcNameList[13] = "RanaSetTimerPeriodTime"
        
        print "---------------API Encode begin---------------"
        if ReRandom == True:
                #funcNameList = {}
                #randomList = {}
                randomListMap = {}
        random_convert_all_file(xmlFilesPath)
        check_all_random()
        save_all_randomName()
        #replace_random_to_spaceh(srcSpacehFilePath, tagSpacehFilePath)
        replace_random_to_cpp(srcCppFilesPath, tagCppFilesPath)
        replace_random_to_lua(srcLuaFilesPath, tagLuaFilesPath)
        print "---------------API Encode finished---------------"


