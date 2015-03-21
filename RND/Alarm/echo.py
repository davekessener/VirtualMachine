import server

class EchoServer:
	def __init__(self, addr):
		self._vr = server.VersionedResponder()
		self._vr.addVersion(('1.0', self))
		self._listener = server.Listener(self._vr, addr)
		self._listener.start()

	def execute(self, cmd):
		print('Received command \'%s\'!' % cmd)
		if len(cmd) >= 1 and cmd[0] == 'q':
			self._listener.quit()

	def __call__(self, addr, tag, responder):
		responder.sendPacket(tag)
		self._listener.quit()
	
