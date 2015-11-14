set(BASE_SRC
  event/EventLoop.cpp
  event/Channel.cpp
  event/ThreadPool.cpp
  event/EventLoopThreadPool.cpp
  poll/Poller.cpp
  poll/EPollPoller.cpp
  )

set(NETWORK_SRC
  SocketAddress.cpp
  Socket.cpp
  tcp/TCPConnection.cpp
  tcp/TCPServer.cpp
  tcp/TCPSocket.cpp
  tcp/Acceptor.cpp
  tcp/TCPClient.cpp
  tcp/Connector.cpp
  udp/UDPSocket.cpp
  udp/UDPServer.cpp
  udp/UDPClient.cpp
  )

set(LIB_SRC ${BASE_SRC} ${NETWORK_SRC})
add_library(collie STATIC ${LIB_SRC})
install(TARGETS collie DESTINATION /usr/local/lib)
