#!/usr/local/bin/python
# -*- coding: utf8 -*-

import xml.etree.cElementTree
import time

from openpyxl.reader.excel import *
from openpyxl.cell import *
from utils import *

class DataError:
	def __init__(self,msg):
		self.msg=msg
	def __str__(self):
#		return self.msg.decode(INNER_CODE).encode(LOCAL_CODE)
		return self.msg

class WorkSheetIterator:
	def __init__(self,sheet,index=0):
		self.sheet=sheet
		self.index = index
	def __iter__(self):
		return self
	def next(self):
		if self.index < len(self.sheet.rows):
			r = Row(self.sheet.title,self.sheet.rows[self.index])
			self.index += 1
			return r
		raise StopIteration()

class Row:
	def __init__(self,title,data):
		self.title=title
		self.data=data
	def __getitem__(self,value):
		return self.data[self.title[value]]

class WorkSheet:
	def __init__(self):
		self.rows=[]
		self.title={}
		self.indices = {}
		self.tmp_Row=[]
		self.cur_row=0
		self.cur_col=0
		self.title_row_index = 1
		self.name=""
	def __iter__(self):
		return WorkSheetIterator(self)
	def setData(self,row,col,value):
		if self.cur_row==0:
			#初次设置时,row,col必须为1
			if row!=1:
				raise DataError("Invalid XML File\n")
		if self.cur_row!=row:
			#上一次输入已完成,设置标题或数据
			if self.cur_row<self.title_row_index:
				pass
			elif self.cur_row==self.title_row_index:
				#设置标题
				index = 0
				self.tmp_Row.insert
				for val in self.tmp_Row:
					if self.title.has_key(val):
						raise DataError("Column[%s] & Column[%s] have the same name:%s\n" % (self.title[val],index,val))
					self.title[val]=index
					index += 1
			else:
				#设置新数据行
				tmp_len = len(self.tmp_Row)
				data_len = len(self.title)
				while tmp_len < data_len:
					self.tmp_Row.append("")
					tmp_len += 1
				self.rows.append(self.tmp_Row)
			#设置系统行号
			if row==1:
				self.tmp_Row=["#row"]
			else:
				self.tmp_Row=[row]
			self.cur_row=row
			self.cur_col=0
		self.cur_col +=1
		if self.cur_row ==1:
			#标题数据
			if self.cur_col != col:
				print self.cur_col,col,value.decode(INNER_CODE).encode(LOCAL_CODE)
				raise DataError("Invalid XML Title\n")
			self.tmp_Row.append(value)
		else:
			#数据
			data_len = len(self.title)
			if self.cur_col > col or col >= data_len:
				print "line number:%d\n" % self.cur_row
				print self.cur_col , col , data_len,"\"%s\""%value.decode(INNER_CODE).encode(LOCAL_CODE)
				raise DataError("Invalid XML Data\n")
			while self.cur_col < col:
					self.tmp_Row.append("")
					self.cur_col += 1
			self.tmp_Row.append(value)
	def find(self,name,value):
		try:
			index = self.indices[name]
		except KeyError:
			index = {}
			col = self.title[name]
			for row in self.rows:
				index[row[col]]=row
			self.indices[name] = index
		try:
			return Row(self.title,index[value])
		except KeyError:
			return None
	def unique(self,name,exp=None):
		errorStr = ""
		col = self.title[name]
		vals = {}
		row = 0
		size = len(self.rows)
		flagRaise = False
		while row < size:
			val = self.rows[row][col]
			if exp <> val:
				if vals.has_key(val):
					errorStr = "\"%s\" Unique check failed, Row[%d] & Row[%d] have the sanme value: \"%s\"\n" % (name,vals[val],self.rows[row][0],val)
					print errorStr.decode(INNER_CODE).encode(LOCAL_CODE)
					flagRaise = True
				else:
					vals[val]=self.rows[row][0]
			row += 1
		if flagRaise:
			raise DataError(errorStr)
	pass
	def getRowStartIndex(self):
		return 1 + self.title_row_index
	def getRowEndIndex(self):
		return self.getRowStartIndex() + len(self.rows)
	def getRow(self,index):
		r = Row(self.title,self.rows[index-1-self.title_row_index])
		return r
	def getTitleCount(self):
		return len(self.title)
	def hasTitle(self,title):
		return self.title.has_key(title)
	def getTitle(self):
		titleMap = {}
		for title in self.title:
			titleMap[self.title[title]] = title
		return titleMap.values()
	def setName(self,name):
		self.name = name
	def getName(self):
		return self.name

BOOKS = {}
TIMESTAMP = {}
FILESIZE = {}

def readXML(filename):
	global BOOKS
	
	print "Loading XML file \"%s\"" % filename

	dom = xml.etree.cElementTree.parse(filename).getroot()
	
	book = {}
	sheets = getElements(dom,"{urn:schemas-microsoft-com:office:spreadsheet}Worksheet")
	sheetNo = 0
	flagRaise = False
	errorStr = ""
	errorLocalStr = ""
	for nodSheet in sheets:
		sheetNo += 1
		sheet = WorkSheet()
		rows = getElements(getElement(nodSheet,"{urn:schemas-microsoft-com:office:spreadsheet}Table"),"{urn:schemas-microsoft-com:office:spreadsheet}Row")
		row = 0
		for nodRow in rows:
			try:
				row = int(nodRow.get("{urn:schemas-microsoft-com:office:spreadsheet}Index"))
			except ValueError:
				row += 1
			except TypeError:
				row += 1
			cols = getElements(nodRow,"{urn:schemas-microsoft-com:office:spreadsheet}Cell")
			col = 0
			invalidColCnt = 0
			for nodCol in cols:
				try:
					col = int(nodCol.get("{urn:schemas-microsoft-com:office:spreadsheet}Index"))
				except ValueError:
					col +=1
				except TypeError:
					col +=1
				try:
					nodeText = getText(getElement(nodCol,"{urn:schemas-microsoft-com:office:spreadsheet}Data"),INNER_CODE)
					sheet.setData(row,col,nodeText)
				except IndexError:
					pass
				except UnicodeEncodeError,e:
					tmpStr = ("File[%s], Sheet[%d], Row[%d], Col[%d]: text error\n" % ( filename,sheetNo,row,col ))
					tmpStr += str(e)
					print(tmpStr)
					errorStr += tmpStr
					flagRaise = True
					pass
		try:
			sheet.setData(-1,1,"")
			sheetName = getAttr(nodSheet,"{urn:schemas-microsoft-com:office:spreadsheet}Name",INNER_CODE)
			sheet.setName(sheetName)
			book[sheetName]=sheet
		except DataError:
			pass
			
	if flagRaise:
		tmpStr = ("File[%s] error:\n%s" % (filename, errorStr))
		fd = os.open(filename+".log", o_flag)
		os.write(fd, tmpStr)
		os.close(fd)
		raise DataError(tmpStr)
		
	BOOKS[filename] = book



def readXLS(filename):
	global BOOKS

	print "Loading XLS file \"%s\"" % filename

	logFile = os.open(filename+".log", o_flag)
	logStr = ""
	
	wb = load_workbook(filename, True)
	sheetNameList = wb.get_sheet_names()
	
	book = {}
	sheetNo = 0
	flagRaise = False
	errorStr = ""
	errorLocalStr = ""
	
	
	for sheetName in sheetNameList:
		ws = wb.get_sheet_by_name(sheetName)
		
		sheetNo += 1
		sheet = WorkSheet()

		rowNo = 0
		for row in ws.iter_rows():
			rowNo += 1
			colNo = 0
			for cell in row:
				colNo += 1
				
				try:
#					print sheetName,rowNo,colNo
#					print cell.data_type
#					print cell.internal_value
					if cell.internal_value == None:
						cellText = ""
					elif cell.data_type == Cell.TYPE_NUMERIC:
						cellText = str(cell.internal_value)
					else:
						if cell.data_type == Cell.TYPE_ERROR:
							tmpStr = ("File[%s], Sheet[%s], Row[%d], Col[%d]: excel error: %s \n" % (filename, sheetName, rowNo, colNo, cell.internal_value))
							print(tmpStr)
							errorStr += tmpStr
							flagRaise = True
						cellText = cell.internal_value.encode(INNER_CODE)
#					output(cellText)
					sheet.setData(rowNo, colNo, cellText)
					
					logStr += "[%s,%d,%d] %s\n" % (sheetName.encode(INNER_CODE), rowNo, colNo, cellText)
					
				except UnicodeEncodeError,e:
					tmpStr = ("File[%s], Sheet[%s], Row[%d], Col[%d]: text error\n" % ( filename,sheetName,rowNo,colNo ))
					tmpStr += str(e)
					print(tmpStr)
					errorStr += tmpStr
					flagRaise = True
					pass
	
		try:
			sheet.setData(-1, 1, "")
			sheet.setName(sheetName.encode(INNER_CODE))
			book[sheetName.encode(INNER_CODE)] = sheet
		except DataError:
			pass
			
	if flagRaise:
		tmpStr = ("File[%s] error:\n%s" % (filename, errorStr))
		fd = os.open(filename+".log", o_flag)
		os.write(fd, tmpStr.encode(INNER_CODE))
		os.close(fd)
		raise DataError(tmpStr)

	os.write(logFile, logStr)
	os.close(logFile)
	
	BOOKS[filename] = book


def readWorkBook(filename):
	global BOOKS
	global TIMESTAMP
	global FILESIZE

	filestat = os.stat(filename)
	if BOOKS.has_key(filename) and TIMESTAMP.has_key(filename) and FILESIZE.has_key(filename) and TIMESTAMP[filename]==filestat.st_mtime and FILESIZE[filename]==filestat.st_size:
		return BOOKS[filename]
	else:
		func_time = time.time
		t_begin = func_time()
		
		if filename[-4:] == ".xml":
			readXML(filename)
		else:
			readXLS(filename)

		TIMESTAMP[filename] = filestat.st_mtime
		FILESIZE[filename] = filestat.st_size
		
		t_end = func_time()
		print "File \"%s\" loaded in %d seconds" % (filename, int(round(t_end-t_begin)))

		return BOOKS[filename]

def reloadWorkBook(filename):
	global BOOKS
	global TIMESTAMP
	global FILESIZE

	if BOOKS.has_key(filename):
		del BOOKS[filename]
	if TIMESTAMP.has_key(filename):
		del TIMESTAMP[filename]
	if TIMESTAMP.has_key(filename):
		del FILESIZE[filename]
	readWorkBook(filename)
	
def clearAllReadBook():
	global BOOKS
	global TIMESTAMP
	global FILESIZE

	BOOKS = {}
	TIMESTAMP = {}
	FILESIZE = {}

def trim(s):
	i = 0
	j = len(s)-1
	if i > j:
		return ""	
	while s[i] == " " or s[i] == "\t" or s[i] =="\n":
		i = i + 1
	if i > j:
		return ""
	while s[j] == " " or s[j] == "\t" or s[j] =="\n":
		j = j - 1	
	if i > j:
		return ""
	s = s[i:j+1]
	return s
	
def getPos(s):
	s=trim(s)	
	result = []
	if s[0:1]=="(" and s[-1:]==")":
		tmp=s[1:-1].split(",")
		tmp0=trim(tmp[0])
		tmp1=trim(tmp[1])
		result.append(int(tmp0))
		result.append(int(tmp1))
	if len(result)==0:
		raise DataError("Invalid Position:%s" % s)
	return result

o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
try:
	o_flag |= os.O_BINARY
except AttributeError:
	pass

#生成返回脚本
def genReturnScript(script):
	if script.find("return ")==-1:
		script="return "+script
	return xmlEncode(script)
