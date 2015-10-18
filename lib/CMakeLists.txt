set(BASE_SRC
  base/Utils.cpp
  base/event/EventLoop.cpp
  base/event/Channel.cpp
  base/poll/EPollPoller.cpp
  )

set(NETWORK_SRC
  network/SocketAddress.cpp
  network/Socket.cpp
  network/tcp/TcpServer.cpp
  network/tcp/TcpSocket.cpp
  network/tcp/Acceptor.cpp
  )

set(LIB_SRC ${BASE_SRC} ${NETWORK_SRC})
add_library(collie STATIC ${LIB_SRC})
install(TARGETS collie DESTINATION /usr/local/lib)
