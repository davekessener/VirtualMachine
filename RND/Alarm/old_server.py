import time
import socket
import pygame
import threading

PORT_NO = 8444

MUSIC_FILE = "shingekiNoClock.mp3"
MUSIC_VOLUME = 0.25
MUSIC_TIMEOUT = 60

running = True
status = ''

class AlarmThread(threading.Thread):
	def __init__(self, fn):
		threading.Thread.__init__(self)
		self.playback = False
		self.timer = 0.0
		self.is_running = False
		self.volume = 0
		pygame.init()
		pygame.mixer.init(frequency=48000)
		pygame.mixer.music.load(fn)

	def reset_clock(self, v = MUSIC_VOLUME):
		self.timer = time.time()
		if v > 1.0:
			v = 1.0
		elif v < 0.05:
			v = 0.05
		self.volume = v;

	def play(self, v = MUSIC_VOLUME):
		self.playback = True
		self.reset_clock(v)
		pygame.mixer.music.set_volume(self.volume)
		pygame.mixer.music.play()
		print('# Playing with volume %.4f' % (self.volume))

	def halt(self):
		self.playback = False
		self.reset_clock()
		pygame.mixer.music.stop()

	def quit(self):
		self.is_running = False

	def run(self):
		self.is_running = True
		while self.is_running:
#			print('# %d seconds passed since %d.' % (time.time() - self.timer, self.timer))
			if self.playback and time.time() - self.timer >= MUSIC_TIMEOUT:
				pygame.mixer.music.stop()
				self.play(self.volume * 2)
			time.sleep(1)
		pygame.mixer.music.stop()

class AlarmServer:
	def __init__(self, port):
		self.th = AlarmThread(MUSIC_FILE)
		self.serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.serversocket.bind(('', port))
		self.serversocket.listen(5)
		self.running = False
		self.status = ''
		print('Listening on port %d' % (PORT_NO))
		try:
			self.listen()
		except Exception as e:
			print('Interrupted.')
			print(str(e))
		self.th.join()
		self.serversocket.close()
		print('Goodbye!')

	def listen(self):
		self.running = True
		self.th.start()
		while self.running:
			print('################################')
#			print('>>> Listening [...]')
			(clientsocket, address) = self.serversocket.accept()
			try:
#				print('Connection established!')
				self.respond(clientsocket.recv(1), clientsocket)
			except Exception as e:
				print('Client interrupted.')
				print(str(e))
				self.running = False
			clientsocket.close()
	
	def respond(self, msg, s):
		{
			'a': lambda: self.status_request(s),
			't': lambda: self.toggle_status(s),
			'x': lambda: self.shutdown()
		}.get(msg, lambda: self.err_msg(msg))()

	def status_request(self, s):
		name = self.read_name(s)
		if not self.status:
			s.send('n')
		elif self.status == name:
			s.send('u')
		else:
			s.send('y')
		print('Received status request from user \'%s\'.' % (name))
	
	def toggle_status(self, s):
		name = self.read_name(s)
		if not self.status:
			print('User \'%s\' activated the alarm.' % (name))
			self.status = name
			self.th.play()
			s.send('O')
		elif self.status == name:
			print('User \'%s\' deactivated the alarm.' % (name))
			self.status = ''
			self.th.halt()
			s.send('o')
		else:
			print('User \'%s\' cannot deactivate the alarm from user \'%s\'!' % (name, self.status))
			s.send('x')
	
	def shutdown(self):
		print('Shutting down [...]')
		self.running = False
		self.th.quit()

	def err_msg(self, msg):
		print('Received invalid request \'%s\'!' % (msg))

	def read_name(self, s):
		l = ord(s.recv(1)[0])
#		print('Reading name of length %d [...]' % (l))
		name = "" if l <= 0 else s.recv(l)
#		print('Name is %s.' % (name))
		return name

AlarmServer(PORT_NO)

