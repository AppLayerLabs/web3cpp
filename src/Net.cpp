#include <web3cpp/Net.h>

#include <boost/certify/extensions.hpp>
#include <boost/certify/https_verification.hpp>

namespace beast = boost::beast;
namespace asio = boost::asio;
namespace ssl = asio::ssl;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;



tcp::resolver::results_type
resolve(asio::io_context& ctx, std::string const& hostname)
{
    tcp::resolver resolver{ctx};
    return resolver.resolve(hostname, "https");
}

tcp::socket
connect(asio::io_context& ctx, std::string const& hostname)
{
    tcp::socket socket{ctx};
    asio::connect(socket, resolve(ctx, hostname));
    return socket;
}

std::unique_ptr<ssl::stream<tcp::socket>>
connect(asio::io_context& ctx,
        ssl::context& ssl_ctx,
        std::string const& hostname)
{
    auto stream = boost::make_unique<ssl::stream<tcp::socket>>(
      connect(ctx, hostname), ssl_ctx);
    // tag::stream_setup_source[]
    boost::certify::set_server_hostname(*stream, hostname);
    boost::certify::sni_hostname(*stream, hostname);
    // end::stream_setup_source[]

    stream->handshake(ssl::stream_base::handshake_type::client);
    return stream;
}


namespace Net {
  //std::future<std::string> HTTPRequest(Utils::Provider *provider, RequestTypes requestType, std::string reqBody) {
    std::string HTTPRequest(Utils::Provider *provider, RequestTypes requestType, std::string reqBody) {
    //return std::async([=]{
      std::string result = "";

      // Lock mutex and get information from it.
      provider->ProviderLock.lock();
      std::string url = provider->rpcUrl;
      std::string target = provider->rpcTarget;
      std::string port = boost::lexical_cast<std::string>(provider->rpcPort);
      provider->ProviderLock.unlock();


      std::cout << "url: " << url << std::endl;
      std::cout << "target: " << target << std::endl;
      std::cout << "port: " << port << std::endl;

      std::cout << "try catch block " << std::endl;
      try {
        // Create context and load certificates into it
        boost::asio::io_context ioc;
        ssl::context ctx{ssl::context::sslv23_client};
        ctx.set_verify_mode(ssl::context::verify_peer |
                            ssl::context::verify_fail_if_no_peer_cert);
        ctx.set_default_verify_paths();
        boost::certify::enable_native_https_server_verification(ctx);

        tcp::resolver resolver{ioc};

        // Set SNI Hostname (many hosts need this to handshake successfully)
        //boost::certify::set_server_hostname(stream, url);
        boost::system::error_code ec;
        std::cout << ec << std::endl;

        auto const results = resolver.resolve(url, port);
        std::cout << "1" << std::endl;
        // Connect and Handshake
        auto stream = boost::make_unique<ssl::stream<tcp::socket>>(connect(ioc, url), ctx);

        boost::certify::sni_hostname(*stream, url, ec);

        stream->handshake(ssl::stream_base::client);
        std::cout << "2" << std::endl;
        // Set up an HTTP POST/GET request message
        http::request<http::string_body> req{(requestType == RequestTypes::POST) ? http::verb::post : http::verb::get, target, 11};
        if (requestType == RequestTypes::GET) {
          req.set(http::field::host, url);
          req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
          req.set(http::field::content_type, "application/json");
          req.body() = reqBody;
          req.prepare_payload();
        } else if (requestType == RequestTypes::POST) {
          req.set(http::field::host, url);
          req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
          req.set(http::field::accept, "application/json");
          req.set(http::field::content_type, "application/json");
          req.body() = reqBody;
          req.prepare_payload();
        }

        std::cout << "3" << std::endl;
        // Send the HTTP request to the remote host
        http::write(*stream, req);
        boost::beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        std::cout << "4" << std::endl;
        // Receive the HTTP response
        http::read(*stream, buffer, res);
        //std::cout << res.base().result() << std::endl;
        std::cout << "5" << std::endl;
        // Write only the body answer to output
        std::string body { boost::asio::buffers_begin(res.body().data()),boost::asio::buffers_end(res.body().data()) };
        result = body;
        //Utils::logToDebug("API Result ID " + RequestID + " : " + result);
        //std::cout << "REQUEST RESULT: \n" << result << std::endl; // Uncomment for debugging

        stream->shutdown(ec);

        // SSL Connections return stream_truncated when closed.
        // For that reason, we need to treat this as an error.
        if (ec == boost::asio::error::eof || boost::asio::ssl::error::stream_truncated)
          ec.assign(0, ec.category());
        if (ec) {
          std::cout << "2 throwed " << boost::system::system_error{ec}.what() << std::endl;
          throw boost::system::system_error{ec};
        }
      } catch (std::exception const& e) {
        std::cout << "3 throwed " << e.what() << std::endl;
        throw std::string("HTTP Request error: ") + e.what();
      }

      std::cout << "Return: " << result << std::endl;
      return result;
    //});
  }

  std::future<std::string> customHttpRequest(std::string reqBody, std::string host, std::string port, std::string target, std::string requestType, std::string contentType) {
    return std::async([=]{
      std::string result = "";
      using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
      namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
      namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
      try {
        // Create context and load certificates into it
        boost::asio::io_context ioc;
        ssl::context ctx{ssl::context::sslv23_client};
        load_root_certificates(ctx);

        tcp::resolver resolver{ioc};
        ssl::stream<tcp::socket> stream{ioc, ctx};

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
          boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
          throw boost::system::system_error{ec};
        }
        auto const results = resolver.resolve(host, port);

        // Connect and Handshake
        boost::asio::connect(stream.next_layer(), results.begin(), results.end());
        stream.handshake(ssl::stream_base::client);

        // Set up an HTTP POST/GET request message
        http::request<http::string_body> req{(requestType == "POST") ? http::verb::post : http::verb::get, target, 11};
        if (requestType == "GET") {
          req.set(http::field::host, host);
          req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
          req.set(http::field::content_type, contentType);
          req.body() = reqBody;
          req.prepare_payload();
        } else if (requestType == "POST") {
          req.set(http::field::host, host);
          req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
          req.set(http::field::accept, "application/json");
          req.set(http::field::content_type, contentType);
          req.body() = reqBody;
          req.prepare_payload();
        }

        // Send the HTTP request to the remote host
        http::write(stream, req);
        boost::beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::dynamic_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        // Write only the body answer to output
        std::string body { boost::asio::buffers_begin(res.body().data()),boost::asio::buffers_end(res.body().data()) };
        result = body;
        //Utils::logToDebug("API Result ID " + RequestID + " : " + result);
        //std::cout << "REQUEST RESULT: \n" << result << std::endl; // Uncomment for debugging

        boost::system::error_code ec;
        stream.shutdown(ec);

        // SSL Connections return stream_truncated when closed.
        // For that reason, we need to treat this as an error.
        if (ec == boost::asio::error::eof || boost::asio::ssl::error::stream_truncated)
          ec.assign(0, ec.category());
        if (ec)
          throw boost::system::system_error{ec};
      } catch (std::exception const& e) {
        throw std::string("Error while doing HTTP Custom Request: ") + e.what(); 
      }
      return result;
    });
  }
}