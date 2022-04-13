#ifndef WEB3_H
#define WEB3_H

#include <string>

#include <nlohmann/json.hpp>

#include "Utils.h"
#include "Wallet.h"
#include "Error.h"

// Main/Umbrella class that houses all modules.

// TODO:
// - Decide how to implement the objects: modules, utils
// - Perhaps study how std::function works to answer the following questions:
//  - Why is BatchRequest() a *class*?
//  - How will extend()... extend a C++ class on the spot?

class Web3 {
  private:
    boost::filesystem::path defaultPath;
    Utils::Provider defaultProvider;

  public:
    // Constructor and overloads.
    Web3(Utils::Provider *provider = NULL, boost::filesystem::path *path = NULL);
    Web3(Utils::Provider provider) : Web3(&provider, NULL) {}
    Web3(boost::filesystem::path path): Web3(NULL, &path) {}

    // Object with the classes of all major sub modules,
    // to be able to instantiate them manually.
    //object modules;

    // Current version of the library.
    static std::string version;

    // The proper wallet object.
    Wallet wallet;

    // Property of the Web3 class and of the instance as well.
    //static object utils;

    // Current available providers.
    std::vector<Utils::Provider> providers;

    // When using Web3 in a compatible browser, it will be set as the current
    // native provider by that browser, otherwise as NULL.
    Utils::Provider givenProvider;

    // Current provider, or NULL if not set.
    Utils::Provider currentProvider;

    // Changes the provider for the given module.
    // When called on the umbrella class it will also set the provider for
    // ALL sub modules EXCEPT bzz, which needs a separate provider at all times.
    bool setProvider(Utils::Provider myProvider);

    // Class to create and execute batch requests.
    // Returns an object with the functions `add(request)` and `execute()`.
    //object BatchRequest();

    // Extends the modules.
    // Returns the extended module.
    //object extend(json methods);
};

#endif  // WEB3_H
