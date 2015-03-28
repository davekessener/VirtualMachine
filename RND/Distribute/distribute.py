import sys
import socket

import nbt
import server

STR_ACT_ADD = "add"
STR_ACT_REROUTE = "reroute"
STR_NAME = "name"
STR_PORT = "port"
STR_HOST = "host"

DEFAULT_HOST = "localhost"
DEFAULT_PORT = 8000

IP_GOOGLE_DNS = '8.8.8.8'

def Reroute(name, addr, tag):
	t = nbt.TAG_Compound()
	t.setCompoundTag(server.STR_DATA, tag)
	t.setString(server.STR_ACTION, STR_ACT_REROUTE)
	t.setString(STR_NAME, name)
	return server.Communicate(addr, t)

def MakeRerouter(name):
	def action(addr, tag):
		return Reroute(name, addr, tag)
	return action

# ==============================================================================

def GetIP():
	ip = socket.gethostbyname(socket.gethostname())
	if ip == '127.0.0.1' or ip == 'localhost':
		try:
			s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
			s.connect((IP_GOOGLE_DNS, 80))
			ip, port = s.getsockname()
			s.close()
		except Exception as e:
			ip = 'localhost'
	return ip

def GetPort():
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.bind(('', 0))
		ip, port = s.getsockname()
		s.close()
	except Exception as e:
		port = 0
	return port

def Register(name, addr):
	host, port = addr
	tag = server.SSTag(STR_ACT_ADD)
	payload = nbt.TAG_Compound()
	payload.setString(STR_NAME, name)
	if host != 'localhost':
		tag.setString(STR_HOST, GetIP())
		tag.setInt(STR_PORT, GetPort())
	tag.setCompoundTag(server.STR_DATA, payload)
	r = server.Communicate(addr, tag)
	if not r.hasTag(server.STR_ACTION):
		tag = r.getCompoundTag(server.STR_DATA)
		return (tag.getString(STR_HOST), tag.getInt(STR_PORT))
	else:
		raise Exception('ERR(%s): Trying to register \'%s\': \'%s\'' % \
			(r.getString(server.STR_ACTION), name, r.getString(server.STR_ERRMSG) if r.hasTag(server.STR_ERRMSG) else ''))

def Listener(factory, name, addr):
	return server.Listener(factory, Register(name, addr))

# ==============================================================================

class Server:
	def __init__(self, addr):
		self._port = DEFAULT_PORT
		self._servers = {}
		self._listener = server.Listener(self, addr)
		self._listener.start()

	def __call__(self, addr, tag, responder):
		try:
			act = tag.getString(server.STR_ACTION)
			responder.sendPacket({
				STR_ACT_ADD : lambda: server.DataTag(self.addServer(tag.getCompoundTag(server.STR_DATA))),
				STR_ACT_REROUTE : lambda: self.reroute(tag)
			}.get(act, lambda: server.UnknownRequest(act))())
		except Exception as e:
			responder.sendPacket(server.InvalidTag(e))

	def execute(self, cmd):
		{
			"quit" : lambda: self._listener.quit()
		}.get(cmd, lambda: sys.stdout.write("Unknown command!\n"))()
	
	def addServer(self, tag):
		name = tag.getString(STR_NAME)
		if name in self._servers:
			addr = self._servers[name]
			host, port = addr
			del self._servers[name]
			if tag.hasTag(STR_HOST):
				host = tag.getString(STR_HOST)
				if host != addr[0]:
					port = tag.getInt(STR_PORT) if tag.hasTag(STR_PORT) else self.nextPort(host)
			if tag.hasTag(STR_PORT):
				port = tag.getInt(STR_PORT)
		else:
			host = tag.getString(STR_HOST) if tag.hasTag(STR_HOST) else DEFAULT_HOST
			port = tag.getInt(STR_PORT) if tag.hasTag(STR_PORT) else self.nextPort(host)
		addr = (host, port)
		if addr in self._servers.values():
			raise Exception("Service '%s' already reroutes to %s:%d!" % \
				(next(x for x in self._servers if self._servers[x] == addr), host, port))
		else:
			self._servers[name] = addr
			return Server.Registered(name, addr)
	
	def reroute(self, tag):
		name = tag.getString(STR_NAME)
		if name in self._servers:
			return server.Communicate(self._servers[name], tag.getCompoundTag(server.STR_DATA))
		else:
			raise Exception("Service '%s' is not registered." % name)
	
	def nextPort(self, host):
		i = self._port
		p = [p for p, h in self._servers.values() if h == host]
		while i in p:
			i += 1
		self._port = i + 1
		return i

	@staticmethod
	def Registered(name, addr):
		host, port = addr
		tag = nbt.TAG_Compound()
		tag.setString(STR_NAME, name)
		tag.setString(STR_HOST, host)
		tag.setInt(STR_PORT, port)
		return tag

