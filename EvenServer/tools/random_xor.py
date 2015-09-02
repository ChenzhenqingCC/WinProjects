#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
#Éú³ÉËæ»úXOR¼ÓÃÜÐòÁÐ

import random,sys

try:
	TOTAL = int(sys.argv[1])
except:
	TOTAL=random.randint(500,1000)

MIN=100
MAX=10000
WIDTH=20
if TOTAL<MIN:
	TOTAL=MIN
if TOTAL>MAX:
	TOTAL=MAX

format="\t\"";
index=0
while index<WIDTH:
	format+="\\x%02X"
	index+=1
format+="\""

i=0
while i<TOTAL-WIDTH:
	print format % tuple(random.sample(xrange(256), WIDTH))
	i+=WIDTH
	
format="\t\"";
index=i
while index<TOTAL:
	format+="\\x%02X"
	index+=1
format+="\""

print format % tuple(random.sample(xrange(256), TOTAL-i))
