import struct

class NBTBase:
	__tids = {}

	def __init__(self, tid, name):
		self._tid = tid
		self._name = name

	def write(self, s):
		NumberTag.WriteNumber(s, 'B', self._tid)
		NumberTag.WriteNumber(s, 'H', len(self._name))
		s.send(self._name[:(len(self._name) % 0x10000)])
		self.write_data(s)

	def read(self, s):
		self._name = NBTBase.ReadName(s)
		self.read_data(s)

	def getID(self):
		return self._tid

	def getName(self):
		return self._name

	def setName(self, s):
		self_.name = s

	@staticmethod
	def Read(s, tid = None):
		if tid is None:
			tid = NumberTag.ReadNumber(s, 'B')
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
		l = NumberTag.ReadNumber(s, 'H')
		return '' if l <= 0 else s.recv(l)

	@staticmethod
	def RegisterTag(tid, c):
		if tid in NBTBase.__tids:
			raise Exception('Tag \'%d\' was already registered.' % (tid))
		else:
			NBTBase.__tids[tid] = lambda: c()

# ------------------------------------------------------------------------------

class NumberTag(NBTBase):
	def __init__(self, tid, form, value, name):
		NBTBase.__init__(self, tid, name)
		self._value = value
		self._form = form

	def read_data(self, s):
		self.set(NumberTag.ReadNumber(s, self._form))

	def write_data(self, s):
		NumberTag.WriteNumber(s, self._form, self.get())

	def get(self):
		return self._value

	def set(self, v):
		self._value = v

	@staticmethod
	def ReadNumber(s, f):
		w = NumberTag.GetWidth(f)
		r = s.recv(w)
		if len(r) != w:
			raise Exception('String \'%s\' should have length %d!' % (r, w))
		return struct.unpack(f, r[::-1])[0]

	@staticmethod
	def WriteNumber(s, f, v):
		s.send(struct.pack(f, v)[::-1])

	@staticmethod
	def ClipInteger(v, w = 1):
		v = int(v)
		w *= 8
		if v < 0:
			v %= -(1 << (w - 1))
		return v % (1 << w)
	
	@staticmethod
	def GetWidth(f):
		return struct.calcsize(f)

# ------------------------------------------------------------------------------

class IntegerTag(NumberTag):
	def get(self):
		return NumberTag.ClipInteger(self._value, self._width)

# ------------------------------------------------------------------------------

class NumberTag_Array(NBTBase):
	def __init__(self, tid, form, value, name):
		NBTBase.__init__(self, tid, name)
		self._value = value
		self._form = form

	def read_data(self, s):
		l = NumberTag.ReadNumber(s, 'I')
		self._value = []
		for i in range(0, l):
			self._value.append(NumberTag.ReadNumber(s, self._form))

	def write_data(self, s):
		l = NumberTag.ClipInteger(len(self._value), 4)
		NumberTag.WriteNumber(s, 'I', l)
		for i in range(0, l):
			NumberTag.WriteNumber(s, self._form, self._value[i])

	def get(self):
		return self._value

	def len(self):
		return len(self._value)

	def size(self):
		return self.len()

# ==============================================================================

class TAG_Byte(IntegerTag):
	def __init__(self, value = 0, name = ''):
		IntegerTag.__init__(self, 1, 'B', value, name)

# ------------------------------------------------------------------------------

class TAG_Short(IntegerTag):
	def __init__(self, value = 0, name = ''):
		IntegerTag.__init__(self, 2, 'H', value, name)

# ------------------------------------------------------------------------------

class TAG_Int(IntegerTag):
	def __init__(self, value = 0, name = ''):
		IntegerTag.__init__(self, 3, 'I', value, name)

# ------------------------------------------------------------------------------

class TAG_Long(IntegerTag):
	def __init__(self, value = 0L, name = ''):
		IntegerTag.__init__(self, 4, 'L', value, name)

# ------------------------------------------------------------------------------

class TAG_Float(NumberTag):
	def __init__(self, value = 0.0, name = ''):
		NumberTag.__init__(self, 5, 'f', value, name)

# ------------------------------------------------------------------------------

class TAG_Double(NumberTag):
	def __init__(self, value = 0.0, name = ''):
		NumberTag.__init__(self, 6, 'd', value, name)

# ------------------------------------------------------------------------------

class TAG_Byte_Array(NumberTag_Array):
	def __init__(self, value = [], name = ''):
		NumberTag_Array.__init__(self, 7, 'B', value, name)

# ------------------------------------------------------------------------------

class TAG_String(NBTBase):
	def __init__(self, value = '', name = ''):
		NBTBase.__init__(self, 8, name)
		self._value = value

	def read_data(self, s):
		l = NumberTag.ReadNumber(s, 'H')
		self._value = s.recv(l)

	def write_data(self, s):
		l = NumberTag.ClipInteger(len(self._value), 2)
		NumberTag.WriteNumber(s, 'H', l)
		s.send(self._value)

	def get(self):
		return self._value

	def set(self, s):
		self._value = s

# ------------------------------------------------------------------------------

class TAG_List(NBTBase):
	def __init__(self, name = ''):
		NBTBase.__init__(self, 9, name)
		self._eid = 0
		self._value = []

	def addTag(self, tag):
		if self._eid == 0:
			self._eid = tag.getID()
		elif self._eid != tag.getID():
			raise Exception('Error: Tag is of invalid type: %d -> %d' % (tag.getID(), self._eid))
		self._values.append(tag)

	def read_data(self, s):
		self._eid = NumberTag.ReadNumber(s, 'B')
		l = NumberTag.ReadNumber(s, 'I')
		self._value = []
		for i in xrange(l):
			tag = NBTBase.Create(self._eid)
			tag.read_data(s)
			self._value.append(tag)

	def write_data(self, s):
		NumberTag.WriteNumber(s, 'B', self._eid)
		NumberTag.WriteNumber(s, 'I', len(self._value))
		for tag in self._value:
			tag.write_data(s)

# ------------------------------------------------------------------------------

class TAG_Compound(NBTBase):
	def __init__(self, name = ''):
		NBTBase.__init__(self, 10, name)
		self._tags = {}
		self._order = []

	def addTag(self, t):
		self._tags[t.getName()] = t
		self._order.append(t.getName())

	def read_data(self, s):
		self._tags = {}
		self._order = []
		while True:
			tid = NumberTag.ReadNumber(s, 'B')
			if tid == 0:
				break
			self.addTag(NBTBase.Read(s, tid))

	def write_data(self, s):
		for tag in self._order:
			self._tags[tag].write(s)
		s.send('\x00')

	def hasTag(self, name):
		return name in self._order

	def getTag(self, name):
		return self_.tags[name]
	
	def setByte(self, name, v):
		addTag(TAG_Byte(value = v, name = name))

	def getByte(self, name):
		return self_.tags[name].get()
	
	def setShort(self, name, v):
		addTag(TAG_Short(value = v, name = name))
	
	def getShort(self, name):
		return self_.tags[name].get()
	
	def setInt(self, name, v):
		addTag(TAG_Int(value = v, name = name))
	
	def getInt(self, name):
		return self_.tags[name].get()
	
	def setLong(self, name, v):
		addTag(TAG_Long(value = v, name = name))
	
	def getLong(self, name):
		return self_.tags[name].get()
	
	def setFloat(self, name, v):
		addTag(TAG_Float(value = v, name = name))
	
	def getFloat(self, name):
		return self_.tags[name].get()
	
	def setDouble(self, name, v):
		addTag(TAG_Double(value = v, name = name))
	
	def getDouble(self, name):
		return self_.tags[name].get()
	
	def setString(self, name, v):
		addTag(TAG_String(value = v, name = name))

	def getString(self, name):
		return self_.tags[name].get()

	def setTagList(self, name, tl):
		tl.setName(name)
		addTag(tl)

	def getTagList(self, name):
		return self._tags[name]

	def setCompoundTag(self, name, tag):
		tag.setName(name)
		addTag(tag)

	def getCompoundTag(self, name):
		return self._tags[name]
	
# ------------------------------------------------------------------------------

class TAG_Int_Array(NumberTag_Array):
	def __init__(self, value = [], name = ''):
		NumberTag_Array.__init__(self, 11, 'I', value, name)

# ------------------------------------------------------------------------------

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

RegisterTags()

# ==============================================================================

class StringBuffer:
	def __init__(self, s = ''):
		self._v = s

	def send(self, s):
		self._v += s

	def recv(self, l):
		r = self._v[:l]
		self._v = self._v[l:]
		return r

class FileBuffer:
	def __init__(self, f):
		self._f = f

	def send(self, s):
		self._f.write(s)

	def recv(self, l):
		return self._f.read(l)

