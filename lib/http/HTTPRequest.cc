#include "../../inc/http/HTTPRequest.h"
#include <sstream>

namespace collie {
namespace http {

HTTPRequest::HTTPRequest(const std::string& message) {
  std::istringstream iss(message);

  std::string start_line;
  std::getline(iss, start_line);

  std::string method_str;
  std::string query_str;
  std::string protocol_str;

  using std::getline;

  // Seperates method, url and protocol
  if (!getline(getline(getline(iss, method_str, ' '), query_str, ' '),
               protocol_str))
    throw std::runtime_error("Unknown http request");
  // Parses method
  method = to_http_method(method_str);

  // Parses url
  {
    std::istringstream iqs(query_str);
    getline(iqs, url, '?');

    // Parses query param
    std::string param;
    while (getline(iqs, param, '&')) {
      std::istringstream ips(param);
      std::string key, value;
      if (getline(getline(ips, key, '='), value))
        query_params.insert({key, value});
    }
  }

  protocol = protocol_str;

  // Parses headers
  {
  }

  // Parses body
}
}
}
