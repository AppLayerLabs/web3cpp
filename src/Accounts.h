#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <future>
#include <string>
#include <vector>

// Module that contains functions to generate accounts and sign transactions and data.
// https://web3js.readthedocs.io/en/v1.7.0/web3-eth-accounts.html

// TODO:
// - Decide how to deal with callbacks for some functions
// - Sort out wallet var/class naming later
// - Check if wallet address list and length should be implemented this way
// - decrypt() shows string as parameter but example shows a JSON object?
// - save() and load(): shouldn't "web3js_wallet" be "web3cpp_wallet"?

class Accounts {
  public:
    //wallet w;

    // Generates an account object with private key and public key.
    // If entropy is given, it should be at least 32 chars.
    // If not, a random string will be generated using Utils.randomHex().
    //jsonObj create(std::string entropy = "");

    // Creates an account object from a private key.
    // privateKey MUST have 32 bytes. If it is in hex, it MUST have the "0x" prefix.
    // If ignoreLength is true, the key's length is not validated.
    //object privateKeyToAccount(std::string privateKey, bool ignoreLength = false);

    // Signs a transaction with a given private key.
    //std::future<jsonObj> signTransaction(jsonObj tx, std::string privateKey);

    // Recovers the address which was used to sign the given transaction.
    // rawTransaction is also called "signature" in the docs.
    std::string recoverTransaction(std::string rawTransaction);

    // Hashes the given message to be passed to Accounts.recover().
    // The data will be UTF-8 HEX decoded and enveloped as
    // `"\x19Ethereum Signed Message:\n" + message.length + message`
    // then hashed using keccak256.
    std::string hashMessage(std::string message);

    // Signs arbitrary data.
    // data will be UTF-* HEX decoded and wrapped as follows:
    // `"\x19Ethereum Signed Message:\n" + message.length + message`
    //jsonObj sign(std::string data, std::string privateKey);

    // Recovers the address which was used to sign the given data.
    // If preFixed is true, the given message will NOT automatically be prefixed with
    // `"\x19Ethereum Signed Message:\n" + message.length + message`
    // and assumed to be already prefixed.
    //std::string recover(jsonObj signatureObject);
    std::string recover(std::string message, std::string signature, bool preFixed = false);
    std::string recover(std::string message, std::string v, std::string r, std::string s, bool preFixed = false);

    // Encrypts a private key to the web3 keystore v3 standard.
    //jsonObj encrypt(std::string privateKey, std::string password);

    // Decrypts a keystore v3 JSON, and creates the account.
    // encryptedPrivateKey is also called "keystoreJsonV3" in the docs.
    //object decrypt(std::string encryptedPrivateKey, std::string password);

    // In-memory wallet with multiple accounts. These accounts can be used
    // when using Eth.sendTransaction().
    class Wallet {
      private:
        std::vector<std::string> addresses;
        unsigned int length;

      public:
        // Generates one or more accounts in the wallet.
        // If wallets already exist they will not be overriden.
        // Leave numberOfAccounts empty to create an empty wallet.
        // If entropy is given, it should have at least 32 chars.
        Wallet create(unsigned int numberOfAccounts, std::string entropy = "");

        // Adds an account to the wallet, using a private key or account object
        // created with Accounts.create().
        //jsonObj add(std::string account);
        //jsonObj add(jsonObj account);

        // Removes an account from the wallet, using the account address or
        // its index number in the wallet.
        // Returns false if the account could not be found.
        bool remove(std::string account);
        bool remove(unsigned int account);

        // Securely empties the wallet and removes all its accounts.
        // Returns the empty Wallet object.
        Wallet clear();

        // Encrypts all wallet accounts to an array of encrypted keystore v3 objects.
        // jsonObj here is actually a JSON array.
        //jsonObj encrypt(std::string password);

        // Decrypts keystore v3 objects.
        // jsonObj here is actually a JSON array.
        //Wallet decrypt(jsonObj keystoreArray, std::string password);

        // Stores the wallet encrypted as a string in local storage.
        // Browser only.
        bool save(std::string password, std::string keyName = "web3js_wallet");

        // Loads a wallet from local storage and decrypts it.
        // Browser only.
        Wallet load(std::string password, std::string keyName = "web3js_wallet");
    };
};

#endif  // ACCOUNTS_H
