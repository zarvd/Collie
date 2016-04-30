add_executable(inet_address inet_address)
target_link_libraries(inet_address collie)

add_executable(timer timer)
target_link_libraries(timer collie)

add_executable(logger logger)
target_link_libraries(logger collie)

add_executable(epoll epoll)
target_link_libraries(epoll collie)

add_executable(echo_server echo_server)
target_link_libraries(echo_server collie)

add_executable(echo_client echo_client)
target_link_libraries(echo_client collie)

add_executable(async_echo_server async_echo_server)
target_link_libraries(async_echo_server collie)
