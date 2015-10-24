set(BASE_SRC
  Utils.cpp
  event/EventLoop.cpp
  event/Channel.cpp
  poll/Poller.cpp
  poll/EPollPoller.cpp
  )

set(NETWORK_SRC
  SocketAddress.cpp
  Socket.cpp
  tcp/TcpConnection.cpp
  tcp/TcpServer.cpp
  tcp/TcpSocket.cpp
  tcp/Acceptor.cpp
  tcp/TcpClient.cpp
  tcp/Connector.cpp
  )

set(LIB_SRC ${BASE_SRC} ${NETWORK_SRC})
add_library(collie STATIC ${LIB_SRC})
install(TARGETS collie DESTINATION /usr/local/lib)
