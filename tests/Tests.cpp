#include "Tests.h"

int main() {
  std::string http = Net::customHTTPRequest(
    "{\"jsonrpc\": \"2.0\", \"method\": \"eth_gasPrice\", \"params\": [], \"id\": 1}",
    "api.avax.network", "443", "/ext/bc/C/rpc", "POST", "application/json"
  );
  std::cout << http << std::endl;

  /*
  Tests t;
  std::string password = "password";
  std::string derivationPath = "m/44'/60'/0'/0/0";
  t.generateAccount(derivationPath, password);
  t.showResults();
  */
  return 0;
}
