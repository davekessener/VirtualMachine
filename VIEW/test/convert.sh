#!/bin/bash
for FILE in *.{jpg,jpeg,JPG,png,PNG,bmp,BMP}; do
	if [ -f "./$FILE" ]; then
		echo -n "Converting file '$FILE' -> '"
		convert "./$FILE" -type truecolor "./$FILE.bmp"
		rm "./$FILE"
		../convert -v "./$FILE.bmp"
		rm "./$FILE.bmp"
		echo "' [DONE]"
	fi
done

echo -n "Password: "
../generate *.gz > config.dat

