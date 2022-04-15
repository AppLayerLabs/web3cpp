#include "Tests.h"

int main() {
  Tests t;
  std::string password = "password";
  std::string derivationPath = "m/44'/60'/0'/0/0";
  t.generateAccount(derivationPath, password);
  t.showResults();
  return 0;
}
