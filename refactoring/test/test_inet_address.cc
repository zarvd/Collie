#include "../inc/inet_address.h"
#include <iostream>

int main(void) {
  auto address = collie::InetAddress::GetInetAddress("baidu.com", 8080);
  if (address)
    std::cout << "host : " << address->host() << std::endl
              << "port : " << address->port() << std::endl;
  return 0;
}
