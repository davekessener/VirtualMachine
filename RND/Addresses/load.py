#!/usr/bin/python

import ddb
import sys

print('\n'.join(ddb.Get(sys.argv[1], sys.argv[2])))
print('DONE')

