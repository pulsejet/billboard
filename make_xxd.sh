#!/bin/bash
mkdir -p src/xxd
cd assets
for file in *
do
    out="../src/xxd/$file.cpp"
    if [[ (! -f $out ) || ($file -nt $out) ]]; then
        echo "Dumping $file"
        xxd -i $file > $out;
    fi
done

echo "xxd dumped!"
