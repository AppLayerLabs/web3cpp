#ifndef NET_H
#define NET_H

#include <future>
#include <string>
#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <web3cpp/Provider.h>
#include <web3cpp/Utils.h>
#include <web3cpp/root_certificates.h>
#include <web3cpp/Error.h>

// Module for making HTTP requests.

namespace Net {
  enum RequestTypes { POST, GET };
  std::string HTTPRequest(
    Provider *provider, RequestTypes requestType, std::string reqBody
  );
  std::string customHTTPRequest(
    std::string reqBody, std::string host, std::string port,
    std::string target, std::string requestType, std::string contentType
  );
}

#endif  // NET_H
