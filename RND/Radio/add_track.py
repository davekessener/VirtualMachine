#!/usr/bin/python

import sys
import radio

rc = radio.Client(('daveserver.no-ip.biz', 8888))

if len(sys.argv) == 3:
	_, name, url = sys.argv
	rc.addTrack((name, url))

