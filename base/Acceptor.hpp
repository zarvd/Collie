#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <memory>
#include <functional>


namespace MiniHttp { namespace Base {

class EventLoop;
class Channel;

class Acceptor {
public:
    Acceptor(std::shared_ptr<EventLoop>, const int & socketFd);
    Acceptor(const Acceptor &) = delete;
    Acceptor & operator=(const Acceptor &) = delete;
    ~Acceptor();
    std::shared_ptr<Channel> getChannel() const {
        return channel;
    }
    void connectCallback();
    void accept();

private:

    int socketFd;
    std::shared_ptr<EventLoop> eventLoop;
    std::shared_ptr<Channel> channel;
};

}}

#endif /* ACCEPTOR_H */
