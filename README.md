# InstiApp TV
Noticeboard application for InstiApp in SFML

## Dependencies
The only two external dependencies are SFML (2.5+) and CURL, with OpenSSL for HTTPS requests.

## Setup
First, create hexdumps for all assets by running `make_xxd.sh` in a Unix like environment. You need to do this every time you change anything in the `assets` folder.

To compile, install SFML and CURL developmental files by running (for a distribution supporting `apt`) `sudo apt install libcurl4-openssl-dev libsfml-dev` and run `make` to compile. You also need to download include files for `nlohmann/json` and put them in the `include` folder.

## Windows
You need `mingw-w64` with POSIX thread support to compile under Windows. To compile, download SFML, CURL and OpenSSL binaries. Keep static binaries in the `lib` folder, include files in `include` and run `mingw32-make` to compile.

## Configuration
See [config.json](config.json) for a sample configuration. For documentation, check [src/config.cpp](src/config.cpp).

## License
All code is licensed under the Apache License v2.
