import nbt
import server

FN = "data.nbt"
EXPORT_FILE = "contacts.txt"
HOST = 'localhost'
PORT = 8448

STR_ACT_UPDATE = "update"
STR_ACT_QUERY = "query"
STR_ACT_EXPORT = "export"

STR_ADR = "Kontakte"
STR_NAME = "Vorname"
STR_SURNAME = "Nachname"
STR_EMAIL = "Email"
STR_CELL = "Handy"
STR_STREET = "Strasse"
STR_ZIP = "PLZ"
STR_CITY = "Stadt"
STR_EXPORT = "export"

VERSION = '1.0'

STR_FIELDS = [STR_NAME, STR_SURNAME, STR_EMAIL, STR_CELL, STR_STREET, STR_ZIP, STR_CITY]

# ==============================================================================

def Run(host = HOST, port = PORT):
	return Server((host, port))

# ==============================================================================

def Get(name, surname):
	c = Client((HOST, PORT))
	return c.get(name, surname)

def Put(name, surname, email, cell, street, plz, city):
	c = Client((HOST, PORT))
	c.set(name, surname, email, cell, street, plz, city)

def Export():
	c = Client((HOST, PORT))
	return c.export()

# ==============================================================================

class Server:
	def __init__(self, addr):
		self._vr = server.VersionedResponder(self)
		self._vr.addVersion((VERSION, self))
		self._listener = server.Listener(self, addr)
		self._db = self.load()
		self._listener.start()

	def __call__(self, addr, tag, responder):
		try:
			t = nbt.TAG_Compound()
			act = tag.getString(server.STR_ACTION)
			data = tag.getCompoundTag(server.STR_DATA) if tag.hasTag(server.STR_DATA) else None
			t.setCompoundTag(server.STR_DATA, {
				STR_ACT_UPDATE: lambda: self.update(data),
				STR_ACT_QUERY:  lambda: self.get(data),
				STR_ACT_EXPORT: lambda: self.export()
			}.get(act, lambda: server.UnknownRequest(act))())
			responder.sendPacket(t)
		except Exception as e:
			responder.sendPacket(server.InvalidTag(e))

	def execute(self, cmd):
		{
			'quit'  : lambda: self._listener.quit(),
			'save'  : lambda: self.save(),
			'load'  : lambda: self.load(),
			'clear' : lambda: self.clear()
		}.get(cmd.lower(), lambda: sys.stdout.write('ERR: Unknown command!\n'))()

	def update(self, tag):
		entry = self.find(tag.getString(STR_NAME), tag.getString(STR_SURNAME))
		if not entry:
			entry = nbt.TAG_Compound()
			self._db.addTag(entry)
		for k in STR_FIELDS:
			if not entry.hasTag(k) or tag.getString(k):
				entry.setString(k, tag.getString(k))
		self.save()
		return tag

	def get(self, tag):
		entry = self.find(tag.getString(STR_NAME), tag.getString(STR_SURNAME))
		return entry if entry else server.SSTag(STR_ERR_UNKNOWN)

	def export(self):
		with open(EXPORT_FILE, "w") as f:
			l = [0 for i in xrange(len(STR_FIELDS))]
			e = []
			for t in self._db:
				c = [t.getString(s) for s in STR_FIELDS]
				for i in xrange(len(l)):
					if l[i] < len(c[i]):
						l[i] = len(c[i])
				e.append(c)
			for c in e:
				for i in xrange(len(l)):
					f.write(c[i].rjust(l[i] + 4))
				f.write('\n')
		tag = nbt.TAG_Compound()
		tag.setString(STR_EXPORT, EXPORT_FILE)
		return tag

	def find(self, name, surname):
		for t in self._db:
			if t.getString(STR_NAME).lower() == name.lower() and t.getString(STR_SURNAME).lower() == surname.lower():
				return t
		return None

	def save(self):
		tag = nbt.TAG_Compound()
		tag.setTagList(STR_ADR, self._db)
		nbt.WriteToFile(FN, tag)

	def load(self):
		try:
			tag = nbt.ReadFromFile(FN)
			self._db = tag.getTagList(STR_ADR)
			return self._db
		except Exception:
			return nbt.TAG_List()

	def clear(self):
		self._db = nbt.TAG_List()
		save()

# ==============================================================================

class Client:
	def __init__(self, addr):
		self.client = server.VersionedClient(addr, VERSION)

	def set(self, name, surname, email, cell, street, plz, city):
		data = nbt.TAG_Compound()
		for k, v in [(STR_NAME, name), (STR_SURNAME, surname), (STR_EMAIL, email), (STR_CELL, cell), (STR_STREET, street), (STR_ZIP, plz), (STR_CITY, city)]:
			data.setString(k, v)
		v, r = self.client.communicate(STR_ACT_UPDATE, data)

	def get(self, name, surname):
		data = nbt.TAG_Compound()
		data.setString(STR_NAME, name)
		data.setString(STR_SURNAME, surname)
		v, r = self.client.communicate(STR_ACT_QUERY, data)
		return (r.getString(s) for s in STR_FIELDS)

	def export(self):
		v, r = self.client.communicate(STR_ACT_EXPORT)
		return r.getString(STR_EXPORT)

