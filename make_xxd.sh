#!/bin/bash
mkdir -p src/xxd
cd assets
for f in *
do
   echo "Dumping $f"
   XXD -i $f > ../src/xxd/$f.cpp
done
