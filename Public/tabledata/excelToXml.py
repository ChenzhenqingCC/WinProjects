#coding='utf-8'
import sys,os,xlrd,codecs,time
import xml.dom.minidom
import types

CfGXMlPATH = "excelconfig"
CPPPATH = "excelcpp"
XMLPATH = "excelxml"
SOURCE = "excelsource"

class Field:
	name = ""
	type = ""
	size = 4
	system = 10
	
class Table:
	name = ""
	xls_name = ""
	sheet_name = ""
	fields = []
	

tables = {}

def endWith(*endstring):
	ends = endstring
	def run(s):
		f = map(s.endswith, ends)
		if True in f: return s
	return run

def parseConfig(path):
	search_file = os.listdir(path)
	a = endWith('.xml')
	c_files = filter(a, search_file)
	for file in c_files:
		content = open(path + "\\" + file).read()
		#print content
		dom = xml.dom.minidom.parseString(content).documentElement
		for item in dom.getElementsByTagName("table"):
			table = Table()
			table.name = item.getAttribute("name")
			table.xls_name = item.getAttribute("ExcelFile")
			table.sheet_name = item.getAttribute("sheet")
			
			table.fields = []
			for ele in item.getElementsByTagName("field"):
				field = Field()
				field.name = ele.getAttribute("name")
				field.type = ele.getAttribute("type")
				
				try:
					field.size = int(ele.getAttribute("size"))
				except:
					pass
					
				try:
					field.system = int(ele.getAttribute("system"))
				except:
					pass
					
				table.fields.append(field)
			tables[table.name] = table

def getPathFileName(path):
	tmp = path.split('/');
	return tmp[len(tmp) -1 ].split('.')[0];

def xlsToxmlPath(path):
	return path.split('.')[0] + '.xml';
	
def tablesToXml():
	for tname in tables:
		table = tables[tname]
		tablePath = SOURCE + "\\" + table.xls_name
		xmlPath = XMLPATH + "\\" + table.name + ".xml"
		data = xlrd.open_workbook(tablePath)
		sheet = data.sheet_by_name(table.sheet_name)
		f = open(xmlPath, 'wb')
		f.write(u'<?xml version="1.0" encoding="utf-8" ?>\n')
		f.write(u'<config>\n')
		f.write(u'\t<datas>\n')
		
		for i in range(1, sheet.nrows):
			s = '\t\t<data';
			for j in range(sheet.ncols):
				rowName = sheet.cell_value(0,j).encode("utf-8")
				rowValue = sheet.cell_value(i,j)
				
				if type(rowValue) is types.UnicodeType:
					rowValue = sheet.cell_value(i,j).encode("utf-8")
				else:
					rowValue = str(rowValue)
					
				s += ' %s = "%s" ' % (rowName, rowValue)
			s += '/>\n'
			f.write(s)
		f.write(u'\t</datas>\n')
		f.write(u'</config>\n')
		f.close()
		
def tablesToCSharp():
	

def export(path):
	data = xlrd.open_workbook(path)
	table = data.sheets()[0]
	print xlsToxmlPath(path)
	f = open(xlsToxmlPath(path), 'wb')
	f.write(u'<?xml version="1.0" encoding="utf-8" ?>\n')
	f.write(u'<%s>\n' % getPathFileName(path))
	for i in range(1, table.nrows):
		s = u'<item ';
		tmp = [u'%s = "%s"' % (str(table.cell_value(0,j)), str(table.cell_value(i,j))) for j in range(table.ncols)]
		s += u' '.join(map(str, tmp))
		s += u'>\n'
		f.write(s)
	f.write(u'</%s>' % getPathFileName(path))
	
#export('C:/Users/chenzhenqing/Desktop/xlrd-0.9.4/1111.xlsx')
parseConfig(CfGXMlPATH)
tablesToXml()