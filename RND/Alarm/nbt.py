import struct

class NBTBase:
	__tids = {}

	def __init__(self, tid, name):
		self._tid = tid
		self._name = name

	def write(self, s):
#		s.send(b'%c%c%s' % (chr(self._tid), chr(len(self._name)), self._name))
		TAG_Num.WriteNumber(s, 'B', self._tid)
		TAG_Num.WriteNumber(s, 'H', len(self._name))
		s.send(self._name)
		self.write_data(s)

	def read(self, s):
		self._name = NBTBase.ReadName(s)
		self.read_data(s)

	def getID(self):
		return self._tid

	def getName(self):
		return self._name

	@staticmethod
	def Read(s, tid = None):
		if tid is None:
			tid = ord(s.recv(1))
		tag = NBTBase.Create(tid)
		tag.read(s)
		return tag

	@staticmethod
	def Create(tid):
		if tid in NBTBase.__tids:
			return NBTBase.__tids[tid]()
		else:
			raise Exception('Invalid NBT-tag ID (%d)!' % (tid))
	
	@staticmethod
	def ReadName(s):
		n = ''
		l = TAG_Num.ReadNumber(s, 'H', 2)
		if l > 0:
			n = s.recv(l)
		return n

	@staticmethod
	def RegisterTag(tid, c):
		if tid in NBTBase.__tids:
			raise Exception('Tag \'%d\' was already registered.' % (tid))
		else:
			NBTBase.__tids[tid] = lambda: c()

def RegisterTags():
	NBTBase.RegisterTag(1, TAG_Byte)
	NBTBase.RegisterTag(2, TAG_Short)
	NBTBase.RegisterTag(3, TAG_Int)
	NBTBase.RegisterTag(4, TAG_Long)
	NBTBase.RegisterTag(5, TAG_Float)
	NBTBase.RegisterTag(6, TAG_Double)
	NBTBase.RegisterTag(7, TAG_Byte_Array)
	NBTBase.RegisterTag(8, TAG_String)
	NBTBase.RegisterTag(9, TAG_List)
	NBTBase.RegisterTag(10, TAG_Compound)
	NBTBase.RegisterTag(11, TAG_Int_Array)

class TAG_Num(NBTBase):
	def __init__(self, tid, form, value, name):
		NBTBase.__init__(self, tid, name)
		self._value = value
		self._form = form
		self._width = TAG_Num.GetWidth(form)

	def read_data(self, s):
		self._value = TAG_Num.ReadNumber(s, self._form, self._width)

	def write_data(self, s):
		TAG_Num.WriteNumber(s, self._form, self.get())

	def get(self):
		return self._value

	@staticmethod
	def ReadNumber(s, f, w = 0):
		if w == 0:
			w = TAG_Num.GetWidth(f)
		r = s.recv(w)
		if len(r) != w:
			raise Exception('String \'%s\' should have length %d!' % (r, w))
		return struct.unpack(f, r)

	@staticmethod
	def WriteNumber(s, f, v):
		s.send(struct.pack(f, v))

	@staticmethod
	def ClipInteger(v, w = 1):
		v = int(v)
		w *= 8
		if v < 0:
			v %= -(1 << (w - 1))
		return v % (1 << w)
	
	@staticmethod
	def GetWidth(f):
		return len(struct.pack(f, 0))

class TAG_Integer(TAG_Num):
	def get(self):
		return TAG_Num.ClipInteger(self._value, self._width)

class TAG_Byte(TAG_Integer):
	def __init__(self, value = 0, name = ''):
		TAG_Integer.__init__(self, 1, 'B', value, name)

class TAG_Short(TAG_Integer):
	def __init__(self, value = 0, name = ''):
		TAG_Integer.__init__(self, 2, 'H', value, name)

class TAG_Int(TAG_Integer):
	def __init__(self, value = 0, name = ''):
		TAG_Integer.__init__(self, 3, 'I', value, name)

class TAG_Long(TAG_Integer):
	def __init__(self, value = 0L, name = ''):
		TAG_Integer.__init__(self, 4, 'L', value, name)

class TAG_Float(TAG_Num):
	def __init__(self, value = 0.0, name = ''):
		TAG_Num.__init__(self, 5, 'f', value, name)

class TAG_Double(TAG_Num):
	def __init__(self, value = 0.0, name = ''):
		TAG_Num.__init__(self, 6, 'd', value, name)

class TAG_Num_Array(NBTBase):
	def __init__(self, tid, form, value, name):
		NBTBase.__init__(self, tid, name)
		self._value = value
		self._form = form
		self._width = TAG_Num.GetWidth(form)

	def read_data(self, s):
		l = TAG_Num.ReadNumber(s, 'I', 4)
		self._value = []
		for i in range(0, l):
			self._value.append(TAG_Num.ReadNumber(s, self._form, self._width))

	def write_data(self, s):
		l = TAG_Num.ClipInteger(len(self._value), 4)
		TAG_Num.WriteNumber(s, 'I', l)
		for i in range(0, l):
			TAG_Num.WriteNumber(s, self._form, self._value[i])

	def get(self):
		return self._value

	def len(self):
		return len(self._value)

class TAG_Byte_Array(TAG_Num_Array):
	def __init__(self, value = [], name = ''):
		TAG_Num_Array.__init__(self, 7, 'B', value, name)

class TAG_String(TAG_Num_Array):
	def __init__(self, value = '', name = ''):
		TAG_Num_Array.__init__(self, 8, 'B', value, name)

	def get(self):
		if not type(self._value) is str:
			self._value = ''.join(chr(c) for c in self._value)
		return self._value

class TAG_List(NBTBase):
	def __init__(self, name = ''):
		NBTBase._init__(self, 9, name)
		self._eid = 0
		self._value = []

	def add_tag(self, tag):
		if self._eid == 0:
			self._eid = tag.getID()
		elif self._eid != tag.getID():
			raise Exception('Error: Tag is of invalid type: %d -> %d' % (tag.getID(), self._eid))
		self._values.append(tag)

	def read_data(self, s):
		self._eid = TAG_Num.ReadNumber(s, 'B', 1)
		l = TAG_Num.ReadNumber(s, 'I', 4)
		self._value = []
		for i in xrange(l):
			tag = NBTBase.Create(self._eid)
			tag.read_data(s)
			self._value.append(tag)

	def write_data(self, s):
		TAG_Num.WriteNumber(s, 'B', self._eid)
		TAG_Num.WriteNumber(s, 'I', len(self._value))
		for tag in self._value:
			tag.write_data(s)

class TAG_Compound(NBTBase):
	def __init__(self, name = ''):
		NBTBase.__init__(self, 10, name)
		self._tags = {}

	def addTag(self, t):
		self._tags[t.getName()] = t

	def read_data(self, s):
		while True:
			tid = TAG_Num.ReadNumber(s, 'B', 1)
			if tid == 0:
				break
			tag = NBTBase.Read(s, tid)
			self._tags[tag.getName()] = tag

	def write_data(self, s):
		for tag in self._tags:
			self._tags[tag].write(s)
		s.send('\x00')

class TAG_Int_Array(TAG_Num_Array):
	def __init__(self, value = [], name = ''):
		TAG_Num_Array.__init__(self, 11, 'I', value, name)

class Buffer:
	def __init__(self):
		self._v = ''

	def send(self, s):
		self._v += s

	def recv(self, l):
		r = self._v[:l]
		self._v = self._v[l:]
		return r

RegisterTags()

i = TAG_Int(42, 'anint')
d = TAG_Double(3.141, 'adouble')
t = TAG_Compound('root')
t.addTag(i)
t.addTag(d)
buf = Buffer()

