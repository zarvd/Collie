#ifndef COLLIE_TCP_TCPSOCKET_H
#define COLLIE_TCP_TCPSOCKET_H

#include <memory>
#include "../Type.hpp"
#include "../Descriptor.hpp"

namespace Collie {

class InetAddress;

namespace Utils {
class File;
}

namespace TCP {

class TCPSocket : public Descriptor, public SharedFromThis<TCPSocket> {
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
    SharedPtr<TCPSocket> accept(bool blocking = false) noexcept;

    // send and recv
    ssize_t recv(String & content, const int flags = 0);
    ssize_t send(const String & content, const int flags = 0);
    bool sendFile(const Utils::File & file);
    bool recvFile(Utils::File & file, const size_t recvSize);
    void close() noexcept;

private:
    // construct Accept connection socket
    TCPSocket(const int fd, SharedPtr<InetAddress> addr) noexcept;
    // construct illegal socket
    TCPSocket() noexcept;

    SharedPtr<TCPSocket> getAcceptSocket(const int fd,
                                         SharedPtr<InetAddress>) noexcept;
    SharedPtr<TCPSocket> getIllegalAcceptSocket() noexcept;
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
