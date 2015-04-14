#!/usr/bin/python

import radio
import sys

rc = radio.Client(('daveserver.no-ip.biz', 8888))
rc.setVolume(sys.argv[1])
print("Changed volume to '%s'" % sys.argv[1])

