#include "../inc/inet_address.h"
#include <iostream>

int main(void) {
  auto address1 = collie::InetAddress::GetInetAddress("192.168.199.136", 8080);
  auto address2 =
      collie::InetAddress::GetInetAddress("fe80::260a:64ff:fe38:2cb4", 8080);
  auto address3 = collie::InetAddress::GetInetAddress("bilibili.com", 8080);
  auto address4 = collie::InetAddress::GetInetAddress("localhost", 8080);
  if (address1)
    std::cout << "host : " << address1->GetHost() << std::endl
              << "port : " << address1->GetPort() << std::endl;
  if (address2)
    std::cout << "host : " << address2->GetHost() << std::endl
              << "port : " << address2->GetPort() << std::endl;
  if (address3)
    std::cout << "host : " << address3->GetHost() << std::endl
              << "port : " << address3->GetPort() << std::endl;
  if (address4)
    std::cout << "host : " << address4->GetHost() << std::endl
              << "port : " << address4->GetPort() << std::endl;

  return 0;
}
