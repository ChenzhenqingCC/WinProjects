from _Robot import _RobotSocket,_RobotClient
import struct
import crypt

class _AgoSocket(_RobotSocket):
	def __init__(self, sock=None):
		_RobotSocket.__init__(self)
		self.xor=crypt.xor()
	
	def head_pack(self,length):
			return struct.pack("H",length+2)
	
	def head_unpack(self):
		if self.readlen<2:
				return None
		else:
			word=struct.unpack("H",self.msg[0:2])
			if word[0] < 4:
				self.sock.close();
				return None;
			self.packetlen=word[0]-2;
			self.readlen-=2
			self.msg=self.msg[2:]
			return self.packetlen
			
	def encode(self,msg):
		return msg
		return self.xor.encode(msg)

class _AgoClient(_RobotClient):
	def __init__(self):
		self.sock = _AgoSocket()
	def parsePacket(self,msg):
		word=struct.unpack("H",msg[0:2])		
		return self.parseCmd(word[0],msg[2:])

class _AgoGate(_RobotClient):
	def __init__(self):
		self.sock = _AgoSocket()
	def parsePacket(self,msg):
		word=struct.unpack("H",msg[0:2])		
		return self.parseCmd(word[0],msg[2:])