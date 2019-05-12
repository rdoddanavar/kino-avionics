#!/usr/bin/env bash
# update_index.sh

file="/home/${USER}/Git/hpr-hardware/README.md"
blk='```'
lvl=3 # No. lvl in tree

# Delete old index 
sed "1,/$blk/!d" $file > temp
mv temp $file

# Generate new index, append to file
tree -L $lvl --dirsfirst >> $file
echo "$blk" >> $file