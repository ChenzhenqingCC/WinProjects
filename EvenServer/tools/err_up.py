#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
import xml.dom.minidom
import os
import re
import glob
from utils import *
from config import *

#ÔØÈëÔ­ÏÈµÄÊý¾Ý¶¨Òå
import sys
sys.path.append(PATH_PROTO_SOURCE)
try:
	from err_def import *
except ImportError:
	ERRS = {"ERROR_OK":0}

#µ±Ç°ÃüÁî¶¨Òå
ERR_LIST = []
CUR_ERRS = {}
CUR_DESC = {}

def handleError(error):
	#ÏûÏ¢´¦Àí
	items=getElements(getElement(error,"Error"),"Item")
	for item in items:
		handleErrorItem(item)

def handleErrorItem(item):
	descs=getText(getElement(item,"Description")).split("\n");
	name=getText(getElement(item,"Name"))
	err = "ERROR_%s" % name
	global CUR_ERRS
	global CUR_DESC
	ERR_LIST.append(err)
	CUR_ERRS[err]=-1
	CUR_DESC[err]=descs[0]

file = "%s/Error.xml" % PATH_PROTO_SOURCE
dom = xml.dom.minidom.parse(file)
handleError(dom)

res=adjustDictValue(ERR_LIST,CUR_ERRS,ERRS)
changed = res[0]
errs_items=res[1].items()

content_def="""#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-

ERRS = \\
{
"""
content_cpp="""#include <Zoic/include/Types.h>

namespace Proto
{
	typedef WORD ErrorType;
"""
content_py="""# -*- coding: iso-8859-1 -*-
"""
content_lua=""
data_def=[]
data_cpp=[]
data_py=[]
data_lua=[]
for k,v in errs_items:
	data_def.append(["\"%s\"" % v,":%d," % k])
	data_cpp.append(["extern const ErrorType %s" % v,"=%d;" % k,"//0x%04X %s" % (k,CUR_DESC[v])])
	data_py.append([v,"=%d" % k,"#0x%04X %s" % (k,CUR_DESC[v])])
	data_lua.append([v,"=%d" % k,"--0x%04X %s" % (k,CUR_DESC[v])])
content_def += tabData(1,data_def)
content_cpp += tabData(1,data_cpp)
content_py += tabData(0,data_py)
content_lua += tabData(0,data_lua)
content_def += "}\n"
content_cpp += "}\n"
content_lua += "\n"

filename = "%s/Proto_Error.cpp" % PATH_PROTO_TARGET
o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
try:
	o_flag |= os.O_BINARY
except AttributeError:
	pass
os.write(os.open(filename,o_flag),content_cpp)
if changed:
	os.write(os.open("%s/err_def.py" % PATH_PROTO_SOURCE,o_flag),content_def)
os.write(os.open("%s/AgoError.py" % PATH_ROBOT_TARGET,o_flag),content_py)
os.write(os.open("%s/rana/error.lua" % PATH_ETC_TARGET,o_flag),content_lua)
