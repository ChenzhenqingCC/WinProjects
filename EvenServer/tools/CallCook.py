#!/usr/bin/env python
#coding=utf-8
import os
import sys

mapList = {}     
def cook_all_maps(path):
    if os.path.isfile(path):
        root, extension = os.path.splitext(path)
        if extension == ".umap":
                l = len(mapList)
                mapList[l] = os.path.basename(path)
                
    elif os.path.isdir(path):
        for item in os.listdir(path):
            itemsrc = os.path.join(path, item)
            cook_all_maps(itemsrc)
        
param = -1
param = int(sys.argv[1])
cook_all_maps(r'..\..\..\FengYunOLGame\Content\Maps')
#cook_all_maps(r'..\..\..\FengYunOLGame\Content\Packages')
exepath = r'..\..\..\Binaries\Win32\FengYunOLGame.exe cookpackages'
calltext = exepath
for idx in mapList:
		print("cook:" + mapList[idx])
		calltext = calltext + " " + mapList[idx]
#calltext = calltext + " -platform=PC"
if param == 0:
	#calltext = calltext + " -platform=PC -full -reduxcache"
	calltext = calltext + " -platform=PC -silent -BUILDMACHINE"
if param == 1:
	calltext = calltext + " -platform=PC -silent -BUILDMACHINE"
os.popen(calltext)
