#!/bin/bash
for FILE in *.png; do
	convert $FILE -type truecolor $FILE.bmp
	../convert $FILE.bmp
	rm $FILE
	rm $FILE.bmp
done

