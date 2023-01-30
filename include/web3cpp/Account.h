#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <future>
#include <string>
#include <vector>

#include <web3cpp/DB.h>
#include <web3cpp/Net.h>
#include <web3cpp/Provider.h>
#include <web3cpp/Utils.h>
#include <web3cpp/RPC.h>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>

using json = nlohmann::ordered_json;

/**
 * Abstraction for a single account.
 * Contains details such as address, nonce, transaction history, etc..
 */

class Account {
  private:
    std::string _address;           ///< Address for the account.
    std::string _name;              ///< Custom name/label for the account.
    std::string _derivationPath;    ///< Complete derivation path for the account (e.g. "m/44'/60'/0'/0").
    uint64_t _nonce;                ///< Current nonce for the account.
    bool _isLedger;                 ///< Indicates the account is imported from a Ledger device.
    Provider *provider;             ///< Pointer to Web3::defaultProvider.
    mutable std::mutex accountLock; ///< Mutex for managing read/write access to the account object.
    Database transactionDB;         ///< Database of transactions made with the account.

  public:
    /// Empty constructor.
    Account(){}

    /**
     * Default constructor.
     * @param walletPath The path for the wallet from which the account comes from.
     * @param name Custom name/label for the account.
     * @param __address Address for the account.
     * @param __derivationPath Full derivation path for the account (e.g. `m/44'/60'/0'/0`).
     * @param __isLedger Flag to set whether the account comes from a Ledger device or not.
     * @param *_provider Pointer to the provider used by the account.
     */
    Account(
      const boost::filesystem::path& walletPath, const std::string& __address, const std::string& __name,
      const std::string& __derivationPath, bool __isLedger, Provider *_provider
    );

    /// Copy constructor.
    Account(const Account& other) noexcept :
      _address(other._address),
      _name(other._name),
      _derivationPath(other._derivationPath),
      _isLedger(other._isLedger),
      _nonce(other._nonce),
      provider(other.provider),
      transactionDB(other.transactionDB)
    {}
    
    const std::string& address()        const { return _address; }           ///< Getter for the address.
    const std::string& name()           const { return _name; }              ///< Getter for the custom name/label.
    const std::string& derivationPath() const { return _derivationPath; }    ///< Getter for the derivation path.
    const uint64_t& nonce()             const { return _nonce; }             ///< Getter for the nonce.
    bool isLedger()                     const { return _isLedger; }          ///< Getter for the Ledger flag.

    /**
     * Request the account's balance from the network.
     * @return The balance in Wei as a BigNumber, or 0 if the request fails.
     */
    std::future<BigNumber> balance() const;

    /**
     * Save a transaction to the account's local history database.
     * @param signedTx The raw transaction signature that will be decoded and stored.
     * @return `true` on success, `false` on failure.
     */
    bool saveTxToHistory(std::string signedTx);

    /**
     * Get all saved transactions from this account's local history database.
     * @return The account's transaction history as a JSON object.
     */
    json getTxHistory() const;
};

#endif  // ACCOUNTS_H
