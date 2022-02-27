#include <future>
#include <string>

// Module that contains functions to generate accounts and sign transactions and data

// TODO:
// - Decide how to deal with callbacks for some functions
// - Sort out wallet var/class naming later
// - Check if some ints can be unsigned
// - Check if wallet address list and length should be implemented this way

public class accounts {
  public:
    //wallet w;

    //object create(std::string entropy = "");
    //object privateKeyToAccount(std::string privateKey, bool ignoreLength = false);
    //std::promise<object> signTransaction(object tx, std::string privateKey);
    std::string recoverTransaction(std::string rawTransaction); // rawTransaction is also called signature
    std::string hashMessage(std::string message);
    //object sign(std::string data, std::string privateKey);
    //std::string recover(object signatureObject);
    std::string recover(std::string message, std::string signature, bool preFixed = false);
    std::string recover(std::string message, std::string v, std::string r, std::string s, bool preFixed = false);
    //object encrypt(std::string privateKey, std::string password);
    //object decrypt(std::string encryptedPrivateKey, std::string password);  // encryptedPrivateKey is also called keystoreJsonV3

    public class wallet {
      private:
        std::vector<std::string> addresses;
        int length;

      public:
        //object create(int numberOfAccounts, std::string entropy = "");
        //object add(std::string account);
        //object add(object account);
        bool remove(std::string account);
        bool remove(int account); // Index in wallet
        //object clear();
        //array encrypt(std::string password);
        //object decrypt(array keystoreArray, std::string password);
        bool save(std::string password, std::string keyName = "web3js_wallet"); // Shouldn't it be "web3cpp_wallet"?
        //object load(std::string password, std::string keyName = "web3js_wallet"); // Shouldn't it be "web3cpp_wallet"?
    };
};
