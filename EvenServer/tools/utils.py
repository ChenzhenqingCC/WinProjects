#!/usr/local/bin/python
# -*- coding: iso-8859-1 -*-
import os


#Ö÷»ú¶¨Òå
HOST_TYPE= \
{
	"Client"		:"C",
	"Gate"			:"G",
	"Zone"			:"Z",
	"Region" 		:"R",
	"Manager"		:"M",
	"Login"			:"L",
	"AIServer"		:"I",
	"Chat"		    :"H",
	"Stat"		    :"W",
	"Assistant"		:"A",
	"Admin"			:"D",
	"Supervisor"	:"V",
	"Daemon"		:"E",
	"Downtime"		:"O",
	"Friend"		:"F",
}
#Ö÷»ú¹ØÁ´¶¨Òå
HOST_LINK= \
{
	"Client"		:["Client","Gate"],
	"Gate"			:["Gate"],
	"Zone"			:["Zone"],
	"Manager"		:["Manager"],
	"Login"			:["Login","Manager"],
	"AIServer"		:["AIServer"],
	"Chat"		    :["Chat"],
	"Stat"		   	:["Stat"],
	"Assistant"		:["Assistant"],
	"Admin"			:["Admin"],
	"Supervisor"		:["Supervisor"],
	"Daemon"		:["Daemon"],
	"Region"		:["Region"],
	"Downtime"		:["Downtime"],
	"Friend"			:["Friend"],
}
#Ö÷»ú»îÔ¾ÐÔ·ÖÀà(²»»îÔ¾µÄÖ÷»úÓëÆäÏà¹ØÐ­ÒéºÅÓÅÏÈ±£Áô)
HOST_ACTIVITY= \
{
	"Client"				:True,
	"Gate"					:True,
	"Zone"					:True,
	"Manager"				:True,
	"AIServer"				:True,
	"Chat"		  			:True,
	"Stat"		  		 	:True,
	"Assistant"				:True,
	"Admin"					:False,
	"Supervisor"			:False,
	"Daemon"				:False,
	"Region"				:True,
	"Downtime"				:False,
	"Friend"				:True,
	"Login"				:True,
}


links={}
for k,v in HOST_LINK.items():
	for host in v:
		links[host]=0

HOST_LINK["None"] = ["Types"];
links["None"]=0
ALL_HOSTS=links.keys()

SEPARATOR_PACKET = "_"
SEPARATOR_HOST = ""
SEPARATOR_PROTO = "_"
SEPARATOR_ACTION = "_"

#¶¯×÷ÀàÐÍ
ACTION_TYPE= \
{
	"Request"		:"Req",
	"Acknowledge"	:"Ack",
	"Notify"		:"Ntf",
	"Broadcast"		:"Brd"
}

#tab×Ö·ûÌî³ä
def tabStr(tabnum,format,*args):
	i=0
	f=""
	while i< tabnum:
		f += "\t"
		i += 1
	f += format
	return f % args

#´Ó½áµãÖÐ»ñµÃ×Ö·û
def getText(node,character="GBK"):
    rc = ""
    nodelist = node.childNodes
    for nod in nodelist:
        if nod.nodeType == nod.TEXT_NODE:
            rc = rc + nod.data
    return rc.encode(character)

def getAttr(node,name):
	return node.getAttribute(name).encode("GBK")

#µ÷Õû×ÖµäÖµ
def adjustDictValue(lst,new,old):
	#Éú³É¿ÉÓÃÖµ×Öµä
	i=1
	val={}
	while i < 60000:
		val[i]=0
		i += 1
	changed=0	#×ÖµäÊÇ·ñ·¢Éú±ä»¯
	#É¾³ý²»ÓÃµÄÏî
	old_items=old.items()
	for k, v in old_items:
		if new.has_key(k):
			try:
				del val[v]
			except KeyError:
				pass
			new[k]=v
		else:
			changed = 1
			
	#¸øÐÂÔöµÄÏî¸³Öµ
	for i in lst:
		if new[i]<0:
			changed = 1
			new[i]=val.popitem()[0]

	val_key={}
	new_items=new.items()
	for k, v in new_items:
		val_key[v]=k
	return [changed,val_key]

#´Ó½áµãÖÐµÃµ½ÌØ¶¨×ÓÔªËØ¼¯ºÏ
def getElements(node,name):
	res = []
	nodes = node.childNodes
	for nod in nodes:
		if nod.localName==name:
			res.append(nod)
	return res

#´Ó½áµãÖÐµÃµ½ÌØ¶¨×ÓÔªËØ
def getElement(node,name):
	res = []
	nodes = node.childNodes
	for nod in nodes:
		if nod.localName==name:
			return nod
	raise IndexError

#µ÷ÕûÁÐ±íÌî³ätable
def fillTabs(data):
	height = len(data)
	width = 0
	i = 0	
	while i < height:
		if type(data[i])==list:
			width = len(data[i])
			break
		i += 1

	i = 0
	while i < width-1:
		maxlen = 0
		j = 0
		while j < height:
			strdata = data[j][i*2]
			if len(strdata)>maxlen:
				maxlen = len(strdata)
			j += 1
		tabmun = maxlen/4 + 1
		maxlen = tabmun*4
		j = 0 
		while j < height:
			if type(data[j])==list:
				strdata = data[j][i*2]
				tabfixnum = tabmun - ( len(strdata) / 4 )
				data[j].insert(i*2+1,tabfixnum)
			j += 1
		i += 1
	return width

#tab×Ö·ûÌî³äÊý¾Ý¿é
def tabData(tabnum,data):
	width=fillTabs(data)*2-1
	res = ""
	for line in data:
		if type(line)==list:
			res += tabStr(tabnum,"%s",line[0])
			i = 1
			while i < width:
				res += tabStr(line[i],"%s",line[i+1])
				i +=2
			res += "\n"
		else:
			res += "%s\n" % line
	return res

#µÃµ½Ëù¹ØÁ´µÄÖ÷»ú
def getHostLink(fr,to):
	fr_hosts=fr.split(",")
	to_hosts=to.split(",")
	link={}
	for f in fr_hosts:
		for host in HOST_LINK[f]:
			link[host]=0
	for t in to_hosts:
		for host in HOST_LINK[t]:
			link[host]=0
	fr = fr_hosts[0]
	to = to_hosts[0]
	return [fr_hosts[0],to_hosts[0],link.keys()]

#³õÊ¼»¯hosts×Öµä
def initHostDict(dic,val):
	for host in ALL_HOSTS:
		if type(val)==list:
			dic[host]=[]
		elif type(val)==dict: 
			dic[host]={}
		else:
			dic[host]=val

#ÊÇ·ñÐèÒª¸üÐÂÎÄ¼þ
def needUpdateFile(filename,content):
	try:
		return os.read(os.open(filename,os.O_RDONLY),100000000)!=content
	except OSError , e:
		if e[0]==2:
			return 1
		else:
			raise e
			
#´Ó³¤ÎÄ¼þÃûÖÐÌáÈ¡È¥³ýÂ·¾¶ºÍºó×ºµÄ»ù±¾Ãû
def GetFileBaseName(filename) :
	begin = max(filename.rfind('\\'), filename.rfind('/'))
	begin += 1
	end = filename.rfind('.')
	if end < begin :
		end = len(filename)
	return filename[begin : end]









