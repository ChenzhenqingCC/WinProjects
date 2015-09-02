# -*- coding: iso-8859-1 -*-

import sys
import os
import uuid
from amara.xslt import transform

start_pos = 1
params = {}
while( sys.argv[start_pos].find("-D")==0 ):
	macro = sys.argv[start_pos][2:]
	key = macro.split("=")[0]
	value = ""
	if len(macro.split("="))==2:
		value = macro.split("=")[1]
	start_pos=start_pos+1
	params[key] = value

outputFile = sys.argv[start_pos]
sourceFile = sys.argv[start_pos+1]
xslFile = sys.argv[start_pos+2]

result = transform(sourceFile, xslFile, params)

o_flag = os.O_WRONLY|os.O_CREAT|os.O_TRUNC
try:
	o_flag |= os.O_BINARY
except AttributeError:
	pass
	
os.write(os.open(outputFile,o_flag),result)
