import sys
import re
import subprocess

import nbt
import server
import distribute

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

STR_RADIOSERVER = 'radioserver'

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

# ------------------------------------------------------------------------------

class MPD:
	def current(self):
		return call(['mpc', 'current'])

	def play(self, track):
		call(['mpc', 'add', track])
		call(['mpc', 'play'])

	def stop(self):
		call(['mpc', 'stop'])
		call(['mpc', 'clear'])

	def volume(self, v):
		av = 55 + 45 * (float(v) / 100.0)
		try:
			call(['mpc', 'volume', ('%d' % int(av))])
		except Exception as e:
			pass

# ------------------------------------------------------------------------------

class MusicProxy:
	_PTRN = re.compile('[^:]*:[ \\t]*(.*)\\n')

	def __init__(self):
		self._music = MPD()
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
		self._tracks = []
		self._idx = 0
		with open(fn, 'r') as f:
			for l in f.readlines():
				td = l[:-1].split('|')
				slot = int(td[0])
				if slot > self._idx:
					self._idx = slot
				self._tracks.append(Track(slot, td[1], td[2]))
		return self._tracks

	def getCurrent(self):
		tag = nbt.TAG_Compound()
		m = MusicProxy._PTRN.match(self._music.current())
		tag.setInt(STR_TRACK_SLOT, self._current.slot if self._current and m else 0)
		if m:
			tag.setString(STR_TRACK_NAME, m.group(1))
		return tag

	def getPlaylist(self):
		return nbt.TAG_List([t.writeToNBT() for t in self._tracks])

	def addTrack(self, track):
		self._tracks.append(Track(self.nextID(), *track))
		return self._tracks[-1]

	def play(self, slot):
		track = next((t for t in self._tracks if t.slot == slot))
		self.stop()
		self._music.play(track.url)
		self._music.volume(self.volume)
		self._current = track
		return track

	@property
	def volume(self):
		return int(self.volume)

	@volume.setter
	def volume(self, v):
		u = {
				'+' : lambda: self.volume + int(v[1:]),
				'-' : lambda: self.volume - int(v[1:])
			}.get(v[0], lambda: int(v))()
		self.volume = min(100, max(0, int(u)))
		self._music.volume(self.volume)

	def stop(self):
		self._music.stop()
		self._current = None

	def nextID(self):
		self._idx += 1
		return self._idx

# ------------------------------------------------------------------------------

class Server:
	def __init__(self, addr):
		self._mpd = MusicProxy()
		self._vr = server.VersionedResponder(self)
		self._vr.addVersion((VERSION, self))
		self._listener = distribute.Listener(self._vr, STR_RADIOSERVER, addr)
		self._listener.start()
	
	def __call__(self, addr, tag, responder):
		try:
			t = nbt.TAG_Compound()
			act = tag.getString(server.STR_ACTION)
			t.setCompoundTag(server.STR_DATA, {
				STR_ACT_STATUS : lambda: self.getStatus(),
				STR_ACT_ADD : lambda: self.addTrack(tag.getCompoundTag(server.STR_DATA)),
				STR_ACT_PLAY : lambda: self.playTrack(tag.getCompoundTag(server.STR_DATA)),
				STR_ACT_STOP : lambda: self.stop(),
				STR_ACT_VOLUME : lambda: self.setVolume(tag.getCompoundTag(server.STR_DATA))
			}.get(act, lambda: server.UnknownRequest(act))())
			responder.sendPacket(t)
		except Exception as e:
			responder.sendPacket(server.InvalidTag(e))

	def execute(self, cmd):
		{
			'quit' : lambda: self.quit(),
			'save' : lambda: self._mpd.save(SAVE_FILE),
			'load' : lambda: self.load(SAVE_FILE)
		}.get(cmd.lower(), lambda: sys.stdout.write('ERR: Unknown command!\n'))()

	def quit(self):
		self._mpd.stop()
		self._listener.quit()

	def load(self, fn):
		ts = self._mpd.load(fn)
		self._listener.log('Reading from \'%s\' ...' % fn)
		for t in ts:
			self._listener.log('Added @%02d: %20s [%60s]' % (t.slot, t.name, t.url if len(t.url) < 60 else t.url[:56] + ' ...'))

	def getStatus(self):
		t = nbt.TAG_Compound()
		t.setCompoundTag(STR_MPD_CURRENT, self._mpd.getCurrent())
		t.setTagList(STR_MPD_TRACKS, self._mpd.getPlaylist())
		t.setInt(STR_MPD_VOLUME, self._mpd.volume)
		return t

	def addTrack(self, tag):
		name = tag.getString(STR_TRACK_NAME)
		url = tag.getString(STR_TRACK_URL)
		slot = self._mpd.addTrack((name, url)).slot
		self._listener.log('Added station \'%s\' @%d' % (name, slot))
		return self.getStatus()

	def playTrack(self, tag):
		track = self._mpd.play(tag.getInt(STR_TRACK_SLOT))
		self._listener.log('Playing now \'%s\'' % track.name)
		return self.getStatus()

	def stop(self):
		self._mpd.stop()
		self._listener.log('Stopped playback.')
		return self.getStatus()

	def setVolume(self, tag):
		self._mpd.volume = tag.getString(STR_MPD_VOLUME)
		self._listener.log('Volume set to %s' % str(self._mpd.volume))
		return self.getStatus()

# ------------------------------------------------------------------------------

class Client:
	def __init__(self, addr):
		self.client = server.VersionedClient(addr, VERSION, distribute.MakeRerouter(STR_RADIOSERVER))
	
	def getStatus(self):
		v, r = self.client.communicate(STR_ACT_STATUS)
		c = r.getCompoundTag(STR_MPD_CURRENT)
		current = None
		if c.getInt(STR_TRACK_SLOT) > 0:
			current = (c.getInt(STR_TRACK_SLOT), c.getString(STR_TRACK_NAME))
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

	def setVolume(self, v):
		tag = nbt.TAG_Compound()
		tag.setString(STR_MPD_VOLUME, str(v))
		v, r = self.client.communicate(STR_ACT_VOLUME, tag)

# ==============================================================================

if __name__ == '__main__':
	radio = Server(('localhost', 8888))

