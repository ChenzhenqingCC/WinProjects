#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-

import os
import re
import sys
from utils import *
from config import *

if len(sys.argv)!=4:
	print "Usage:\n\t./run_path.py <runfile> <basepath> <pathfilewithbasepath>\n"
	sys.exit(1)

path_len = len( sys.argv[2] )
if sys.argv[2] != sys.argv[3][0:path_len]:
	print "Failed:%s is not the basepath of %s" % (sys.argv[2],sys.argv[3])
	sys.exit(1)

nextstart = path_len
while sys.argv[3][nextstart] == "/" or sys.argv[3][nextstart] == "\\":
	nextstart += 1

excfile = "%s/%s" % ( PATH_TOOL_SOURCE,sys.argv[1])
filepath = sys.argv[3][nextstart:]
oldargv = sys.argv
sys.argv = [excfile,filepath]

result = execfile( excfile )
sys.argv = oldargv
sys.exit(result)
