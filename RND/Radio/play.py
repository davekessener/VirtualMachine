#!/usr/bin/python

import sys
import radio

rc = radio.Client(('daveserver.no-ip.biz', 8888))
if len(sys.argv) > 1:
	rc.playTrack(int(sys.argv[1]))
else:
	rc.stop()

