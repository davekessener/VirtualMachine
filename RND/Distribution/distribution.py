import server

STR_ACT_ADD = "add"
STR_ACT_REROUTE = "reroute"
STR_NAME = "name"
STR_PORT = "port"
STR_HOST = "host"
STR_DEFAULT_HOST = "localhost"

def Reroute(tag, name, addr):
	t = nbt.TAG_Compound()
	t.setCompoundTag(server.STR_DATA, tag)
	t.setString(server.STR_ACTION, STR_REROUTE)
	t.setString(STR_NAME, name)
	return server.Communicate(addr, tag)

class DistributingServer:
	def __init__(self, addr):
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
		port = tag.getInt(STR_PORT)
		host = tag.getString(STR_HOST) if tag.hasTag(STR_HOST) else STR_DEFAULT_HOST
		addr = (host, port)
		if name in self._servers:
			if addr != self._servers[name]:
				raise Exception("A service already has that name!")
		elif addr in self._servers.values():
			raise Exception("Service '%s' already reroutes to %s:%d!" % \
				(next(x for x in self._servers if self._servers[x] == addr), host, port))
		else:
			self._servers[name] = addr
			return server.SSTag(STR_SUCCESS)
	
	def reroute(self, tag):
		name = tag.getString(STR_NAME)
		if name in self._servers:
			return server.Communicate(self._servers[name], tag.getCompoundTag(server.STR_DATA))
		else:
			raise Exception("Service '%s' is not registered." % name)

