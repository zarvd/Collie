#include "../inc/base/InetAddress.h"
#include <iostream>

using namespace collie;

int main(void) {
  auto address1 = std::make_shared<InetAddress>("192.168.199.136", 8080);
  auto address2 =
      std::make_shared<InetAddress>("fe80::260a:64ff:fe38:2cb4", 8080);
  auto address3 = std::make_shared<InetAddress>("bilibili.com", 8080);
  auto address4 = std::make_shared<InetAddress>("localhost", 8080);
  std::cout << "host : " << address1->Host() << std::endl
            << "port : " << address1->Port() << std::endl
            << "host : " << address2->Host() << std::endl
            << "port : " << address2->Port() << std::endl
            << "host : " << address3->Host() << std::endl
            << "port : " << address3->Port() << std::endl
            << "host : " << address4->Host() << std::endl
            << "port : " << address4->Port() << std::endl;

  return 0;
}
