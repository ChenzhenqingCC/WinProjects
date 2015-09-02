# -*- coding: iso-8859-1 -*-
#Ago»ù±¾Ð­ÒéÊý¾ÝÀàÐÍ¶¨Òå
from _Message import *

QUEUE_FULL_INDEX = 0xFF

class PCSTR							(String):pass
class TIME_SEC						(LONG):pass
class TIME_MSEL					(INT_64):pass

class SessionHandle(STRUCT):
	def __init__(self):
		STRUCT.__init__(self)
		self.setData("handle",WORD())
		self.setData("serial",DWORD())
	def pack(self):
		res = ""
		res += self.handle.pack()
		res += self.serial.pack()
		return res
	def unpack(self,data):
		index=0
		index += self.handle.unpack(data[index:])
		index += self.serial.unpack(data[index:])
		return index

class RoleZoneHandle(SessionHandle):pass

class POS2D_W(STRUCT):
	def __init__(self):
		STRUCT.__init__(self)
		self.setData("x",WORD())
		self.setData("y",WORD())
	def pack(self):
		res = ""
		res += self.x.pack()
		res += self.y.pack()
		return res
	def unpack(self,data):
		index=0
		index += self.x.unpack(data[index:])
		index += self.y.unpack(data[index:])
		return index
	def __int__(self):
		return (int(self.y) << 16) + int(self.x)

class POS2D_S(STRUCT):
	def __init__(self):
		STRUCT.__init__(self)
		self.setData("x",SHORT())
		self.setData("y",SHORT())
	def pack(self):
		res = ""
		res += self.x.pack()
		res += self.y.pack()
		return res
	def unpack(self,data):
		index=0
		index += self.x.unpack(data[index:])
		index += self.y.unpack(data[index:])
		return index

class POS2D_L(STRUCT):
	def __init__(self):
		STRUCT.__init__(self)
		self.setData("x",LONG())
		self.setData("y",LONG())
	def pack(self):
		res = ""
		res += self.x.pack()
		res += self.y.pack()
		return res
	def unpack(self,data):
		index=0
		index += self.x.unpack(data[index:])
		index += self.y.unpack(data[index:])
		return index

class GRADS_C(STRUCT):
	def __init__(self):
		STRUCT.__init__(self)
		self.setData("dx",CHAR())
		self.setData("dy",CHAR())
	def pack(self):
		res = ""
		res += self.dx.pack()
		res += self.dy.pack()
		return res
	def unpack(self,data):
		index=0
		index += self.dx.unpack(data[index:])
		index += self.dy.unpack(data[index:])
		return index

class GRADS_S(STRUCT):
	def __init__(self):
		STRUCT.__init__(self)
		self.setData("dx",SHORT())
		self.setData("dy",SHORT())
	def pack(self):
		res = ""
		res += self.dx.pack()
		res += self.dy.pack()
		return res
	def unpack(self,data):
		index=0
		index += self.dx.unpack(data[index:])
		index += self.dy.unpack(data[index:])
		return index

class GRADS_L(STRUCT):
	def __init__(self):
		STRUCT.__init__(self)
		self.setData("dx",LONG())
		self.setData("dy",LONG())
	def pack(self):
		res = ""
		res += self.dx.pack()
		res += self.dy.pack()
		return res
	def unpack(self,data):
		index=0
		index += self.dx.unpack(data[index:])
		index += self.dy.unpack(data[index:])
		return index

class DirectType(GRADS_C):pass

class RoleHandle(STRUCT):
	def __init__(self):
		STRUCT.__init__(self)
		self.setData("handle",WORD())
		self.setData("serial",DWORD())
	def pack(self):
		res = ""
		res += self.handle.pack()
		res += self.serial.pack()
		return res
	def unpack(self,data):
		index=0
		index += self.handle.unpack(data[index:])
		index += self.serial.unpack(data[index:])
		return index
