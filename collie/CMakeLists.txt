cmake_minimum_required(VERSION 3.6)
project(collie)
set(CMAKE_CXX_FLAGS "-std=c++14")

include_directories(
  ${CMAKE_SOURCE_DIR}/..
  )

set(COLLIE_HEADER_DIRS
  .
  tcp
  http
  util
  )

foreach(dir ${COLLIE_HEADER_DIRS})
  file(GLOB_RECURSE headers ${dir}/*.h)
  set(COLLIE_HEADERS
    ${COLLIE_HEADERS}
    ${headers})
endforeach()

set(COLLIE_SOURCES
  InetAddress.cc
  Logger.cc
  LogStream.cc
  LogHandler.cc
  Poller.cc
  EPollPoller.cc
  EventPool.cc
  EventThreadPool.cc
  tcp/TCPSocket.cc
  tcp/TCPServer.cc
  tcp/TCPStream.cc
  tcp/TCPClient.cc
  tcp/AsyncTCPServer.cc
  tcp/AsyncTCPStream.cc
  )

add_library(collie STATIC ${COLLIE_SOURCES})
# #install(TARGETS collie DESTINATION /usr/local/lib)

option(BUILD_EXAMPLES "BUILD_EXAMPLES" OFF)

if(BUILD_EXAMPLES)
  add_executable(EchoClient example/EchoClient.cc)
  target_link_libraries(EchoClient collie)
  add_executable(EchoServer example/EchoServer.cc)
  target_link_libraries(EchoServer collie)
endif()
