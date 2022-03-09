#ifndef WEB3_H
#define WEB3_H

#include <string>
#include "Utils.h"
#include "Wallet.h"

#include <nlohmann/json.hpp>

// Main/Umbrella class that houses all modules.

// TODO:
// - Decide how to implement the objects:
//   - modules, utils
// - Understand why BatchRequest() is a *class*
// - Understand how will extend()... extend a C++ class on the spot

class Web3 {
  private:
    boost::filesystem::path defaultPath;
    Utils::Provider defaultProvider;

  public:
    // Object with the classes of all major sub modulesm to be able to
    // instantiate them manually.
    //object modules;
    
    // Current version of the library.
    static std::string version;

    // Wallet object
    Wallet wallet;


    // Property of the Web3 class and of the instance as well
    //static object utils;

    // Current available providers.
    //object providers;

    // When using Web3 in a compatible browser, it will be set as the current
    // native provider by that browser, otherwise as NULL.
    //object givenProvider;

    // Current provider, or NULL if not set.
    //object currentProvider;

    // Changes the provider for the given module.
    // When called on the umbrella class it will also set the provider for
    // ALL sub modules EXCEPT bzz, which needs a separate provider at all times.
    //bool setProvider(object myProvider);

    // Class to create and execute batch requests.
    // Returns an object with the functions `add(request)` and `execute()`.
    //object BatchRequest();

    // Extends the modules.
    // Returns the extended module.
    //object extend(jsonObj methods);

    // Default Constructor.
    Web3();

    // Initialize with different Provider
    Web3(Utils::Provider provider);

    // Initialize with different path
    Web3(boost::filesystem::path path);

    // Initialize with different path and provider...
    Web3(Utils::Provider provider, boost::filesystem::path path);
};

#endif  // WEB3_H
