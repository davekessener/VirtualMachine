#!/bin/bash
for F in *.txt
do
	if [ -f "$F" ]; then
		if [ `./mail < $F` ]; then
			tail -n +2 "$F" | mail -s "Reminder!" davekessener@gmail.com
		fi
	fi
done

