#!/usr/bin/python

import radio

rc = radio.RadioClient(('localhost', 8888))
c, t, v = rc.getStatus()
rs = '%s\n%s\n%d' % (c, ';'.join(('%d|%s|%s' % (x.slot, x.name, x.url)) for x in t), v)
print(rs)

