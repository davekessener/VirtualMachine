import server
import nbt
import distribute

STR_ACT_STORE = "store"
STR_ACT_RETRIEVE = "retrieve"
STR_ID = "id"
STR_DATA = "data"
STR_STATUS = "status"
STR_DB_ENTRIES = "entries"

DB_NEW = 0
DB_UPDATE = 1

DB_PRETTY = {
	DB_NEW : "new",
	DB_UPDATE : "updated"
}

DB_STR_UNKNOWN = "unknown"

STR_POOLSERVER = "datapool"
VERSION = "1.0"

DB_FILE = "database"

class DataBase:
	def __init__(fn):
		self._store = {}
		self._filename = fn
		try:
			root = nbt.ReadFromFile(fn)
			for t in root.getTagList(STR_DB_ENTRIES):
				self._store[t.getString(STR_ID)] = t.getCompoundTag(STR_DATA)
		except Exception:
			pass

	def save(self):
		root = nbt.TAG_Compound()
		entries = nbt.TAG_List()
		for sid in self._store:
			t = nbt.TAG_Compound()
			t.setString(STR_ID, sid)
			t.setCompoundTag(STR_DATA, self._store[sid])
			entries.addTag(t)
		root.setTagList(STR_DB_ENTRIES, entries)
		nbt.WriteToFile(root, self._filename)

	def store(self, sid, tag):
		status = DB_UPDATE if sid in self._store.keys() else DB_NEW
		self._store[sid] = tag
		self.save()
		return status

	def retrieve(self, sid):
		return self._store[sid]

	@staticmethod
	def Pretty(code):
		return DB_PRETTY.get(code, DB_STR_UNKNOWN)

class Server:
	def __init__(self, addr):
		self._vr = server.VersionedResponder(self)
		self._vr.addVersion((VERSION, self))
		self._listener = distribute.Listener(self._vr, STR_POOLSERVER, addr)
		self._db = DataBase(DB_FILE)
		self._listener.start()

	def __call__(self, addr, tag, responder):
		try:
			t = nbt.TAG_Compound()
			act = tag.getString(server.STR_ACTION)
			t.setCompoundTag(server.STR_DATA, {
				STR_ACT_STORE : lambda: self.store(tag.getCompoundTag(server.STR_DATA)),
				STR_ACT_RETRIEVE : lambda: self.retrieve(tag.getCompoundTag(server.STR_DATA))
			}.get(act, lambda: server.UnknownRequest(act))())
			responder.sendPacket(t)
		except Exception as e:
			responder.sendPacket(server.InvalidTag(e))

	def execute(self, cmd):
		{
			'quit' : lambda: self._listener.quit()
		}.get(cmd.lower(), lambda: sys.stdout.write('ERR: Unknown command!\n'))()

	def store(self, tag):
		sid = tag.getString(STR_ID)
		data = tag.getCompoundTag(STR_DATA)
		rc = self._db.store(sid, data)
		t = nbt.TAG_Compound()
		t.setString(STR_STATUS, DataBase.Pretty(rc))
		return t

	def retrieve(self, tag):
		return self._db.retrieve(tag.getString(STR_ID))

class Client:
	def __init__(self, addr):
		self.client = server.VersionedClient(addr, VERSION, distribute.MakeRerouter(STR_POOLSERVER))

	def store(self, sid, data):
		tag = nbt.TAG_Compound()
		tag.setString(STR_ID, sid)
		tag.setCompoundTag(STR_DATA, data)
		v, r = self.client.communicate(STR_ACT_STORE, tag)
		return r.getCompoundTag(server.STR_DATA).getString(STR_STATUS)

	def retrieve(self, sid):
		tag = nbt.TAG_Compound()
		tag.setString(STR_ID, sid)
		v, r = self.client.communicate(STR_ACT_RETRIEVE, tag)
		return r.getCompoundTag(server.STR_DATA)

