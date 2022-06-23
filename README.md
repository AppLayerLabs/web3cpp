# web3cpp

Custom implementation of [Web3](https://web3js.readthedocs.io) in C++.

This is ***NOT*** a 100% parity conversion of the web3js library, but rather a mix of web3js structure with some logic from the now deprecated [aleth](https://github.com/ethereum/aleth) library from Ethereum.

See [Differences from web3js](#differences-from-web3js) for more details on how this project deviates from web3js.

## Compiling

### Dependencies

* **CMake 3.19.0** or higher
* **GCC** with support for **C++17** or higher
* **Boost**, **libhidapi** and **OpenSSL**
* (optional) **Doxygen** for generating the docs

### Instructions

* Clone the project: `git clone https://github.com/avme/web3cpp`
* Go to the project's root folder, create a "build" folder and change to it: `cd web3cpp && mkdir build && cd build`
* Run `cmake ..` then `cmake --build . -- -j$(nproc)` inside the build folder
* If you want documentation, go back to the project's root folder and run `doxygen Doxyfile` from there
  * Docs will be generated at a folder named `docs/html`

### CMake parameters

For use with `cmake -D<param> ..`:

* `BUILD_STATIC` (default **ON**) - compiles the library as static
* `BUILD_TESTS` (default **ON**) - compiles an extra program that runs some tests on the library

## Differences from web3js

Due to architectural differences between JS and C++, most things were significantly changed. It's recommended to generate and read the docs from this project (see [Instructions](#instructions) instead of following other implementations.

* `web3.utils` becomes `namespace Utils`
  * e.g. **web3.utils.sha3("Hello!%")** -> **Utils::sha3("Hello!%")**
* `web3.eth.Contract` becomes `class Contract`
  * e.g. **var c = new web3.eth.Contract(...)** -> **Contract c(...)**
* `web3.eth.abi` becomes `namespace Solidity`
  * e.g. **web3.eth.abi.encodeParameters(['uint256','string'], ['2345675643', 'Hello!%'])** -> **Solidity::packMulti({{{"t", "uint256"}, {"v", "2345675643"}}, {{"t", "string"}, {"v", "Hello!%"}}}, err)**
* The "network" part of `web3.*.net` is technically replaced by `namespace Net` (which does HTTP requests) and `namespace RPC` (which builds the data that is sent through the requests)
  * They're meant to be used together (e.g. **Net::HTTPRequest(provider, Net::RequestTypes::POST, RPC::eth_getBlockNumber().dump())**)
* The "node" part of `web3.*.net` is replaced by `class Provider`
* Parts of `web3.eth.accounts` and `web3.eth.personal` are joined in a custom `Wallet` class
* `web3.eth` becomes `class Eth` and almost all of its member variables were moved to `Provider` and/or `Contract::Options`
* You still access `Provider`, `Wallet` and `Eth` from inside the Web3 class
  * e.g. **web3->getProvider()->getChainId()**, **web3->wallet.getAccounts()**, **web3->eth.getGasPrice()**
* JavaScript's `BN.js` object is replaced by a simple unsigned 256-bit integer (aleth's `dev::u256`), which is aliased to `BigNumber`
* JSON objects are handled using [nlohmann::json](https://github.com/nlohmann/json)

### Missing modules and functionalities

Some particular things are not (and probably won't be) implemented as we either don't have a need for them (at least for now) or other parts of the library are already doing their job.

* [web3.bzz (Swarm Protocol)](https://web3js.readthedocs.io/en/v1.7.4/web3-bzz.html)
* [web3.shh (Whisper Protocol)](https://web3js.readthedocs.io/en/v1.7.4/web3-shh.html)
* [web3.eth.ens (Ethereum Name Service)](https://web3js.readthedocs.io/en/v1.7.4/web3-eth-ens.html)
* [web3.eth.Iban (IBAN/BBAN support)](https://web3js.readthedocs.io/en/v1.7.4/web3-eth-iban.html)
* [web3.eth.subscribe](https://web3js.readthedocs.io/en/v1.7.4/web3-eth-subscribe.html)
  * This one will probably be implemented inside `Contract` but it's not set in stone
* Bloom filters on [web3.utils](https://web3js.readthedocs.io/en/v1.7.4/web3-utils.html#bloom-filters)
* [BatchRequest](https://web3js.readthedocs.io/en/v1.7.4/web3.html#batchrequest)
* [PromiEvent](https://web3js.readthedocs.io/en/v1.7.4/callbacks-promises-events.html)
* [Access lists](https://web3js.readthedocs.io/en/v1.7.4/web3-eth.html#createaccesslist)
* [Extending modules](https://web3js.readthedocs.io/en/v1.7.4/web3.html#extend)
* Other individual functions across some modules ("if it's not in the docs, we're not missing it")

