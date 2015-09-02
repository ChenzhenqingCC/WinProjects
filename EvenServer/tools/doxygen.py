#!/usr/local/bin/python
import os

TOP_TAGS = \
[
	"Zoic",
	"rana",
	"RanaSrv",
	"RanaMysql",
	"Assistant",
	"Manager",
	"Chat",
	"Zone",
	"Gate",
	"Stat",
	"AI"
]

def doxygen(tag):
	os.chdir("..")
	os.chdir(tag)
	os.system("doxygen")

for tag in TOP_TAGS:
	doxygen(tag)

os.chdir("..")
os.chdir("tools")
