set(BASE_SRC
  event/eventloop.cc
  event/channel.cc
  event/threadpool.cc
  event/eventloop_threadpool.cc
  poll/poller.cc
  poll/epoll_poller.cc
  utils/file.cc
  )

set(NETWORK_SRC
  inet_address.cc
  socket.cc
  tcp/tcp_connection.cc
  tcp/tcp_server.cc
  tcp/tcp_socket.cc
  tcp/tcp_acceptor.cc
  tcp/tcp_client.cc
  tcp/tcp_connector.cc
  udp/udp_socket.cc
  udp/udp_server.cc
  udp/udp_client.cc
  )

set(LIB_SRC
  ${BASE_SRC}
  ${NETWORK_SRC})
add_library(collie STATIC ${LIB_SRC})
install(TARGETS collie DESTINATION /usr/local/lib)
