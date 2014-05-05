#!/bin/bash

while :; do
    rsync -ruv  /home/ | sed -n 's|^Music/||p' >~/filelist.tmp
	while IFS= read filename
	do
		[ -f "$filename" ] || continue
		# do something with file
		echo "Now processing '$filename'"
		./motion "$filename"
done <~/filelist.tmp
done