#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
import xml.dom.minidom
import os
import re
import glob
from utils import *
from config import *
from random import *

FIXHOST = {}
ACTIVEHOST = {}
for k,v in HOST_TYPE.items():
	if k in HOST_ACTIVITY:
		if v in FIXHOST:
			print("Warning: Duplicate Server Host \"%s\":%s,%s !" % (v,FIXHOST[v],k) )
			raise "Error: Duplicate Server Host \"%s\":%s,%s !" % (v,FIXHOST[v],k)
			continue
		if v in ACTIVEHOST:
			print("Warning: Duplicate Server Host \"%s\":%s,%s !" % (v,ACTIVEHOST[v],k) )
			raise "Error: Duplicate Server Host \"%s\":%s,%s !" % (v,ACTIVEHOST[v],k)
			del ACTIVEHOST[v]
		if HOST_ACTIVITY[k] == True:
			ACTIVEHOST[v] = k
		else:
			FIXHOST[v] = k

#ÔØÈëÔ­ÏÈµÄÊý¾Ý¶¨Òå
import sys
sys.path.append(PATH_PROTO_SOURCE)
try:
	from fixed_cmd_def import *
except ImportError:
	CMDS = {}

FIX_CMDS = CMDS
CMDS = {}

try:
	from cmd_def import *
except ImportError:
	CMDS = {}

ACTIVE_CMDS = CMDS
CMDS = {}

def isFixCmd(cmd_name):
	global ACTIVEHOST
	hosts = [ cmd_name[0],cmd_name[1] ]
	for str in hosts:
		if not( str in ACTIVEHOST ):
			return True
	return False

changed = False


for k,v in ACTIVE_CMDS.items():
	if isFixCmd(k):
		if k in FIX_CMDS:
			continue
		changed = True
	elif k in FIX_CMDS and FIX_CMDS[k] <> v:
		changed = True
	CMDS[k] = v
	
for k,v in FIX_CMDS.items():
	if not( isFixCmd(k) ):
		if k in ACTIVE_CMDS:
			continue
		changed = True
	elif not( k in ACTIVE_CMDS):
		changed = True
	elif ACTIVE_CMDS[k] <> v:
		changed = True
	CMDS[k] = v

#µ±Ç°ÃüÁî¶¨Òå
CMD_LIST = []
CUR_CMDS = {}
CUR_DESC = {}

def handleMessage(message):
	global CMD_LIST
	descs=getText(getElement(message,"Description")).split("\n");
	name=getText(getElement(message,"Name"))
	type=getText(getElement(message,"Type"))
	fr_hosts=getText(getElement(message,"From")).replace("\n",",")
	to_hosts=getText(getElement(message,"To")).replace("\n",",")
	hosts_info=getHostLink(fr_hosts,to_hosts)
	fr=hosts_info[0]
	to=hosts_info[1]
	cmd = HOST_TYPE[fr] + SEPARATOR_HOST + HOST_TYPE[to] + SEPARATOR_PROTO + name.upper() + SEPARATOR_ACTION + ACTION_TYPE[type].upper()
	global CUR_CMDS
	global CUR_DESC
	CMD_LIST.append(cmd)
	CUR_CMDS[cmd]=-1
	CUR_DESC[cmd]="%s %s->%s" % (descs[0],fr_hosts,to_hosts)

def handleProto(proto):
	global handleMessage
	#ÏûÏ¢´¦Àí
	messages=getElements(getElement(getElement(proto,"Proto"),"Messages"),"Message")
	for message in messages:
		handleMessage(message)

files=glob.glob("%s/Proto_*.xml" % PATH_PROTO_SOURCE)

for file in files:
	dom = xml.dom.minidom.parse(file)
	handleProto(dom)

seed()
shuffle(CMD_LIST)
res=adjustDictValue(CMD_LIST,CUR_CMDS,CMDS)
if not(changed):
	changed = res[0]
cmds_map = res[1]
cmds_items=cmds_map.items()

content_def="""#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-

CMDS = \\
{
"""
fix_content_def = content_def
content_cpp="""#include <Zoic/include/Types.h>

namespace Proto
{
"""
content_py="""# -*- coding: iso-8859-1 -*-
"""
data_def=[]
fix_data_def=[]
data_cpp=[]
data_py=[]
k = 0
while k < 60000:
	if k in cmds_map:
		v = cmds_map[k]
		if isFixCmd(v):
			fix_data_def.append(["\"%s\"" % v,":%d," % k])
		data_def.append(["\"%s\"" % v,":%d," % k])
		data_cpp.append(["extern const WORD %s" % v,"=%d;" % k,"//0x%04X %s" % (k,CUR_DESC[v])])
		data_py.append([v,"=%d" % k,"#0x%04X %s" % (k,CUR_DESC[v])])
	k += 1
fix_content_def += tabData(1,fix_data_def)
content_def += tabData(1,data_def)
content_cpp += tabData(1,data_cpp)
content_py += tabData(0,data_py)
fix_content_def += "}\n"
content_def += "}\n"
content_cpp += "}\n"

filename = "%s/Proto_Command.cpp" % PATH_PROTO_TARGET
o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
try:
	o_flag |= os.O_BINARY
except AttributeError:
	pass
os.write(os.open(filename,o_flag),content_cpp)
#changed=1
if changed:
	os.write(os.open("%s/fixed_cmd_def.py" % PATH_PROTO_SOURCE,o_flag),fix_content_def)
	os.write(os.open("%s/cmd_def.py" % PATH_PROTO_SOURCE,o_flag),content_def)
os.write(os.open("%s/protoup.x" % PATH_PROTO_SOURCE,o_flag),"")
os.write(os.open("%s/AgoCommand.py" % PATH_ROBOT_TARGET,o_flag),content_py)
print "Proto Command Updated."
