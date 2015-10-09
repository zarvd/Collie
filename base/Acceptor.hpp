#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <memory>
#include <functional>


namespace MiniHttp { namespace Base {

class EventLoop;

class Acceptor {
public:
    typedef std::function<void(int)> ConnectCallback;

    Acceptor(std::shared_ptr<EventLoop>, const int & socketFd);
    Acceptor(const Acceptor &) = delete;
    Acceptor & operator=(const Acceptor &) = delete;
    ~Acceptor();
    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }
    void accept();

private:

    int socketFd;
    std::shared_ptr<EventLoop> eventLoop;
    ConnectCallback connectCallback;
};

}}

#endif /* ACCEPTOR_H */
