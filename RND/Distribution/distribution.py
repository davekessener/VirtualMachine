import server
import socket

STR_ACT_ADD = "add"
STR_ACT_REROUTE = "reroute"
STR_NAME = "name"
STR_PORT = "port"
STR_HOST = "host"
DEFAULT_HOST = "localhost"
DEFAULT_PORT = 8000

def Reroute(tag, name, addr):
	t = nbt.TAG_Compound()
	t.setCompoundTag(server.STR_DATA, tag)
	t.setString(server.STR_ACTION, STR_REROUTE)
	t.setString(STR_NAME, name)
	return server.Communicate(addr, tag)

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

def Register(name, addr):
	host, port = addr
	tag = server.SSTag(STR_ACT_ADD)
	payload = nbt.TAG_Compound()
	payload.setString(STR_NAME, name)
	if host != 'localhost':
		tag.setString(STR_HOST, GetIP())
	tag.setCompoundTag(server.STR_DATA, payload)
	r = server.Communicate(addr, tag)
	return (r.getString(STR_HOST), r.getInt(STR_PORT))

class DistributedListener:
	def __init__(self, factory, name, addr):
		self._addr = Register(name, addr)
		self._listener = Listener(factory, 

# ==============================================================================

class DistributingServer:
	def __init__(self, addr):
		self._port = DEFAULT_PORT
		self._listener = server.Listener(self, addr)
		self._listener.start()

	def __call__(self, addr, tag, responder):
		try:
			t = nbt.TAG_Compound()
			t.setCompoundTag(server.STR_DATA, {
				STR_ACT_ADD : lambda: self.addServer(t.getTagCompound(server.STR_DATA)),
				STR_ACT_REROUTE : lambda: self.reroute(t)
			}.get(tag.getString(server.STR_ACTION), lambda: server.UnknownRequest())())
			responder.sendPacket(t)
		except Exception as e:
			responder.sendPacket(server.InvalidTag(e))

	def execute(self, cmd):
		{
			"quit" : lambda: self._listener.quit()
		}.get(cmd, lambda: print("Unknown command!"))()
	
	def addServer(self, tag):
		name = tag.getString(STR_NAME)
		host = tag.getString(STR_HOST) if tag.hasTag(STR_HOST) else DEFAULT_HOST
		port = tag.getInt(STR_PORT) if tag.hasTag(STR_PORT) else self.nextPort(host)
		addr = (host, port)
		if name in self._servers:
			if addr != self._servers[name]:
				raise Exception("A service already has that name!")
		elif addr in self._servers.values():
			raise Exception("Service '%s' already reroutes to %s:%d!" % \
				(next(x for x in self._servers if self._servers[x] == addr), host, port))
		else:
			self._servers[name] = addr
			return DistributingServer.Registered(name, addr)
	
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
		t = nbt.TAG_Compound()
		tag = nbt.TAG_Compound()
		tag.setString(STR_NAME, name)
		tag.setString(STR_HOST, host)
		tag.setInt(STR_PORT, port)
		t.setCompoundTag(server.STR_DATA, tag)
		return t

