#!/bin/bash


find . -name *.sh.o* | 
while read l; do 
	echo -n "$l ";
	echo $(cat $l);
done >> test.res 
