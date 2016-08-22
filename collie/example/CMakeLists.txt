add_executable(InetAddress InetAddress)
target_link_libraries(InetAddress collie)

add_executable(EchoServer EchoServer)
target_link_libraries(EchoServer collie)

add_executable(EchoClient EchoClient)
target_link_libraries(EchoClient collie)

add_executable(AsyncEchoServer AsyncEchoServer)
target_link_libraries(AsyncEchoServer collie)


# add_executable(timer timer)
# target_link_libraries(timer collie)

# add_executable(logger logger)
# target_link_libraries(logger collie)

# add_executable(epoll epoll)
# target_link_libraries(epoll collie)

# add_executable(async_echo_server async_echo_server)
# target_link_libraries(async_echo_server collie)
