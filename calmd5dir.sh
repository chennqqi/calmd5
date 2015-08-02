#!/bin/bash

files=`ls $1`

for f in $files
do
	fpath=$1/$f
	fpathnew=${fpath}_md5
	screen ./calmd5 -i ${fpath} -o ${fpathnew} \> ${fpath}time.txt	
done
