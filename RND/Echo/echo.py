import server
import nbt
import distribute

STR_ACT_ECHO = "echo"
STR_ERROR = "ERR! Only Strings and compound-tags may be echoed"
STR_MSG = "message"

STR_ECHOSERVER = "echo"
VERSION = "1.0"

class Server:
	def __init__(self, addr):
		self._vr = server.VersionedResponder(self)
		self._vr.addVersion((VERSION, self))
		self._listener = distribute.Listener(self._vr, STR_ECHOSERVER, addr)
		self._listener.start()

	def __call__(self, addr, tag, responder):
		t = nbt.TAG_Compound()
		t.setCompoundTag(server.STR_DATA, tag.getCompoundTag(server.STR_DATA))
		responder.sendPacket(t)

	def execute(self, cmd):
		{
			'quit' : lambda: self._listener.quit()
		}.get(cmd.lower(), lambda: sys.stdout.write('ERR: Unknown command!\n'))()

class Client:
	def __init__(self, addr):
		self.client = server.VersionedClient(addr, VERSION, distribute.MakeRerouter(STR_ECHOSERVER))

	def echo(self, msg):
		data = nbt.TAG_Compound()
		if type(msg) is nbt.TAG_Compound:
			data = msg
		elif type(msg) is str:
			data.setString(STR_MSG, msg)
		else:
			data.setString(server.STR_ERRMSG, STR_ERROR)
		v, r = self.client.communicate(STR_ACT_ECHO, data)
		return r

