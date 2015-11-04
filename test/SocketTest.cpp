#include <gtest/gtest.h>
#include "../include/network/tcp/TcpSocket.hpp"
#include "../include/network/SocketAddress.hpp"

using namespace Collie::Network;
using namespace Collie::Network::Tcp;

std::shared_ptr<SocketAddress> addr =
    SocketAddress::getSocketAddress("127.0.0.1", 8080);
TcpSocket tcpSocket(addr);

TEST(SocketTest, listen) { tcpSocket.listen(); }
