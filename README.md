# Billboard
Digital billboard for [WnCC](https://github.com/wncc)'s InstiApp in SFML

## Dependencies
The package depends on SFML (2.5+), CURL (with OpenSSL binaries required for HTTPS requests) and `nlohmann::json`.

## Setup
First, create hexdumps for all assets by running `make_xxd.sh` in a Unix like environment. You need to do this every time you change anything in the `assets` folder.

To compile, install SFML and CURL developmental files by running (for a distribution supporting `apt`) `sudo apt install libcurl4-openssl-dev libsfml-dev` and run `make` to compile. You also need to download include files for `nlohmann/json` and put them in the `include` folder.

## Windows
You need `mingw-w64` with POSIX thread support to compile under Windows. To compile, download SFML, CURL and OpenSSL binaries. Keep static binaries in the `lib` folder, include files in `include` and run `mingw32-make` to compile.

## Raspberry Pi
First, you need to compile [mickelson/sfml-pi](https://github.com/mickelson/sfml-pi), and then compile this repo with those dependencies. This can be done by running such a script:

```bash
#!/bin/bash

# Get dependencies
sudo apt-get -y install cmake libflac-dev libogg-dev libvorbis-dev libopenal-dev libjpeg8-dev libfreetype6-dev libudev-dev libraspberrypi-dev libcurl4-openssl-dev

# Clone mickelson/sfml-pi
git clone https://github.com/mickelson/sfml-pi sfmlpi --depth 1
cd sfmlpi
mkdir build
cd build
cmake .. -DSFML_RPI=1 -DEGL_INCLUDE_DIR=/opt/vc/include -DEGL_LIBRARY=/opt/vc/lib/libbrcmEGL.so -DGLES_INCLUDE_DIR=/opt/vc/include -DGLES_LIBRARY=/opt/vc/lib/libbrcmGLESv2.so
sudo make install
sudo ldconfig
cd ../..

# Clone this repo
git clone https://github.com/pulsejet/billboard
cd billboard

# Get include files
wget https://github.com/nlohmann/json/releases/download/v3.4.0/include.zip && unzip -o include.zip && rm include.zip

# Dump assets
./make_xxd.sh

# Produce an optimized build
make CXXFLAGS="-O3 -Wno-psabi"
```

## Configuration
See [assets/config.json](assets/config.json) for the default configuration. You may change one or more values by having the file `config.json` with relevant keys in your working directory when you run billboard. For documentation, check [src/config.hpp](src/config.hpp).

## License
All code is licensed under the Apache License v2.
