# web3cpp

Implementation of [Web3](https://web3js.readthedocs.io) in C++.

## Compiling

### Dependencies

* **CMake 3.19.0** or higher
* **GCC** with support for **C++17** or higher

### Known caveats

* **Debian 11 Bullseye and older**: CMake is too old and has to be installed manually from [their website](https://cmake.org/download)

### Instructions

* Clone the project: `git clone https://github.com/avme/web3cpp`
* Go to the project's root folder, create a "build" folder and change to it:
  * `cd web3cpp && mkdir build && cd build`
* Run `cmake ..` then `make` inside the build folder
  * Compiling defaults to a static build, use `-DBUILD_STATIC=OFF` to do a shared build

## Missing modules

The following modules are **not** implemented:

* web3.bzz
* web3.shh
* web3.eth.ens
* web3.eth.Iban
