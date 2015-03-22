import sys
import nbt
import server
import re
import subprocess

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
STR_MPD_CURRENT = 'playing'
STR_MPD_TRACKS = 'tracks'
STR_MPD_VOLUME = 'volume'
STR_EMPTY = '--stopped--'
STR_SUCCESS = 'success'

VERSION = '1.0'
SAVE_FILE = 'playlist.txt'

def call(args):
	return subprocess.check_output(args, stderr = subprocess.STDOUT)

# ------------------------------------------------------------------------------

class Track:
	def __init__(self, slot = 0, name = '', url = '', tag = None):
		self.slot = slot
		self.name = name
		self.url = url
		if tag:
			self.readFromNBT(tag)

	def __cmp__(self, other):
		return self.slot.__cmp__(other.slot)

	def writeToNBT(self):
		tag = nbt.TAG_Compound()
		tag.setString(STR_TRACK_NAME, self.name)
		tag.setString(STR_TRACK_URL, self.url)
		tag.setInt(STR_TRACK_SLOT, self.slot)
		return tag

	def readFromNBT(self, tag):
		self.name = tag.getString(STR_TRACK_NAME)
		self.url = tag.getString(STR_TRACK_URL)
		self.slot = tag.getInt(STR_TRACK_SLOT)

class MPD:
	_PTRN = re.compile('[^:]*:[ \\t]*(.*)\\n')

	def __init__(self):
		self._idx = 0
		self._tracks = []
		self.volume = 100
		self._current = None
		self.stop()

	def save(self, fn):
		with open(fn, 'w') as f:
			for t in self._tracks:
				f.write('%d|%s|%s\n' % (t.slot, t.name, t.url))

	def load(self, fn):
		with open(fn, 'r') as f:
			for l in f.readlines():
				td = l[:-1].split('|')
				slot = int(td[0])
				if slot > self._idx:
					self._idx = slot
				self._tracks.append(Track(slot, td[1], td[2]))

	def getCurrent(self):
		m = MPD._PTRN.match(call(['mpc', 'current']))
		s = STR_EMPTY
		if m:
			s = m.group(1)
			if self._current:
				s = '%s: %s' % (self._current.name, s)
		return s

	def getPlaylist(self):
		return nbt.TAG_List([t.writeToNBT() for t in self._tracks])

	def addTrack(self, track):
		self._tracks.append(Track(self.nextID(), *track))

	def play(self, slot):
		track = next((t for t in self._tracks if t.slot == slot))
		self.stop()
		call(['mpc', 'add', track.url])
		call(['mpc', 'play'])
#		call(['mpc', 'volume', str(self.volume)])
		self._current = track

	@property
	def volume(self):
		return self.volume

	@volume.setter
	def volume(self, v):
		u = {
				'+' : lambda: self.volume + int(v[1:]),
				'-' : lambda: self.volume - int(v[1:])
			}.get(v[0], lambda: int(v))()
		self.volume = min(100, max(0, u))
#		call(['mpc', 'volume', str(self.volume)])

	def stop(self):
		call(['mpc', 'stop'])
		call(['mpc', 'clear'])
		self._current = None

	def nextID(self):
		self._idx += 1
		return self._idx

# ------------------------------------------------------------------------------

class RadioServer:
	def __init__(self, addr):
		self._mpd = MPD()
		self._vr = server.VersionedResponder(self)
		self._vr.addVersion((VERSION, self))
		self._listener = server.Listener(self._vr, addr)
		self._listener.start()
	
	def __call__(self, addr, tag, responder):
		try:
			t = nbt.TAG_Compound()
			t.setCompoundTag(server.STR_DATA, {
				STR_ACT_STATUS : lambda: self.getStatus(),
				STR_ACT_ADD : lambda: self.addTrack(tag.getCompoundTag(server.STR_DATA)),
				STR_ACT_PLAY : lambda: self.playTrack(tag.getCompoundTag(server.STR_DATA)),
				STR_ACT_STOP : lambda: self.stop(),
				STR_ACT_VOLUME : lambda: self.setVolume(tag.getCompoundTag(server.STR_DATA))
			}.get(tag.getString(server.STR_ACTION), lambda: UnknownRequest())())
			responder.sendPacket(t)
		except Exception as e:
			responder.sendPacket(InvalidTag(e))

	def execute(self, cmd):
		{
			'quit' : lambda: self._listener.quit(),
			'save' : lambda: self._mpd.save(SAVE_FILE),
			'load' : lambda: self._mpd.load(SAVE_FILE)
		}.get(cmd.lower(), lambda: sys.stdout.write('ERR: Unknown command!\n'))()

	def getStatus(self):
		t = nbt.TAG_Compound()
		t.setString(STR_MPD_CURRENT, self._mpd.getCurrent())
		t.setTagList(STR_MPD_TRACKS, self._mpd.getPlaylist())
		t.setInt(STR_MPD_VOLUME, self._mpd.volume)
		return t

	def addTrack(self, tag):
		name = tag.getString(STR_TRACK_NAME)
		url = tag.getString(STR_TRACK_URL)
		self._mpd.addTrack((name, url))
		return self.getStatus()

	def playTrack(self, tag):
		self._mpd.play(tag.getInt(STR_TRACK_SLOT))
		return self.getStatus()

	def stop(self):
		self._mpd.stop()
		return self.getStatus()

	def setVolume(self, tag):
		self._mpd.volume = tag.getString(STR_MPD_VOLUME)
		return self.getStatus()

def UnknownRequest():
	return SSTag(STR_ERR_UNKNOWN)

def InvalidTag(e):
	tag = SSTag(STR_ERR_INVALID)
	tag.setString(server.STR_ERRMSG, str(e))
	return tag

# ------------------------------------------------------------------------------

class RadioClient:
	def __init__(self, addr):
		self.client = server.VersionedClient(addr, VERSION)
	
	def getStatus(self):
		v, r = self.client.communicate(STR_ACT_STATUS)
		current = r.getString(STR_MPD_CURRENT)
		tracks = [Track(tag=t) for t in r.getTagList(STR_MPD_TRACKS)]
		volume = r.getInt(STR_MPD_VOLUME)
		return (current, tracks, volume)

	def addTrack(self, track):
		name, url = track
		tag = nbt.TAG_Compound()
		tag.setString(STR_TRACK_NAME, name)
		tag.setString(STR_TRACK_URL, url)
		v, r = self.client.communicate(STR_ACT_ADD, tag)

	def playTrack(self, slot):
		tag = nbt.TAG_Compound()
		tag.setInt(STR_TRACK_SLOT, slot)
		v, r = self.client.communicate(STR_ACT_PLAY, tag)

	def stop(self):
		v, r = self.client.communicate(STR_ACT_STOP)

# ------------------------------------------------------------------------------

def SSTag(s):
	tag = nbt.TAG_Compound()
	tag.setString(server.STR_ACTION, s)
	return tag

class MPC:
	def __init__(self, addr):
		self.client = server.VersionedClient(addr, VERSION)

	def printStatus(self, s):
		print(s.getString(STR_MPD_CURRENT))
		for t in s.getTagList(STR_MPD_TRACKS):
			track = Track(tag=t)
			print('#%02d: %10s [@\'%s\']' % (track.slot, track.name, track.url))
		print('Volume %d%%' % s.getInt(STR_MPD_VOLUME))

	def getStatus(self):
		v, r = self.client.communicate(STR_ACT_STATUS)
		self.printStatus(r)

	def addTrack(self, track):
		name, url = track
		tag = nbt.TAG_Compound()
		tag.setString(STR_TRACK_NAME, name)
		tag.setString(STR_TRACK_URL, url)
		v, r = self.client.communicate(STR_ACT_ADD, tag)
		self.printStatus(r)

	def play(self, slot):
		tag = nbt.TAG_Compound()
		tag.setInt(STR_TRACK_SLOT, slot)
		v, r = self.client.communicate(STR_ACT_PLAY, tag)
		self.printStatus(r)

	def stop(self):
		v, r = self.client.communicate(STR_ACT_STOP)
		self.printStatus(r)

# ==============================================================================

if __name__ == '__main__':
	radio = RadioServer(('localhost', 8888))

