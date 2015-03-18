import threading
import socket
import time
import nbt

TIME_OUT = 10

def SendPacket(cs, tag):
	tag.write(cs)

def ReceivePacket(cs):
	return nbt.NBTBase.Read(cs)

def Communicate(server, tag):
	with socket.connect(server) as cs:
		SendPacket(cs, tag)
		return ReceivePacket(cs)

class ClientResponder(threading.Thread):
	def __init__(self, cs, acceptor):
		threading.Thread.__init__(self)
		self._acceptor = acceptor
		self._clientsocket = cs
		self.start()
	
	def run(self):
		try:
			tag = ReceivePacket(self._clientsocket)
			self._acceptor.receivePacket(tag, lambda p: self.sendPacket(p))
		except Exception as e:
			print('Client interrupted \'%s\'!' % str(e))
		self._clientsocket.close()

	def sendPacket(self, tag):
		SendPacket(self._clientsocket, tag)

class Listener:
	def __init__(self, port, factory, host = ''):
		self._port = port
		self._host = host
		self._factory = factory
		self._running = False

	def start(self):
		self._running = True
		clients = []
		ssock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			ssock.bind((self._host, self._port))
			ssock.listen(5)
			while self._running:
				(cs, addr) = ssock.accept()
				cs.settimeout(TIME_OUT)
				clients.append(ClientResponder(cs, self._factory(addr, lambda: self.quit())))
		except Exception as e:
			print('Listener interrupted \'%s\'!' % str(e))
		ssock.close()
		for client in clients:
			client.join()

	def quit(self):
		self._running = False

