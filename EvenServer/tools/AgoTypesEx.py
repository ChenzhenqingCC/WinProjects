#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
# ±¾ÎÄ¼þÓÃÓÚÀ©Õ¹×Ô¶¯½Å±¾Éú³ÉµÄPythonÀà

import sys
sys.path.append("../robot.py")
try:
	from AgoTypes import *
except ImportError:
	print "Can't import AgoTypes from robot.py"
	raise
