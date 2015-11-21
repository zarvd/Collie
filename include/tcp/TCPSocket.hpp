#ifndef COLLIE_TCP_TCPSOCKET_H
#define COLLIE_TCP_TCPSOCKET_H

#include <memory>
#include "../Type.hpp"
#include "../Descriptor.hpp"

namespace Collie {

class InetAddress;

namespace TCP {

class TCPSocket : public Descriptor {
public:
    enum class State {
        Init,
        Socket,
        Bind,
        Listen,
        Accept,
        IllegalAccept,
        Connect,
        Close
    };
    // Accept: which is the connection socket that accept() returned
    // IllegalAccept: which is the ILLEGAL connection socket that accept()
    // returned

    explicit TCPSocket(SharedPtr<InetAddress> localAddr) noexcept;
    TCPSocket(const TCPSocket &) = delete;
    TCPSocket & operator=(const TCPSocket &) = delete;
    ~TCPSocket() noexcept override;

    // Descriptor
    int get() const override { return fd; }
    State getState() const { return state; }
    SharedPtr<InetAddress> getAddress() const { return address; }

    bool connect(SharedPtr<InetAddress>);
    bool bindAndListen();
    SharedPtr<TCPSocket> accept(bool blocking = false);

    // send and recv
    String recv();
    void send(const String & msg);
    void close() noexcept;

private:
    // construct Accept connection socket
    TCPSocket(const int fd, SharedPtr<InetAddress> addr) noexcept;
    // construct illegal socket
    TCPSocket() noexcept;

    static SharedPtr<TCPSocket> getAcceptSocket(const int fd,
                                                SharedPtr<InetAddress>);
    static SharedPtr<TCPSocket> getIllegalAcceptSocket();
    bool listenV4();
    SharedPtr<TCPSocket> acceptV4(bool blocking);
    bool connectV4(SharedPtr<InetAddress> servAddr);
    // bool listenV6();
    // int acceptV6(SharedPtr<InetAddress>, bool blocking);
    // bool connectV6(SharedPtr<InetAddress>);

    int fd;
    State state;
    SharedPtr<InetAddress> address;
};
}
}

#endif /* COLLIE_TCP_TCPSOCKET_H */
