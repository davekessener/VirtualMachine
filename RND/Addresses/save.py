#!/usr/bin/python

import ddb
import sys

try:
	ddb.Put(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7])
	print('Saved successfully!')
except Exception as e:
	print('ERR: %s' % str(e))

