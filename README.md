# web3cpp

Implementation of [Web3](https://web3js.readthedocs.io) in C++.

**NOTE: this is NOT a 100% parity-complete conversion of the web3.js library. Due to the differences in structure between JS and C++, some functions or properties may not exist at all, or may be re-coded with different names.**

## Compiling

### Dependencies

* **CMake 3.19.0** or higher
* **GCC** with support for **C++17** or higher
* **Boost**, **libhidapi** and **OpenSSL**

### Known caveats

* **Debian 11 Bullseye and older**: CMake is too old and has to be installed manually from [their website](https://cmake.org/download)

### Instructions

* Clone the project: `git clone https://github.com/avme/web3cpp`
* Go to the project's root folder, create a "build" folder and change to it:
  * `cd web3cpp && mkdir build && cd build`
* Run `cmake ..` then `cmake --build . -- -j$(nproc)` inside the build folder

### CMake parameters

For use with `cmake -D<param> ..`:

* `BUILD_STATIC` (default **ON**) - compiles the library as static
* `BUILD_TESTS` (default **ON**) - compiles an extra program that tests the library

## Missing modules

The following modules are **not** implemented:

* web3.bzz
* web3.shh
* web3.eth.ens
* web3.eth.Iban
