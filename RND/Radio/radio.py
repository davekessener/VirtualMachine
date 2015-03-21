import nbt
import server

STR_ACTION = 'action'
STR_ERR_INVALID = 'invalid tag'
STR_ERR_UNKNOWN = 'unknown request'

class RadioServer:
	def __init__(self, addr):
		self._listener = server.Listener(self, addr)
		self._listener.start()
	
	def __call__(self, addr):
		return self

	def receivePacket(self, tag, responder):
		if tag.hasString(STR_ACTION):
			responder.sendPacket({
				'status' : lambda: self.getStatus(),
				'add'    : lambda: self.addTrack(tag),
				'play'   : lambda: self.playTrack(tag),
				'stop'   : lambda: self.stop(),
				'volume' : lambda: self.setVolume(tag)
			}.get(tag.getString(STR_ACTION), lambda: RadioServer.UnknownRequest())())
		else:
			responder.sendPacket(RadioServer.InvalidTag())

	def execute(self, cmd):
		{
			'quit' : lambda: self._listener.quit()
		}.get(cmd.lower(), lambda: print('ERR: Unknown command!'))()

	@staticmethod
	def UnknownRequest():
		tag = nbt.TAG_Compound()
		tag.setString(STR_ACTION, STR_ERR_UNKNOWN)
		return tag

	@staticmethod
	def InvalidTag():
		tag = nbt.TAG_Compound()
		tag.setString(STR_ACTION, STR_ERR_INVALID)
		return tag

if __name__ == '__main__':
	radio = RadioServer(('localhost', 8888))

