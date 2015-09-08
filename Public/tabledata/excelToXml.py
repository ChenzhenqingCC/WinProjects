#coding='utf-8'
import sys,os,xlrd,codecs,time,string
import xml.dom.minidom
import types
from xml.etree.ElementTree import ElementTree,Element 

def read_xml(in_path):  
    '''''读取并解析xml文件 
       in_path: xml路径 
       return: ElementTree'''  
    tree = ElementTree()  
    tree.parse(in_path)  
    return tree  
  
def write_xml(tree, out_path):  
    '''''将xml文件写出 
       tree: xml树 
       out_path: 写出路径'''  
    tree.write(out_path, encoding="utf-8",xml_declaration=True)  
  
def if_match(node, kv_map):  
    '''''判断某个节点是否包含所有传入参数属性 
       node: 节点 
       kv_map: 属性及属性值组成的map'''  
    for key in kv_map:  
        if node.get(key) != kv_map.get(key):  
            return False  
    return True  
  
#---------------search -----  
  
def find_nodes(tree, path):  
    '''''查找某个路径匹配的所有节点 
       tree: xml树 
       path: 节点路径'''  
    return tree.findall(path)  
  
  
def get_node_by_keyvalue(nodelist, kv_map):  
    '''''根据属性及属性值定位符合的节点，返回节点 
       nodelist: 节点列表 
       kv_map: 匹配属性及属性值map'''  
    result_nodes = []  
    for node in nodelist:  
        if if_match(node, kv_map):  
            result_nodes.append(node)  
    return result_nodes  
  
#---------------change -----  
  
def change_node_properties(nodelist, kv_map, is_delete=False):  
    '''''修改/增加 /删除 节点的属性及属性值 
       nodelist: 节点列表 
       kv_map:属性及属性值map'''  
    for node in nodelist:  
        for key in kv_map:  
            if is_delete:   
                if key in node.attrib:  
                    del node.attrib[key]  
            else:  
                node.set(key, kv_map.get(key))  
              
def change_node_text(nodelist, text, is_add=False, is_delete=False):  
    '''''改变/增加/删除一个节点的文本 
       nodelist:节点列表 
       text : 更新后的文本'''  
    for node in nodelist:  
        if is_add:  
            node.text += text  
        elif is_delete:  
            node.text = ""  
        else:  
            node.text = text  
              
def create_node(tag, property_map, content):  
    '''''新造一个节点 
       tag:节点标签 
       property_map:属性及属性值map 
       content: 节点闭合标签里的文本内容 
       return 新节点'''  
    element = Element(tag, property_map)  
    element.text = content  
    return element  
          
def add_child_node(nodelist, element):  
    '''''给一个节点添加子节点 
       nodelist: 节点列表 
       element: 子节点'''  
    for node in nodelist:  
        node.append(element)  
          
def del_node_by_tagkeyvalue(nodelist, tag, kv_map):  
    '''''同过属性及属性值定位一个节点，并删除之 
       nodelist: 父节点列表 
       tag:子节点标签 
       kv_map: 属性及属性值列表'''  
    for parent_node in nodelist:  
        children = parent_node.getchildren()  
        for child in children:  
            if child.tag == tag and if_match(child, kv_map):  
                parent_node.remove(child)

CfGXMlPATH = "excelconfig"
CPPPATH = "excelcpp"
XMLPATH = "excelxml"
SOURCE = "excelsource"
TAGDATAS = "exceltbl"

class Field:
	name = ""
	type = ""
	size = 4
	system = 10
	
class Table:
	name = ""
	xls_name = ""
	sheet_name = ""
	sort_name = ""
	fields = {}
	

tables = {}

def endWith(*endstring):
	ends = endstring
	def run(s):
		f = map(s.endswith, ends)
		if True in f: return s
	return run
	
def IsInteger(tag):
	uptag = tag.upper()
	return uptag == "INT" or uptag == "DWORD" or uptag == "BYTE" or uptag == "LONG" or uptag == "QWORD"
	
def GetDataSize(tag):
	aa = 0

def parseConfig(path):
	search_file = os.listdir(path)
	a = endWith('.xml')
	c_files = filter(a, search_file)
	for file in c_files:
		content = open(path + "\\" + file).read()
		dom = xml.dom.minidom.parseString(content).documentElement
		for item in dom.getElementsByTagName("table"):
			table = Table()
			table.name = item.getAttribute("name")
			table.xls_name = item.getAttribute("ExcelFile")
			table.sheet_name = item.getAttribute("sheet")
			table.sort_name = item.getAttribute("sort")
			
			table.fields = {}
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
					
				table.fields[field.name] = field 
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
				rowNameSrc = sheet.cell_value(0,j)
				if table.fields.has_key(rowNameSrc) != True:
					continue
				rowName = rowNameSrc.encode("utf-8")
				rowValue = sheet.cell_value(i,j)
				field = table.fields[rowNameSrc]
				if type(rowValue) is types.UnicodeType:
					rowValue = sheet.cell_value(i,j).encode("utf-8")
				else:
					if IsInteger(field.type):
						rowValue = int(rowValue)
					rowValue = str(rowValue)
					
				s += ' %s = "%s" ' % (rowName, rowValue)
			s += '/>\n'
			f.write(s)
		f.write(u'\t</datas>\n')
		f.write(u'</config>\n')
		f.close()
	
	
def numeric_compare(node1, node2):
	global sorttag
	if len(sorttag) > 0:
		n1 = string.atof(node1.attrib[sorttag])
		n2 = string.atof(node2.attrib[sorttag])
		#print "\nn1:" + str(n1)
		#print "\nn2:" + str(n2)
		return int(n1 - n2)
	return 0
		
def xml2tbl2(xmlname):
	global sorttag
	xmlsource = read_xml(XMLPATH + "\\" + xmlname)
	nPos = xmlname.index(".xml")
	xname = xmlname[0:nPos]
	sorttag = ""
	if tables.has_key(xname):
		table = tables[xname]
		sorttag = table.sort_name
	#print table
	datas = find_nodes(xmlsource, "datas")
	dataVec = find_nodes(datas[0], "data")
	#dataVec = datas[0].findall("data")
	#datasSorted = sorted(dataVec, cmp=numeric_compare)	#可排序
	
	
	#print datasSorted
	#for node in datasSorted:
		#for k in node.attrib:
			#print k + ":" + node.attrib[k] + "\n"
	return True
	
def xml2tbl2process():
	search_file = os.listdir(XMLPATH)
	a = endWith('.xml')
	c_files = filter(a, search_file)
	for file in c_files:
		xml2tbl2(file)
		
def tablesToCSharp():
	return True
	

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
xml2tbl2process()
#tablesToXml()