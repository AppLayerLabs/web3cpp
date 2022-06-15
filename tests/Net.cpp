#include "Tests.h"

void Tests::testHTTPRequest() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  json reqBody = RPC::eth_blockNumber();
  std::string reqStr = Net::HTTPRequest(
    web3->getProvider(), Net::RequestTypes::POST, reqBody.dump()
  );
  json respObj = json::parse(reqStr);

  if (respObj.count("error")) {
    failed("Request returned error");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Request body: " << reqBody.dump() << std::endl
    << "* Response body: " << reqStr << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testCustomHTTPRequest() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string host = "api.avax.network";
  std::string port = "443";
  std::string target = "/ext/bc/C/rpc";
  std::string reqType = "POST";
  std::string reqCont = "application/json";
  json reqBody = RPC::eth_blockNumber();
  std::string reqStr = Net::customHTTPRequest(
    reqBody.dump(), host, port, target, reqType, reqCont
  );
  json respObj = json::parse(reqStr);

  if (respObj.count("error")) {
    failed("Request returned error");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Host: " << host << std::endl
    << "* Port: " << port << std::endl
    << "* Target: " << target << std::endl
    << "* Request type: " << reqType << std::endl
    << "* Request content: " << reqCont << std::endl
    << "* Request body: " << reqBody.dump() << std::endl
    << "* Response body: " << reqStr << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}
