#!/bin/bash

mv *.log logs/ 2> /dev/null
if [ ! -f "Mk" ]; then
	generate-makefile > Mk
fi
make --no-print-directory -f Mk clean
rm -f *.log

