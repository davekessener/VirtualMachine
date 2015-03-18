import threading
import pygame
import time
import socket

class ClientThread(threading.Thead):
	def __init__(self, socket, handler):
		threading.Thread.__init__(self)
		self.socket = socket
		self.handler = handler
		self.start()

	def run(self):
		try:
			msg = self.socket.recv(1)
			name = self.read_name(self.socket)
			self.handle_msg(msg, name, self.socket)
		except Exception as e:
			print('Client interrupted: \'%s\'' % (str(e)))
		self.socket.close()

	def handle_msg(self, msg, name, s):
		{
			'a' : lambda: self.handler.send_status(name, s),
			't' : lambda: self.handler.toggle_status(name, s),
			'x' : lambda: self.handler.quit()
		}.get(msg, lambda: self.handler.error(msg, name))()

	def read_name(self, s):
		l = ord(s.recv(1))
		name = ''
		if l > 0:
			name = s.recv(l)
		return name


class Listener:
	def __init__(self, ip, port, handler):
		self.handler = handler
		self.running = False
		self.clients = []
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket.bind((ip, port))
		self.socket.listen(5)
		try:
			self.start()
		except Exception as e:
			print('Interrupt: \'%s\'' % (str(e)))
		self.socket.close()

	def start(self):
		self.running = True
		while self.running:
			(clientsocket, address) = self.socket.accept()
			self.clients.append(ClientThread(clientsocket, self.handler))
		for client in self.clients:
			client.join()
	
	def quit(self):
		self.running = False


class Alarm:
	def __init__(self, ip, port, fn):
		self.status = ''
		self.sound_threat = SoundThread(fn, self)
		self.listener = Listener(ip, port, self)

	def send_status(self, name, s):
		st = 'y'
		if not self.status:
			st = 'n'
		elif self.status == name:
			st = 'u'
		s.send(st);
	def toggle_status(self, name, s):
	def error(self, msg, name):
	def quit(self):

