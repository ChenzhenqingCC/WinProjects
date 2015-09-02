#!/usr/bin/env python
#coding=utf-8
import os
import sys

mapList = {}     
def cook_all_maps(path):
    if os.path.isfile(path):
        root, extension = os.path.splitext(path)
        if extension == ".upk":
                l = len(mapList)
                root, extension = os.path.splitext(path)
                mapList[l] = os.path.basename(root)
                
    elif os.path.isdir(path):
        for item in os.listdir(path):
            itemsrc = os.path.join(path, item)
            cook_all_maps(itemsrc)
        

if __name__ == "__main__":
        mapList = {}
        writef = open(r'StartupPackage.txt', "w")
        #cook_all_maps(r'..\..\..\FengYunOLGame\Content\Maps')
        cook_all_maps(r'..\..\..\FengYunOLGame\Content\Packages')
        calltext = ""
        for idx in mapList:
                #print "cook:" + mapList[idx]
                calltext = calltext + "+Package=" + mapList[idx] + "\n"
        #calltext = calltext + " -platform=PC -silent -BUILDMACHINE"
        writef.write(calltext)
        writef.close()
        #os.popen(calltext)
