#include "../inc/inet_address.h"
#include <iostream>

int main(void) {
  auto address1 = collie::InetAddress::GetInetAddress("192.168.199.136", 8080);
  auto address2 =
      collie::InetAddress::GetInetAddress("fe80::260a:64ff:fe38:2cb4", 8080);
  // auto address = collie::InetAddress::GetInetAddress("localhost", 8080);
  auto address = collie::InetAddress::GetInetAddress("bilibili.com", 8080);
  if (address)
    std::cout << "host : " << address->host() << std::endl
              << "port : " << address->port() << std::endl;

  if (address1)
    std::cout << "host : " << address1->host() << std::endl
              << "port : " << address1->port() << std::endl;
  if (address2)
    std::cout << "host : " << address2->host() << std::endl
              << "port : " << address2->port() << std::endl;

  return 0;
}
