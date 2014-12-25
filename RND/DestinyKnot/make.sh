#!/bin/bash
mv resource/*.log resource/logs/ 2> /dev/null
rm -f Mk
if [ ! -f do.cfg ]; then
	echo "A do.cfg file is needed!"
else
	if command -v generate-makefile > /dev/null 2>&1; then
		generate-makefile > Mk
	else
		cp do.cfg Mk
		cat man.mk >> Mk
	fi
	make --no-print-directory -f Mk $@
	rm -f *.log
fi

