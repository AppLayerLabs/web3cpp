#include <future>
#include <string>

// Module that interacts with accounts

// TODO:
// - Inherit the umbrella class to get access to common functions
//   - setProvider(), providers, givenProvider, currentProvider, BatchRequest(), extend()
// - Decide how to deal with callbacks on (almost) all functions
// - Check if some ints can be unsigned

public class personal {
  public:
    std::promise<std::string> newAccount(std::string password);
    std::promise<std::string> sign(std::string dataToSign, std::string address, std::string password);
    std::promise<std::string> ecRecover(std::string dataThatWasSigned, std::string signature);
    //std::promise<object> signTransaction(object transaction, std::string password);
    //std::string sendTransaction(object transactionOptions, std::string password);
    void unlockAccount(std::string address, std::string password, int unlockDuration);  // Apparently in seconds?
    std::promise<bool> lockAccount(std::string address);
    std::promise<std::vector<std::string>> getAccounts();
    std::promise<std::string> importRawKey(std::string privateKey, std::string password); // Priv key is hex string
};
