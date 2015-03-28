#!/usr/bin/python

import radio

rc = radio.RadioClient(('alarmpi.loc', 8888))
c, t, v = rc.getStatus()
if c:
	s, n = c
	c = '%s: %s' % (next(x.name for x in t if x.slot == s), n)
else:
	c = '--stopped--'
rs = '%s\n%s\n%d' % (c, ';'.join(('%d|%s|%s' % (x.slot, x.name, x.url)) for x in t), v)
print(rs)

