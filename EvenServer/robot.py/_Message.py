# -*- coding: iso-8859-1 -*-
#»ù±¾Ð­ÒéÊý¾ÝÀàÐÍ¶¨Òå
import struct
class Message(object):
	def __init__(self,data=""):
		self.data=data
		self.index=0
	def __lshift__(self,other):
		self.data+=other.pack()
		return self
	def __rshift__(self,other):
		self.index+=other.unpack(self.data[self.index:])
		return self
	def __str__(self):
		return self.data
	def __len__(self):
		return len(self.data)
	def __add__(self,other):
		return self.data+other
	def __radd__(self,other):
		return other+self.data
	def ID(self,cmd):
		self << WORD(cmd)
		return self

class Object(object):
	def getData(self):
		return object.__getattribute__(self,"__data")
	def setData(self,data):
		object.__setattr__(self,"__data",data)
	def __str__(self):
		return Object.getData(self).__str__()
	def __repr__(self):
		return Object.getData(self).__repr__()

class Simple:
	'''¼òµ¥Êý¾ÝÀàÐÍ'''
	def __add__(self,other):
		return self.getData()+other
	def __radd__(self,other):
		return other+self.getData()

class Integer(Simple,Object):
	def __init__(self,val=0):
		self.setData(val)
	def setData(self,data):
		Object.setData(self,int(data));
	def __getattr__(self,attr):
		raise AttributeError("'Integer' object has no attribute '%s'" % attr)
	def __setattr__(self,attr,val):
		raise AttributeError("'Integer' object has no attribute '%s'" % attr)
	def __cmp__(self,other):
		data = self.getData()
		if data > other:
			return 1
		elif data < other:
			return -1
		else:
			return 0
	def __int__(self):
		return self.getData().__int__()
	def __long__(self):
		return self.getData().__long__()

class Integer_Long(Simple,Object):
	def __init__(self,val=0):
		self.setData(val)
	def setData(self,data):
		Object.setData(self,long(data));
	def __getattr__(self,attr):
		raise AttributeError("'Integer' object has no attribute '%s'" % attr)
	def __setattr__(self,attr,val):
		raise AttributeError("'Integer' object has no attribute '%s'" % attr)
	def __cmp__(self,other):
		data = self.getData()
		if data > other:
			return 1
		elif data < other:
			return -1
		else:
			return 0
	def __int__(self):
		return self.getData().__int__()
	def __long__(self):
		return self.getData().__long__()

class BYTE(Integer):
	def pack(self):
		return struct.pack("B",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("B",data[0:1])[0])
		return 1

class CHAR(Integer):
	def pack(self):
		return struct.pack("b",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("b",data[0:1])[0])
		return 1

class WORD(Integer):
	def pack(self):
		return struct.pack("H",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("H",data[0:2])[0])
		return 2

class SHORT(Integer):
	def pack(self):
		return struct.pack("h",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("h",data[0:2])[0])
		return 2

class DWORD(Integer):
	def pack(self):
		return struct.pack("I",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("I",data[0:4])[0])
		return 4

class QWORD(Integer_Long):
	def pack(self):
		return struct.pack("Q",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("Q",data[0:8])[0])
		return 8

class INT(Integer):
	def pack(self):
		return struct.pack("i",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("i",data[0:4])[0])
		return 4

class LONG(INT):
	pass

class INT_64(Integer_Long):
	def pack(self):
		return struct.pack("q",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("q",data[0:8])[0])
		return 8

class Float(Simple,Object):
	def __init__(self,val=0):
		self.setData(val)
	def setData(self,data):
		Object.setData(self,float(data));
	def __getattr__(self,attr):
		raise AttributeError("'FLOAT' object has no attribute '%s'" % attr)
	def __setattr__(self,attr,val):
		raise AttributeError("'FLOAT' object has no attribute '%s'" % attr)
	def __cmp__(self,other):
		data = self.getData()
		if data > other:
			return 1
		elif data < other:
			return -1
		else:
			return 0
	def __int__(self):
		return self.getData().__int__()
	def __long__(self):
		return self.getData().__long__()

class FLOAT(Float):
	def pack(self):
		return struct.pack("f",self.getData())
	def unpack(self,data):
		self.setData(struct.unpack("f",data[0:4])[0])
		return 4

class String(Simple,Object):
	def __init__(self,val=""):
		self.setData(val)
	def setData(self,data):
		Object.setData(self,str(data));
	def __getattr__(self,attr):
		raise AttributeError("'String' object has no attribute '%s'" % attr)
	def __setattr__(self,attr,val):
		raise AttributeError("'String' object has no attribute '%s'" % attr)
	def __cmp__(self,other):
		return self.getData().__cmp__(other)
	def __getString(self,msg):
		res=""
		length = len(msg)
		index=0
		while index < length:
			if msg[index:index+1]=='\0':
				break;
			res += msg[index:index+1]
			index +=1
		return res
	def pack(self):
		return self.getData()+"\0"
	def unpack(self,data):
		res = self.__getString(data)
		self.setData(res)
		return len(res)+1

class STRUCT(Object):
	def __init__(self):
		Object.setData(self,{})
	def setData(self,name,val):
		Object.getData(self)[name]=val
	def getData(self,name):
		return Object.getData(self)[name]
	def __getattr__(self,attr):
		data=Object.getData(self)
		if data.has_key(attr):
			return data[attr]
		else:
			raise AttributeError("'STRUCT' object has no attribute '%s'" % attr)
	def __setattr__(self,attr,val):
		data=Object.getData(self)
		if data.has_key(attr):
			attr_class=data[attr].__class__
			if isinstance(val,attr_class):
				data[attr]=val
			elif issubclass(attr_class,Simple):
				data[attr].setData(val)
			else:
				raise TypeError("STRUCT attribute type error.")
		else:
			raise AttributeError("'STRUCT' object has no attribute '%s'" % attr)

class VECTOR(Object):
	def __init__(self,element_class):
		object.__setattr__(self,"__element_class",element_class)
		Object.setData(self,[])
	def element(self):
		return object.__getattribute__(self,"__element_class")()
	def __getattr__(self,attr):
		raise AttributeError("'VECTOR' object has no attribute '%s'" % attr)
	def __setattr__(self,attr,val):
		raise AttributeError("'VECTOR' object has no attribute '%s'" % attr)
	def pack(self):
		data = self.getData()
		size=BYTE(len(data))
		res = size.pack()
		i=0
		while i < size:
			res += data[i].pack()
			i+=1
		return res
	def unpack(self,data):
		_data = self.getData()
		index=0
		size=BYTE()
		index += size.unpack(data[index:])
		i = 0
		while i < size:
			ele=self.element()
			index += ele.unpack(data[index:])
			_data.append(ele)		
			i+=1
		return index
	def __len__(self):
		return self.getData().__len__()
	def append(self,ele):
		ele_class=object.__getattribute__(self,"__element_class")
		if issubclass(ele_class,Simple):
			self.getData().append(ele_class(ele))
		else:
			if not isinstance(ele,ele_class):
				raise TypeError("VECTOR element type error.")
			self.getData().append(ele)
	def __getitem__(self,key):
		return self.getData().__getitem__(key)
	def __setitem__(self,key,ele):
		ele_class=object.__getattribute__(self,"__element_class")
		if isinstance(ele,ele_class):
			return self.getData().__setitem__(key,ele)
		elif issubclass(ele_class,Simple):
			self.getData().__setitem__(key,ele_class(ele))
		else:
			raise TypeError("VECTOR element type error.")

class VAR_ARRAY(VECTOR):
	def __init__(self,element_class,size_class=WORD):
		VECTOR.__init__(self,element_class)
		object.__setattr__(self,"__size_class",size_class)
	def pack(self):
		data = self.getData()
		size=object.__getattribute__(self,"__size_class")( len(data) )
		res = size.pack()
		i=0
		while i < size:
			res += data[i].pack()
			i+=1
		return res
	def unpack(self,data):
		_data = self.getData()
		index=0
		size=object.__getattribute__(self,"__size_class")()
		index += size.unpack(data[index:])
		i = 0
		while i < size:
			ele=self.element()
			index += ele.unpack(data[index:])
			_data.append(ele)		
			i+=1
		return index
	def __getattr__(self,attr):
		raise AttributeError("'VAR_ARRAY' object has no attribute '%s'" % attr)
	def __setattr__(self,attr,val):
		raise AttributeError("'VAR_ARRAY' object has no attribute '%s'" % attr)

class FIX_ARRAY(VECTOR):
	def __init__(self,element_class,count):
		VECTOR.__init__(self,element_class)
		object.__setattr__(self,"__count",count)
	def pack(self):
		data = self.getData()
		size=WORD(len(data))
		count = object.__getattribute__(self,"__count")
		if size > count:
			size = count
		i=0
		res = ""
		while i < size:
			res += data[i].pack()
			i+=1
		if i < count:
			while i < count:
				res += self.element().pack()
				i+=1
		return res
	def unpack(self,data):
		Object.setData(self,[])
		_data = self.getData()
		index=0
		size = object.__getattribute__(self,"__count")
		i = 0
		while i < size:
			ele=self.element()
			index += ele.unpack(data[index:])
			_data.append(ele)		
			i+=1
		return index
	def __getattr__(self,attr):
		raise AttributeError("'FIX_ARRAY' object has no attribute '%s'" % attr)
	def __setattr__(self,attr,val):
		raise AttributeError("'FIX_ARRAY' object has no attribute '%s'" % attr)

def hexCharToVal(hexChar):
	ch = struct.unpack("B",hexChar)[0]
	if ch >= 48 and ch <= 57:
		return ch - 48
	if ch >= 97 and ch <= 102:
		return ch - 97 + 10
	if ch >= 65 and ch <= 70:
		return ch - 65 + 10
	return 16
		
def getHexData(hexStr):
	size = 0
	data = []
	strLen = len(hexStr)
	i = 0
	while i < strLen:
		num = hexStr[i]
		if num == " " or num == "\t":
			i += 1
			continue
		k = hexCharToVal(num)
		if k >= 16:
			return []
		if i+1 < strLen:
			num2 = hexStr[i+1]
			j = hexCharToVal(num2)
		else:
			j = 16
		if j < 16:
			i += 1
			data.append(16*k+j)
		else:
			data.append(k)
		i += 1
	return data
	
class TinyBinary(VAR_ARRAY):
	def __init__(self):
		VAR_ARRAY.__init__(self,BYTE)
	def setData(self,data):
		if isinstance(data,str):
			_data = Object.getData(self)
			del _data[:]
			hexData = getHexData(data)
			for i in hexData:
				self.append(i)		

class Binary(VAR_ARRAY):
	def __init__(self):
		ARRAY.__init__(self,CHAR)
	def setData(self,data):
		if isinstance(data,str):
			_data = Object.getData(self)
			del _data[:]
			for ch in data:
				self.append(struct.unpack("b",ch)[0])
			self.append(0)