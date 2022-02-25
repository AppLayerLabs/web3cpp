#include <string>
#include <future>

// Module that interacts with the blockchain and smart contracts.

// TODO:
// - Implement the other module properties when they're ready
//   - subscribe, Contract, Iban, personal, accounts, ens, abi, net
// - Inherit the main Web3 class to access the common provider functions
// - Implement defaultHardfork and defaultChain names for Avalanche
// - Replace default Ethereum values for Avalanche ones(?)
// - Decide how to deal with callbacks as parameters for (almost) all functions
// - Decide how to deal with mixed params (Number|BigNumber|String|etc.)
// - Check if some ints can/have to be unsigned/bignumbers/etc.
// - Decide how to deal with "PromiEvents" for some functions
//   - sendTransaction(), sendSignedTransaction()

public class eth {
  public:
    std::string defaultAccount;
    //mixed defaultBlock; // Number|BigNumber|string ("earliest", "latest", "pending")
    std::string defaultHardfork;
    std::string defaultChain;
    //Common defaultCommon; // "Common" is an object
    int transactionBlockTimeout = 50;
    int blockHeaderTimeout = 10;  // Also used on Contract instances
    int transactionConfirmationBlocks = 24;
    int transactionPollingTimeout = 750;
    int transactionPollingInterval = 1; // Actually "1000ms"
    bool handleRevert = false;
    int maxListenersWarningThreshold = 100;

    std::promise<std::string> getProtocolVersion();
    //std::promise<object|bool> isSyncing();
    std::promise<std::string> getCoinbase();
    std::promise<bool> isMining();
    std::promise<int> getHashrate();
    std::promise<std::string> getGasPrice(); // in Wei
    //std::promise<object> getFeeHistory(blockCount, newestBlock, rewardPercentiles);
    std::promise<std::vector<std::string>> getAccounts();
    std::promise<int> getBlockNumber();
    //std::promise<std::string> getBalance(std::string address, defaultBlock);
    //std::promise<std::string> getStorageAt(std::string address, position, defaultBlock);
    //std::promise<std::string> getCode(std::string address, defaultBlock);
    //std::promise<object> getBlock(blockHashOrBlockNumber, bool returnTransactionObjects = false);
    //std::promise<int> getBlockTransactionCount(blockHashOrBlockNumber);
    //std::promise<int> getBlockUncleCount(blockHashOrBlockNumber);
    //std::promise<object> getUncle(blockHashOrBlockNumber, bool returnTransactionObjects = false); // Return is same as getBlock()
    //std::promise<object> getTransaction(std::string transactionHash);
    //std::promise<std::vector<object>> getPendingTransactions(); // Object is the same as getTransaction()
    //std::promise<object> getTransactionFromBlock(hashStringOrNumber, int indexNumber);
    //std::promise<object> getTransactionReceipt(hash); // Returns null on pending/non-existant transactions
    //std::promise<int> getTransactionCount(std::string address, defaultBlock);
    std::promise<std::string> sign(std::string dataToSign, std::string address);  // Address can also be a number(?)
    //std::promise<object> signTransaction(transactionObject, std::string address);
    //std::promise<std::string> call(callObject, defaultBlock);
    //std::promise<int> estimateGas(callObject);
    //std::promise<std::vector<object>> getPastLogs(object options);
    std::promise<std::vector<string>> getWork();  // Strings are all in hex form
    std::promise<bool> submitWork(std::string nonce, std::string powHash, std::string digest);  // Strings are all in hex form
    std::promise<std::vector<std::string>> requestAccounts();
    std::promise<int> getChainId();
    std::promise<std::string> getNodeInfo();
    //std::promise<object> getProof(std::string address, storageKey, blockNumber);
    //std::promise<object> createAccessList(callObject); // callObject is the same as sendTransaction(), this is specific for contract method executions
};
