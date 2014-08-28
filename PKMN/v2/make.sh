#!/bin/bash
mv resource/*.log resource/logs/ 2> /dev/null
generate-makefile > Mk
make --no-print-directory -f Mk
rm *.log

