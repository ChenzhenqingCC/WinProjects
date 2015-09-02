# -*- coding: utf8 -*-
from workbook import *
import sys
import logging
import os
import sets
import shutil

#print sys.argv[1]
BOOK = readWorkBook(sys.argv[1])

#都写绝对路径
SourcePath = sys.argv[2]
DesPath = sys.argv[3]

SHEET_SCENE = BOOK["场景"]
SHEET_CHAR = BOOK["角色"]
SHEET_AVATAR = BOOK["Avatar"]

for ROW in SHEET_SCENE:
	ResourceName = ROW["名称"]
	DesName = ROW["名称"]
	for root, dirs, files in os.walk(SourcePath):
		for name in files:
			if name == ROW["名称"]:
				ResourceName = os.path.join(root, name)
				DesName = ResourceName.replace(SourcePath, DesPath)
				
	if os.path.exists(ResourceName) == True:
		DesPathMaked = os.path.dirname(DesName) 
		output("File Name is %s \n" % DesPathMaked)
		try:
			os.makedirs(DesPathMaked)			
		except os.error:
			shutil.copyfile(ResourceName, DesName)
			continue
		shutil.copyfile(ResourceName, DesName)	
	else:
		output("文件 %s 不存在" % ResourceName)
		break


#for ROW in SHEET_CHAR:
if 0:
	ResourceName = ROW["名称"]
	DesName = ROW["名称"]
	for root, dirs, files in os.walk(SourcePath):
		for name in files:
			if name == ROW["名称"]:
				ResourceName = os.path.join(root, name)
				DesName = ResourceName.replace(SourcePath, DesPath)
				
	if os.path.exists(ResourceName) == True:
		DesPathMaked = os.path.dirname(DesName) 
		output("File Name is %s \n" % DesPathMaked)
		try:
			os.makedirs(DesPathMaked)			
		except os.error:
			shutil.copyfile(ResourceName, DesName)
			continue
		shutil.copyfile(ResourceName, DesName)	
	else:
		output("文件 %s 不存在" % ResourceName)
		
#for ROW in SHEET_AVATAR:
if 0:
	ResourceName = ROW["名称"]
	DesName = ROW["名称"]
	for root, dirs, files in os.walk(SourcePath):
		for name in files:
			if name == ROW["名称"]:
				ResourceName = os.path.join(root, name)
				DesName = ResourceName.replace(SourcePath, DesPath)
				
	if os.path.exists(ResourceName) == True:
		DesPathMaked = os.path.dirname(DesName) 
		output("File Name is %s \n" % DesPathMaked)
		try:
			os.makedirs(DesPathMaked)			
		except os.error:
			shutil.copyfile(ResourceName, DesName)
			continue
		shutil.copyfile(ResourceName, DesName)	
	else:
		output("文件 %s 不存在" % ResourceName)
