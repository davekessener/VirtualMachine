import sys
import threading
import select
import socket
import time
import nbt

TIME_OUT = 10

STR_ACTION = 'action'
STR_VERSION = 'version'
STR_DATA = 'data'
STR_ERR_OUTDATED = 'outdated server'
STR_ERR_MISSING = 'versioning required'
STR_ERRMSG = 'error'

class Connect:
	def __init__(self, addr):
		self._s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self._s.connect(addr)

	def __enter__(self):
		return self._s
	
	def __exit__(self, ex_t, ex_v, tr):
		if self._s:
			self._s.close()
		return True
	
# ==============================================================================

def SendPacket(cs, tag):
	tag.write(cs)

def ReceivePacket(cs):
	return nbt.NBTBase.Read(cs)

def Communicate(server, tag):
	with Connect(server) as cs:
		SendPacket(cs, tag)
		return ReceivePacket(cs)
	
# ==============================================================================

class ClientResponder(threading.Thread):
	def __init__(self, client, acceptor):
		threading.Thread.__init__(self)
		self._acceptor = acceptor
		self._clientsocket, self._addr = client
		self.start()
	
	def run(self):
		try:
			tag = ReceivePacket(self._clientsocket)
			self._acceptor(self._addr, tag, self)
		except Exception as e:
			print('Client interrupted \'%s\'!' % str(e))
		self._clientsocket.close()

	def sendPacket(self, tag):
		SendPacket(self._clientsocket, tag)

class Listener:
	def __init__(self, factory, addr):
		self._factory = factory
		self._addr = addr
		self._running = False
		self._prompt = False

	def start(self):
		self._running = True
		clients = []
		ssock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			ssock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
			ssock.bind(self._addr)
			ssock.listen(5)
			self._prompt = True
			while self._running:
				self.prompt()
				ins, outs, exs = select.select([ssock, sys.stdin], [], [])
				for s in ins:
					if s == ssock:
						(cs, addr) = ssock.accept()
						cs.settimeout(TIME_OUT)
						clients.append(ClientResponder((cs, addr), self._factory))
					elif s == sys.stdin:
						cmd = sys.stdin.readline()[:-1]
						self._prompt = True
						if cmd and len(cmd) > 0:
							self._factory.execute(cmd)
				time.sleep(0.1)
		except Exception as e:
			print('Listener interrupted \'%s\'!' % str(e))
		if ssock:
			ssock.close()
		for client in clients:
			client.join()
		sys.stdout.write('\n')

	def log(self, data):
		if not self._prompt:
			sys.stdout.write('\n')
		print(data)
		self._prompt = True

	def prompt(self):
		if self._prompt:
			sys.stdout.write('[Server]: $ ')
			sys.stdout.flush()
			self._prompt = False

	def quit(self):
		self._running = False

# ------------------------------------------------------------------------------

class VersionedClient:
	def __init__(self, addr, version):
		self.addr, self.version = addr, version
	
	def communicate(self, action, data = None):
		tag = nbt.TAG_Compound()
		tag.setString(STR_ACTION, action)
		tag.setString(STR_VERSION, self.version)
		if data:
			tag.setCompoundTag(STR_DATA, data)
		return self.accept(Communicate(self.addr, tag))

	def accept(self, tag):
		if not tag.hasTag(STR_ACTION):
			return (tag.getString(STR_VERSION), tag.getCompoundTag(STR_DATA))
		else:
			e = tag.getString(STR_ACTION)
			if tag.hasTag(STR_ERRMSG):
				e = '%s [%s]' % (e, tag.getString(STR_ERRMSG))
			raise Exception(e)

class VersionedResponder:
	def __init__(self, exe = None):
		self._versions = []
		self._exe = exe

	def __call__(self, addr, tag, responder):
		class Tunnel:
			def __init__(self, r, v):
				self._r = r
				self._v = v
			def sendPacket(self, tag):
				tag.setString(STR_VERSION, self._v)
				self._r.sendPacket(tag)
		if tag.hasTag(STR_VERSION) and tag.hasTag(STR_ACTION):
			vs, vo = self.getVersion(tag)
			if vo:
				vo(addr, tag, Tunnel(responder, vs))
			else:
				responder.sendPacket(VersionedResponder.Outdated())
		else:
			responder.sendPacket(VersionedResponder.Missing())

	def execute(self, cmd):
		if self._exe:
			self._exe.execute(cmd)

	def addVersion(self, resp):
		vs, vo = resp
		if self.hasExactVersion(vs):
			raise Exception('ERR: Version \'%s\' already exists.' % vs)
		else:
			self._versions.append((VersionedResponder.Convert(vs), vo))
			self._versions.sort()

	def getVersion(self, tag):
		v = None
		if tag.hasTag(STR_VERSION):
			s = tag.getString(STR_VERSION)
			v = self.hasVersion(s)
			if v:
				vn, vo = v;
				v = (VersionedResponder.Revert(vn), vo)
			else:
				v = (s, None)
		return v

	def hasExactVersion(self, vs):
		vn = VersionedResponder.Convert(vs)
		return next((x for x in self._versions if x[0] == vn), None)

	def hasVersion(self, vs):
		vn = VersionedResponder.Convert(vs)
		return next((x for x in self._versions if x[0] >= vn), None)

	@staticmethod
	def Convert(v):
		return ''.join(chr(int(s)) for s in v.split('.')).ljust(4, '\x00')

	@staticmethod
	def Revert(v):
		return '.'.join(('%d' % ord(c)) for c in v.ljust(4, '\x00'))
	
	@staticmethod
	def Outdated():
		t = nbt.TAG_Compound()
		t.setString(STR_ACTION, STR_ERR_OUTDATED)
		return t
	
	@staticmethod
	def Missing():
		t = nbt.TAG_Compound()
		t.setString(STR_ACTION, STR_ERR_MISSING)
		return t

