#include "../../inc/http/HTTPRequest.h"
#include <cassert>
#include <iostream>
#include <sstream>

namespace collie {
namespace http {

HTTPRequest::HTTPRequest(const std::string& message) {
  const auto line = string::split(message, "\r\n");

  const auto start_line = string::split(line[0], " ");

  assert(start_line.size() == 3);

  method = to_http_method(start_line[0]);

  // Parses url
  {
    const auto it = start_line[1].find('?');
    if (it != std::string::npos) {
      url = start_line[1].substr(0, it);
    }
    const auto params = string::split(start_line[1].substr(it + 1), "&");

    for (auto& param : params) {
      const auto it = param.find('=');

      if (it != std::string::npos && it + 1 < param.length()) {
        // If value is null then ignore it
        const std::string key(param.substr(0, it));
        const std::string value(param.substr(it + 1));
        query_params.insert({key, value});
      }
    }
  }

  protocol = start_line[2];

  // // Parses headers

  // Parses body
}

HTTPRequest::~HTTPRequest() {}
}
}
