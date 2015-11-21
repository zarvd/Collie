#ifndef COLLIE_TCP_TCPCONNECTION_H
#define COLLIE_TCP_TCPCONNECTION_H

#include <string>
#include <memory>
#include <unordered_set>
#include "../Type.hpp"

namespace Collie {

namespace Utils {
class File;
}

namespace Event {

class EventLoop;
class Channel;
}

namespace TCP {

class TCPConnection : public SharedFromThis<TCPConnection> {
public:
    using MessageCallback = std::function<void(SharedPtr<TCPConnection>)>;
    using EventCallback = std::function<void(SharedPtr<TCPConnection>)>;

    TCPConnection(SharedPtr<Event::Channel> channel);
    TCPConnection(const TCPConnection &) = delete;
    TCPConnection & operator=(const TCPConnection &) = delete;
    ~TCPConnection();

    SharedPtr<Event::Channel> getChannel() const { return channel; }
    void setMessageCallback(const MessageCallback & cb) {
        messageCallback = cb;
    }
    void setMessageCallback(const MessageCallback && cb) {
        messageCallback = std::move(cb);
    }

    void setShutdownCallback(const EventCallback & cb) {
        shutdownCallback = cb;
    }
    void setShutdownCallback(const EventCallback && cb) {
        shutdownCallback = std::move(cb);
    }

    void disconnect();
    String recvAll();
    void send(const String &);
    void sendFile(const String & fileName);
    void sendFile(const Utils::File & file);
    void recvFile(const String & fileName, const size_t fileSize);

private:
    void shutdown();
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    bool connected;
    bool isShutDown;
    const SharedPtr<Event::Channel> channel;
    String inputBuffer;
    String outputBuffer;
    MessageCallback messageCallback;
    EventCallback shutdownCallback;
};

extern thread_local UnorderedSet<SharedPtr<TCPConnection>>
    localThreadConnections;
}
}

#endif /* COLLIE_TCP_TCPCONNECTION_H */
