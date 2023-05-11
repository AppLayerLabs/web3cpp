#include "../src/libs/catch2/catch_amalgamated.hpp"
#include "Tests.h"
#include "../include/web3cpp/Wallet.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <boost/filesystem.hpp>

using namespace std;
using Catch::Matchers::Equals;

void initializeWallet(std::unique_ptr<Wallet> &wallet, std::unique_ptr<Provider> &provider, const std::string &folderName, bool clearDB)
{
    bool test = boost::filesystem::exists(boost::filesystem::current_path().string() + folderName);

    if (clearDB)
    {
        if (test)
        {
            boost::filesystem::remove_all(boost::filesystem::current_path().string() + folderName);
        }
    }

    boost::filesystem::path walletPath = boost::filesystem::current_path().string() + folderName;
    provider = std::make_unique<Provider>("avax-c-test");

    wallet = std::make_unique<Wallet>(provider, walletPath);
    return;
}


namespace TWallet
{
    TEST_CASE("Test Wallet Functions", "[wallet]")
    {

        SECTION("Generate Account")
        {
            std::unique_ptr<Wallet> wallet = nullptr;
            std::unique_ptr<Provider> provider = nullptr;
            initializeWallet(wallet, provider, "testWallet", true);
            Error error;
            bool loaded = wallet->loadWallet("password", error);
            REQUIRE(loaded == true);
        
                std::string derivPath = "m/44'/60'/0'/0/0";
                std::string seed = "corn girl crouch desk duck save hedgehog choose kitchen unveil dragon space";
                std::string name = "testAccount";
                std::string example = "0x3e8467983ba80734654208b274ebf01264526117";
                std::string password = "password";

                std::string result = Utils::toLowercaseAddress(wallet->createAccount(
                    derivPath, password, name, error, seed));

                REQUIRE(result == example);
                wallet->deleteAccount(result);
              
        }
        SECTION("Handle Password Check")
        {
            std::unique_ptr<Wallet> wallet = nullptr;
            std::unique_ptr<Provider> provider = nullptr;
            initializeWallet(wallet, provider, "testWallet", true);
            Error error;
            bool loaded = wallet->loadWallet("password", error);
            REQUIRE(loaded == true);

            std::string p1 = "password";
            std::string p2 = "password123";
            std::string p3 = "321password";
            bool e1 = true, e2 = false, e3 = false;

            bool c1 = wallet->checkPassword(p1);
            bool c2 = wallet->checkPassword(p2);
            bool c3 = wallet->checkPassword(p3);
            REQUIRE(c1 == e1);
            REQUIRE(c2 == e2);
            REQUIRE(c3 == e3);
        }

        SECTION("Handle Password Storage Auto")
        {
            std::unique_ptr<Wallet> wallet = nullptr;
            std::unique_ptr<Provider> provider = nullptr;
            initializeWallet(wallet, provider, "testWallet", true);
            Error error;
            bool loaded = wallet->loadWallet("password", error);
            REQUIRE(loaded == true);
            int secs = 3;
            wallet->storePassword("password", secs);
            REQUIRE(wallet->isPasswordStored());
            std::this_thread::sleep_for(std::chrono::seconds(secs*2));
            REQUIRE(!wallet->isPasswordStored());
        }

        SECTION("Sign And Recover Message")
        {

            std::unique_ptr<Wallet> wallet = nullptr;
            std::unique_ptr<Provider> provider = nullptr;
            initializeWallet(wallet, provider, "testWallet", true);
            Error error;
            bool loaded = wallet->loadWallet("password", error);
            REQUIRE(loaded == true);

            if (wallet->getAccounts().size() == 0)
            {
                std::string result = Utils::toLowercaseAddress(wallet->createAccount(
                    "m/44'/60'/0'/0/0", "password", "testAccount", error,
                    "corn girl crouch desk duck save hedgehog choose kitchen unveil dragon space"));
                REQUIRE_FALSE(result != "0x");
            }

            std::string msg, sig, acc, rec = "";
            msg = "Test Message";
            acc = wallet->getAccounts().at(0);
            sig = wallet->sign(msg, acc, "password");
            REQUIRE(Utils::isHexStrict(sig));
            rec = wallet->ecRecover(msg, sig);
            REQUIRE(rec == acc);


        }

        SECTION("Test Transaction")
        {
            std::unique_ptr<Wallet> wallet = nullptr;
            std::unique_ptr<Provider> provider = nullptr;
            initializeWallet(wallet, provider, "testWallet", true);
            Error error;
            bool loaded = wallet->loadWallet("password", error);
            REQUIRE(loaded == true);

            REQUIRE(wallet->getAccounts().size() != 0);

            std::string acc = wallet->getAccounts().at(0);
            const std::unique_ptr<Account> &a = wallet->getAccountDetails(acc);

            // This is dependent on the wallet having balance.
            // BigNumber bal = a.balance().get();
            // if (bal < Utils::toBN(Utils::toWei("0.01", 18))) {
            //   failed("No account found with bal, please create one manually");
            //   std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
            //   logStream << "* Test: " << __func__ << std::endl
            //     << "* Address: " << acc << std::endl
            //     << "* Balance (Wei): " << bal << std::endl
            //     << "* Balance (Fix): " << Utils::fromWei(bal, 18) << std::endl
            //     << "* Test result: " <<
            //       ((curPass) ? "PASSED" : "FAILED - " + curReason)
            //     << std::endl << std::endl;
            //   return;
            // }

            if (a)
            {
                Error buildErr, signErr, sendErr;
                std::string signedTx = "";
                json txRes;
                std::string from = a->address();
                std::string to = a->address();
                BigNumber value(Utils::toWei("0.0001", 18));
                BigNumber gasLimit = 21000;       // 21000 wei
                BigNumber gasPrice = 40000000000; // 40 gwei = 40000000000 wei
                std::string dataHex = "";
                int nonce = a->nonce();
                dev::eth::TransactionSkeleton txSkel = wallet->buildTransaction(
                    from, to, value, gasLimit, gasPrice, dataHex, nonce, buildErr);
                REQUIRE(buildErr.getCode() == 0);
                if (buildErr.getCode() == 0)
                {
                    signedTx = wallet->signTransaction(txSkel, "password", signErr);
                    REQUIRE(signErr.getCode() == 0);
                }
               

                
                // This is dependent on the wallet having balance.
                //  if (buildErr.getCode() == 0 && signErr.getCode() == 0) {
                //    txRes = web3->wallet.sendTransaction(signedTx, sendErr).get();
                //    if (sendErr.getCode() != 0) {
                //      failed("Error on transaction send - " + sendErr.what());
                //    }
                //  }
                //
                //  if (buildErr.getCode() == 0 && signErr.getCode() == 0 && sendErr.getCode() == 0) {
                //    if (!a.saveTxToHistory(signedTx)) {
                //      failed("Transaction was made but not stored in history");
                //    } else {
                //      passed();
                //    }
                //  }

            }
        }
    }
}