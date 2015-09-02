#!/usr/bin/env python
#coding=utf-8
import os
import sys
import re

#SrcPath = r'D:\alienbrainWork\FYRelease\ACB_FINALRELEASE'
#TagPath = r'D:\FYReleases\test\client\game'


def CheckFileName(fname):
        p = re.compile("[^a-z0-9A-Z_.]")
        match = p.search(fname)
        if match:
                print "match"
def CopyFile(filepath, tagpath):
        root, filename = os.path.split(filepath)
        tagfilepath = os.path.join(tagpath, filename)
        if not os.path.exists(tagfilepath):
                commandtxt = "copy " + filepath + " " + tagpath
                os.system(commandtxt)
                commandtxt = "svn add --username writer --password w@hy " + tagfilepath
                os.system(commandtxt)
        else:
                commandtxt = "copy " + filepath + " " + tagpath + " /y"
                os.system(commandtxt) 
def VersionCompareAdd(srcpath, tagpath):
    if os.path.isfile(srcpath):
                CopyFile(srcpath, tagpath)
    elif os.path.isdir(srcpath):
        for item in os.listdir(srcpath):
                itemsrc = os.path.join(srcpath, item)
                changpath = tagpath
                if os.path.isdir(itemsrc):
                                changpath = os.path.join(tagpath, item)
                                if not item == ".svn":
                                        if not os.path.exists(changpath):
                                                os.mkdir(changpath)
                                                commandtxt = "svn add --username writer --password w@hy " + changpath
                                                os.system(commandtxt)
                VersionCompareAdd(itemsrc, changpath)

def CheckFile(filepath, tagpath):
        root, filename = os.path.split(filepath)
        tagfilepath = os.path.join(tagpath, filename)
        if not filename == "ver.txt":
                if not os.path.exists(tagfilepath):
                        commandtxt = "svn delete --username writer --password w@hy " + filepath
                        os.system(commandtxt)
                
def VersionCompareDel(srcpath, tagpath):
        if os.path.isfile(srcpath):
                CheckFile(srcpath, tagpath)
        elif os.path.isdir(srcpath):
                for item in os.listdir(srcpath):
                        itemsrc = os.path.join(srcpath, item)
                        changpath = tagpath
                        if os.path.isdir(itemsrc):
                                changpath = os.path.join(tagpath, item)
                                if not item == ".svn":
                                        if not os.path.exists(changpath):
                                                commandtxt = "svn delete --username writer --password w@hy " + itemsrc
                                                os.system(commandtxt)
                                else:
                                        continue
                        VersionCompareDel(itemsrc, changpath)

if __name__ == "__main__":
        SrcPath = sys.argv[1]
        TagPath = sys.argv[2]
        print "---------------Version Compare begin---------------"
        VersionCompareAdd(SrcPath, TagPath)
        VersionCompareDel(TagPath, SrcPath)
        #commandtxt = "svn commit --username writer --password w@hy -m version " + TagPath
        #os.system(commandtxt)
        print "---------------Version Compare finish---------------"

