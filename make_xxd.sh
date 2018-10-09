#!/bin/bash
mkdir -p src/xxd
cd assets
for f in *
do
   echo "Dumping $f"
   xxd -i $f > ../src/xxd/$f.cpp
done
