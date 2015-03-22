import nbt
import server
import re
import subprocess

STR_ACTION = 'action'
STR_ERR_INVALID = 'invalid tag'
STR_ERR_UNKNOWN = 'unknown request'
STR_ACT_STATUS = 'status'
STR_ACT_ADD = 'add'
STR_ACT_PLAY = 'play'
STR_ACT_STOP = 'stop'
STR_ACT_VOLUME = 'volume'
STR_TRACK_NAME = 'name'
STR_TRACK_URL = 'url'
STR_TRACK_SLOT = 'slot'
STR_EMPTY = '--stopped--'
VERSION = '1.0'

def call(args, stdin = None, stdout = None, stderr = None, shell = False):
	return subprocess.check_output(args, stdin=stdin, stdout=stdout, stderr=stderr, shell=shell)

# ==============================================================================

class Track:
	def __init__(self, slot, name, url):
		self.slot = slot
		self.name = name
		self.url = url

	def __cmp__(self, other):
		return self.slot.__cmp__(other.slot)

	def writeToNBT(self, tag):
		tag.setString(STR_TRACK_NAME, self.name)
		tag.setString(STR_TRACK_URL, self.url)
		tag.setInt(STR_TRACK_SLOT, self.slot)

class MPC:
	_PTRN = re.compile('[^:]*:[ \\t]*(.*)\\n')

	def __init__(self):
		self._idx = 0
		self._tracks = []
		self.volume = 100
		self._current = None
		self.stop()

	def getCurrent(self):
		m = MPC._ptrn.match(call(['mpc', 'current']))
		s = STR_EMPTY
		if m:
			s = m.group(1)
			if self._current:
				s = '%s: %s' % (self._current.name, s)
		return s

	def getPlaylist(self):
		return nbt.TAG_List([t.writeToNBT(nbt.TAG_Compound()) for t in self._tracks])

	def addTrack(self, track):
		self._tracks.append(Track(self.nextID(), *track))
		self._tracks.sort()

	def play(self, slot):
		track = next((t for t in self._tracks if t.slot == slot))
		self.stop()
		call(['mpc', 'add', track.url])
		call(['mpc', 'play'])
		self._current = track

	@property
	def volume(self):
		return self.volume

	@volume.setter
	def volume(self, v):
		v = {
				'+' : lambda: self.volume + int(v[1:]),
				'-' : lambda: self.volume - int(v[1:])
			}.get(v[0], lambda: int(v))()
		self._volume = int(min(100, max(0, v)))
		call(['mpc', 'volume', str(self._volume)])

	def stop(self):
		call(['mpc', 'stop'])
		call(['mpc', 'clear'])
		self._current = None

	def nextID(self):
		self._idx += 1
		return self._idx

# ==============================================================================

class RadioServer:
	def __init__(self, addr):
		self._mpc = MPC()
		self._vr = server.VersionedResponder()
		self._vr.addVersion((VERSION, self))
		self._listener = server.Listener(self._vr, addr)
		self._listener.start()
	
	def __call__(self, addr, tag, responder):
		try:
			responder.sendPacket({
				STR_ACT_STATUS : lambda: self.getStatus(),
				STR_ACT_ADD : lambda: self.addTrack(tag),
				STR_ACT_PLAY : lambda: self.playTrack(tag),
				STR_ACT_STOP : lambda: self.stop(),
				STR_ACT_VOLUME : lambda: self.setVolume(tag)
			}.get(tag.getString(STR_ACTION), lambda: RadioServer.UnknownRequest())())
		except Exception as e:
			responder.sendPacket(RadioServer.InvalidTag(e))

	def execute(self, cmd):
		{
			'quit' : lambda: self._listener.quit()
		}.get(cmd.lower(), lambda: print('ERR: Unknown command!'))()

	def getStatus(self):
		t = nbt.TAG_Compound()
		t.setString(STR_MPC_CURRENT, self._mpc.getCurrent())
		t.setTagList(STR_MPC_TRACKS, self._mpc.getPlaylist())
		t.setInt(STR_MPC_VOLUME, self._mpc.volume)
		return t

	def addTrack(self, tag):
		name = tag.getString(STR_MPC_NAME)
		url = tag.getString(STR_MPC_URL)
		return self._mpc.addTrack((name, url))

	def playTrack(self, tag):
		self._mpc.play(tag.getInt(STR_MPC_SLOT))
		return self.getStatus()

	def stop(self):
		self._mpc.stop()
		return RadioServer.Success()

	def setVolume(self, tag):
		self._mpc.volume = tag.getString(STR_MPC_VOLUME)
		return self.getStatus()

	@staticmethod
	def UnknownRequest():
		tag = nbt.TAG_Compound()
		tag.setString(STR_ACTION, STR_ERR_UNKNOWN)
		return tag

	@staticmethod
	def Success():
		tag = nbt.TAG_Compound()
		tag.setString(STR_ACTION, STR_SUCCESS)
		return tag

	@staticmethod
	def InvalidTag(e):
		tag = nbt.TAG_Compound()
		tag.setString(STR_ACTION, STR_ERR_INVALID)
		tag.setString(STR_ERRMSG, str(e))
		return tag

if __name__ == '__main__':
	radio = RadioServer(('localhost', 8888))

