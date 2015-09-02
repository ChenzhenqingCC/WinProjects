#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
import xml.dom.minidom
import os
import re
import glob
import stat
from utils import *
from config import *

#����ԭ�ȵ����ݶ���
import sys
sys.path.append(PATH_PROTO_SOURCE)
try:
	from ver_def import *
except ImportError:
	VER = 0

content_def="""#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-

VER = """
content_cpp="""#include <Zoic/include/Types.h>

namespace Proto
{
"""
#���cmd_def.py��Proto_Command.cpp�����ڣ�
#��Proto_Command.cpp���ļ�����ʱ�䲻С��cmd_def.py,�汾�Ÿ���
def_file = "%s/ver_def.py" % PATH_PROTO_SOURCE
cpp_file = "%s/Proto_Version.cpp" % PATH_PROTO_TARGET
changed = 0
try:
	#def_fd = os.open(def_file,os.O_RDONLY)
	#cpp_fd = os.open(cpp_file,os.O_RDONLY)
	def_stat = os.stat(def_file)
	cpp_stat = os.stat(cpp_file)
	if cpp_stat[stat.ST_MTIME] >= def_stat[stat.ST_MTIME]:
		VER += 1
		changed = 1
except OSError,e:
	pass

content_def += "%d\n" % VER
content_cpp += "\textern const DWORD version=%d;\n" % VER
content_cpp += "}\n"

o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
try:
	o_flag |= os.O_BINARY
except AttributeError:
	pass

if changed:
	os.write(os.open(def_file,o_flag),content_def)
os.write(os.open(cpp_file,o_flag),content_cpp)
