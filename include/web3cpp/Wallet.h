#ifndef WALLET_H
#define WALLET_H

#include <chrono>
#include <ctime>
#include <future>
#include <string>
#include <thread>
#include <vector>

#include "Utils.h"

#include <boost/algorithm/string/replace.hpp>
#include <nlohmann/json.hpp>

#include <bip3x/crypto/sha3.h>  // keccak_256()
#include <secp256k1.h>
#include <secp256k1_recovery.h>

#include <web3cpp/devcore/Common.h>
#include <web3cpp/devcore/FixedHash.h>
#include <web3cpp/devcore/Address.h>
#include <web3cpp/ethcore/KeyManager.h>
#include <web3cpp/ethcore/TransactionBase.h>
#include <web3cpp/Error.h>
#include <web3cpp/Account.h>
#include <web3cpp/DB.h>
#include <web3cpp/Cipher.h>
#include <web3cpp/Bip39.h>
#include <web3cpp/Provider.h>

using json = nlohmann::ordered_json;

/**
 * Abstraction for a single wallet.
 */

class Wallet {
  private:
    dev::bytesSec passHash;       ///< Hash for the wallet's password
    dev::h256 passSalt;           ///< Salt for the wallet's password.
    int passIterations = 100000;  ///< Number of PBKDF2 iterations to hash+salt the wallet's password.
    boost::filesystem::path path; ///< The wallet's folder path.
    Provider* provider;           ///< Pointer to Web3::defaultProvider.
    Database infoDB;              ///< The wallet's information database.
    Database accountDB;           ///< The wallet's account database.
    bool _isLoaded;               ///< Indicates the wallet is properly loaded.
    std::string _password;        ///< In-memory plain text copy of the wallet's password. Set by the user when they want to "remember" the password.
    std::time_t _passEnd;         ///< Timestamp after which the password will be "forgotten"/erased from memory.
    std::thread _passThread;      ///< Thread object that runs passHandler().

    /// Get the wallet's "wallet.info" file path. Usually used to check if the wallet exists.
    boost::filesystem::path walletExistsPath() { return path.string() + "/wallet.info"; };

    /// Get the wallet's root folder path.
    boost::filesystem::path walletFolder() { return path.string() + "/wallet"; };

    /// Get the wallet's account database folder path.
    boost::filesystem::path accountsFolder() { return path.string() + "/wallet/accounts"; };

    /// Get the wallet's seed phrase file path.
    boost::filesystem::path seedPhraseFile() { return path.string() + "/wallet/seed"; };

    /// Get the wallet's transaction database root folder path.
    boost::filesystem::path transactionsFolder() { return path.string() + "/wallet/transactions"; }

    /// Threaded function that handles the logic of storing/clearing password to/from memory.
    void passHandler() {
      while (true) {
        std::time_t now = std::time(nullptr);
        if (now >= this->_passEnd) break;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      this->_password = "";
      this->_passEnd = 0;
      return;
    }

    /**
     * Creates a new wallet.
     * Called by loadWallet() if no wallet is found on the desired path.
     * Also attempts to create a default account in the process.
     * @param &password The wallet's password.
     * @param &error Error object.
     * @return `true` if the wallet was successfully created, `false` otherwise.
     *         Will still return `true` if the default account wasn't created.
     */
    bool createNewWallet(const std::string& password, Error &error);

  public:
    /**
     * Constructor.
     * @param *_provider Pointer to the provider that will be used.
     * @param _path The path for the wallet.
     */
    Wallet(Provider* _provider, boost::filesystem::path _path)
      : provider(_provider), path(_path),
        infoDB("walletInfo", walletFolder()),
        accountDB("accounts", walletFolder())
    {};

    Provider* getProvider() { return this->provider; } ///< Getter for provider.

    /**
     * Load a wallet with the given password.
     * Creates a new wallet if no wallet exists.
     * @param &password The wallet's password.
     * @param &error Error object.
     * @return `true` if the wallet is successfully loaded, `false` otherwise.
     */
    bool loadWallet(const std::string& password, Error &error);

    /**
     * Check if a wallet is loaded.
     * @return `true` if wallet is loaded, `false` otherwise.
     */
    bool isLoaded() { return this->_isLoaded; }

    /**
     * Check if a given password matches with the wallet's.
     * @param &password The password to be checked.
     * @return `true` if the password matches the wallet's, `false` otherwise.
     */
    bool checkPassword(const std::string& password);

    /**
     * Check if the wallet file and its keys folder exists in a given folder.
     * @param &wallet_path The folder to be checked.
     * @return `true` if a wallet exists in the given folder, `false` otherwise.
     */
    static bool walletExists(boost::filesystem::path &wallet_path);

    /**
     * Create a new account. Address is stored as lowercase.
     * @param derivPath The full derivation path of the account (e.g. "m/44'/60'/0'/0").
     * @param &password The wallet's password.
     * @param name A custom human-readable name/label for the account.
     * @param &error Error object.
     * @param seed (optional) The BIP39 seed phrase to use for generating the account.
     *                        If no seed is passed, uses the BIP39 seed from the wallet.
     * @return The checksum address of the new account, or an empty string on failure.
     */
    std::string createAccount(
      std::string derivPath, const std::string& password, std::string name,
      Error &error, std::string seed = ""
    );

    /**
     * Import a given private key into the wallet. Address is stored as lowercase.
     * @param &secret The private/public key pair to import.
     * @param &password The wallet's password.
     * @param name A custom human-readable name/label for the account.
     * @param &derivPath The full derivation path of the account (e.g. "m/44'/60'/0'/0").
     * @param &error Error object.
     * @return `true` if the key was successfully imported, `false` otherwise.
     */
    bool importPrivKey(
      dev::Secret const &secret, std::string const &password,
      std::string const &name, std::string const &derivPath, Error &error
    );

    /**
     * Delete an existing account.
     * @param address The address of the account to be deleted. Will be converted
     *                to lowercase before attempting to delete.
     * @return `true` if the account was successfully deleted or didn't exist
     *         anyway prior to requesting deletion, `false` otherwise.
     */
    bool deleteAccount(std::string address);

    /**
     * Sign a data string as an "Ethereum Signed Message".
     * [EIP-712](https://eips.ethereum.org/EIPS/eip-712) compliant.
     * @param dataToSign The data to be signed.
     * @param address The address to use for signing.
     * @param password The wallet's password.
     * @return The hex signature which is meant to be used on ecRecover(),
     *         or an error message on failure.
     */
    std::string sign(
      std::string dataToSign, std::string address, std::string password
    );

    /**
     * Recover the account that signed a given data string.
     * @param dataThatWasSigned The unencrypted data string that was signed.
     * @param signature The raw signature that came from sign().
     * @return The original account used to sign the data string.
     */
    std::string ecRecover(
      std::string dataThatWasSigned, std::string signature
    );

    /**
     * Build a transaction from user data.
     * @param from The address that will make the transaction.
     * @param to The address that will receive the transaction. For coin transactions,
     *           this is the destination address. For token transactions, this is
     *           the token contract's address.
     * @param value The value of the transaction (in Wei). For token transactions
     *              this is 0, as the token value would be packed inside `dataHex`.
     * @param gasLimit The gas limit of the transaction (in Wei).
     * @param gasPrice The gas price of the transaction (in Wei).
     * @param dataHex The arbitrary data of the transaction. For coin transactions
     *                this is blank. For token transactions this would be a
     *                packed ABI call.
     * @param nonce The nonce of the `from` address.
     * @param &error Error object.
     * @param creation (optional) Sets whether the transaction is creating a contract
     *                 or not. Defaults to false.
     * @return A struct filled with data for the transaction, ready to be signed,
     *         or an empty/incomplete struct on failure.
     */
    dev::eth::TransactionSkeleton buildTransaction(
      std::string from, std::string to, BigNumber value,
      BigNumber gasLimit, BigNumber gasPrice, std::string dataHex,
      int nonce, Error &error, bool creation = false
    );

    /**
     * Sign a built transaction.
     * @param txObj The transaction struct returned from buildTransaction().
     * @param password The wallet's password.
     * @param &err Error object.
     * @return The raw transaction signature ready to be sent, or an empty string on failure.
     */
    std::string signTransaction(
      dev::eth::TransactionSkeleton txObj, std::string password, Error &err
    );

    /**
     * Send/broadcast a signed transaction to the blockchain.
     * @param signedTx The raw transaction signature returned from signTransaction().
     * @param &err Error object.
     * @return A JSON object with the send results (either "result" or "error")
     *         and the transaction's raw signature.
     */
    std::future<json> sendTransaction(std::string signedTx, Error &err);

    /**
     * Store the wallet's password in memory.
     * @param password The wallet's password.
     * @param seconds (optional) The number of seconds to store the password in memory.
     *                Defaults to 0, which means "store indefinitely until cleared manually".
     */
    void storePassword(const std::string& password, unsigned int seconds = 0);

    void clearPassword(); ///< Clear the wallet's password from memory.

    /**
     * Check if the wallet has a password stored in memory.
     * @return `true` is password is stored in memory, `false` otherwise.
     */
    bool isPasswordStored();

    /**
     * Get the accounts stored in the wallet.
     * @return A list of addresses from the wallet.
     */
    std::vector<std::string> getAccounts();

    /**
     * Get the account seed phrase
     * @param password the Wallet's password.
     * @param &err Error object.
     * @return The wallet seed phrase.
     */

    std::string getSeedPhrase(const std::string& password, Error &err);

    /**
     * Get the details for a specific account.
     * @param address The address of the account. Will be converted to lowercase.
     * @return A struct with details of the account, or an empty struct
     *         if the address is not found.
     */
    Account getAccountDetails(std::string address);

    /**
     * Get the raw structure of a specific account.
     * @param address The address of the account. Will be converted to lowercase.
     * @return A JSON object with the raw details of the account, or an empty
     *         JSON object if the address is not found.
     */
    json getAccountRawDetails(std::string address);
};

#endif  // WALLET_H
