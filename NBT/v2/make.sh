#!/bin/bash
mv *.log logs/ 2> /dev/null
generate-makefile > Mk
make --no-print-directory -f Mk
rm -f *.log

