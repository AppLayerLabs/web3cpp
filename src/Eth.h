#ifndef ETH_H
#define ETH_H

#include <string>
#include <future>

// Module that interacts with the blockchain and smart contracts.
// https://web3js.readthedocs.io/en/v1.7.0/web3-eth.html

// TODO:
// - Implement the other module properties here when they're ready
//   - Personal, Accounts, ABI, Net
//   - Contract and Subscribe weren't implemented yet
// - Inherit the main Web3 class to access the common provider functions
// - Implement defaultHardfork and defaultChain names for Avalanche
// - Replace default Ethereum values for Avalanche ones(?)
// - Decide how to deal with callbacks as parameters for (almost) all functions
// - Decide how to deal with mixed params (Number|BigNumber|String|etc.)
// - Check if some ints can/have to be unsigned/bignumbers/etc.
// - Decide how to deal with "PromiEvents" to implement those functions:
//   - sendTransaction(), sendSignedTransaction()
// - Should defaultAccount be NULL or empty? ("undefined" does not exist in C++)
// - Decide how to deal with big numbers for some functions and properties
//   - defaultBlock: Number|BigNumber|string ("earliest", "latest", "pending")
//   - getHashrate(): is just an unsigned int enough?

public class Eth {
  public:
    // Default address used as the default "from" property, if no "from"
    // property is specified in functions like sendTransaction() or call().
    std::string defaultAccount = NULL;

    // Default block used for certain methods.
    // Can be overriden in some functions' last parameters.
    //mixed defaultBlock = "latest";

    // Default hardfork used for signing transactions locally.
    std::string defaultHardfork;

    // Default chain used for signing transactions locally.
    std::string defaultChain;

    // Default common property for signing transactions locally.
    // "Common" is a JSON object.
    //Common defaultCommon;

    // Amount of new blocks to wait for until first confirmation happens,
    // otherwise issue a timeout.
    // Used over socket-based connections.
    unsigned int transactionBlockTimeout = 50;

    // Amount of seconds to wait for "newBlockHeaders" event before
    // falling back to polling to fetch transaction receipts.
    // Used over socket-based connections and Contract instances.
    unsigned int blockHeaderTimeout = 10;

    // Number of blocks required until a transaction is considered confirmed.
    unsigned int transactionConfirmationBlocks = 24;

    // Number of seconds to wait for a receipt which confirms that
    // a transaction was mined by the network.
    // If this times out, the transaction may still be pending.
    // Used over HTTP connections.
    unsigned int transactionPollingTimeout = 750;

    // Number of milliseconds between Web3 calls for a receipt which confirms
    // that a transaction was mined by the network.
    // Docs say "seconds" but default value and implementations use milliseconds.
    unsigned int transactionPollingInterval = 1000;

    // If enabled, returns the revert reason string on methods like
    // sendTransaction() and call().
    bool handleRevert = false;

    // Threshold above which a warning about the number of event listeners
    // attached to a provider which supports socket subscriptions will be
    // written to the console.
    unsigned int maxListenersWarningThreshold = 100;

    // Returns the protocol version of the node.
    std::future<std::string> getProtocolVersion();

    // Checks if the node is currently syncing and returns either a
    // JSON syncing object, or false.
    //std::future<jsonObj|bool> isSyncing();

    // Returns the Coinbase address to which mining rewards will go.
    std::future<std::string> getCoinbase();

    // Checks whether the node is mining or not.
    std::future<bool> isMining();

    // Returns the number of hashes per second that the node is mining with.
    std::future<unsigned int> getHashrate();

    // Returns the current gas price oracle in Wei (median gas price of the last few blocks).
    std::future<std::string> getGasPrice();

    // Returns base fee per gas and transaction effective priority fee per gas
    // history for the requested block range if available.
    // Range between headBlock-4 and headBlock is guaranteed to be available,
    // while retrieving data from the pending block and older history are
    // optional to support.
    // For pre-EIP-1559 blocks, the gas prices are returned as rewards and
    // zeroes are returned for the base fee per gas.
    //std::future<jsonObj> getFeeHistory(blockCount, newestBlock, rewardPercentiles);

    // Returns a list of accounts the node controls.
    std::future<std::vector<std::string>> getAccounts();

    // Returns the current block number.
    std::future<unsigned int> getBlockNumber();

    // Returns the balance in Wei of an address at a given block.
    //std::future<std::string> getBalance(std::string address, defaultBlock);

    // Returns the value in storage at a specific position of an address.
    //std::future<std::string> getStorageAt(std::string address, position, defaultBlock);

    // Returns the code at a specific address.
    //std::future<std::string> getCode(std::string address, defaultBlock);

    // Returns a block matching the given block number or hash.
    // If returnTransactionObjects is true, the returned block will contain
    // all transactions as objects.
    // If false, will only contain transaction hashes.
    //std::future<jsonObj> getBlock(blockHashOrBlockNumber, bool returnTransactionObjects = false);

    // Returns the number of transactions in a given block.
    //std::future<unsigned int> getBlockTransactionCount(blockHashOrBlockNumber);

    // Returns the number of uncles in a block from a block matching the
    // given block hash or number.
    //std::future<unsigned int> getBlockUncleCount(blockHashOrBlockNumber);

    // Returns a block's uncle by a given uncle index position.
    // An uncle does NOT contain individual transactions.
    // Return structure is the same as getBlock().
    //std::future<jsonObj> getUncle(blockHashOrBlockNumber, bool returnTransactionObjects = false);

    // Returns a transaction matching the given hash.
    //std::future<jsonObj> getTransaction(std::string transactionHash);

    // Returns a list of pending transactions.
    // Return structure is the same as getTransaction().
    //std::future<std::vector<jsonObj>> getPendingTransactions();

    // Returns a transaction based on a block hash or number and the
    // transaction's index position.
    // Return structure is the same as getTransaction().
    //std::future<jsonObj> getTransactionFromBlock(hashStringOrNumber, unsigned int indexNumber);

    // Returns the receipt of a transaction by transaction hash,
    // or NULL on pending/non-existant transactions.
    //std::future<jsonObj> getTransactionReceipt(hash);

    // Returns the number of transactions sent from the given address.
    //std::future<unsigned int> getTransactionCount(std::string address, defaultBlock);

    // Signs data using a specific account, which needs to be unlocked.
    // dataToSign will be converted using Utils.utf8ToHex().
    // Address can be a string or the index of a local wallet in Accounts.Wallet.
    std::future<std::string> sign(std::string dataToSign, std::string address);
    std::future<std::string> sign(std::string dataToSign, unsigned int address);

    // Signs a transaction with a given address, which needs to be unlocked.
    // transactionObject is the same as sendTransaction().
    //std::future<jsonObj> signTransaction(jsonObj transactionObject, std::string address);

    // Executes a message call transaction, which is directly executed in the
    // VM of the node, but never mined in the blockchain.
    // callObject is the same as sendTransaction().
    // Returns the data of the call, e.g. a smart contract function's return value.
    //std::future<std::string> call(jsonObj callObject, defaultBlock);

    // Executes a message call or transaction and returns the amount of gas used.
    // The `from` address MUST be specified, otherwise odd behaviour may be experienced.
    // callObject is the same as sendTransaction().
    // Returns the used gas for the simulated call/transaction, as a HEX string.
    //std::future<std::string> estimateGas(jsonObj callObject);

    // Returns past logs matching the given options.
    // jsonObj return is actually a JSON array of log objects.
    //std::future<std::vector<jsonObj>> getPastLogs(jsonObj options);

    // Returns work for miners to mine on.
    // Returned strings are all in hex form.
    std::future<std::vector<string>> getWork();

    // Submits a proof-of-work solution.
    // Provided strings are all in hex form.
    // Returns true if solution is valid, false otherwise.
    std::future<bool> submitWork(std::string nonce, std::string powHash, std::string digest);

    // Requests/enables the accounts from the current environment.
    // Only works if using an injected provider from an application like
    // MetaMask, Status or TrustWallet.
    // Doesn't work if connected to a node with a default provider.
    // See https://github.com/ethereum/EIPs/blob/master/EIPS/eip-1102.md
    // for more info about the behaviour of this method.
    // Returns an array of enabled accounts.
    std::future<std::vector<std::string>> requestAccounts();

    // Returns the chain ID of the current connected node as described here:
    // https://github.com/ethereum/EIPs/blob/master/EIPS/eip-695.md
    std::future<unsigned int> getChainId();

    // Returns a string with the current client version and info about the node.
    std::future<std::string> getNodeInfo();

    // Returns the account and storage values of the specified account,
    // including the Merkle-proof as described here:
    // https://github.com/ethereum/EIPs/blob/master/EIPS/eip-1186.md
    //std::future<jsonObj> getProof(std::string address, storageKey, blockNumber);

    // Calls to create an access list that amethod execution will access
    // when executed in the VM.
    // The `from` address and `gas` MUST be specified if not already specified
    // in `options` when instantiating a parent contract object.
    // callObject is the same as sendTransaction(), except this method is
    // specifically for contract method executions.
    //std::future<jsonObj> createAccessList(jsonObj callObject);
};

#endif  // ETH_H
