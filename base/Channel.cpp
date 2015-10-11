#include "../Httpd.hpp"
#include "Channel.hpp"
#include "EventLoop.hpp"


namespace MiniHttp { namespace Base {

Channel::Channel(std::shared_ptr<EventLoop> eventLoop, const int& fd) :
    fd(fd),
    events(0),
    eventLoop(eventLoop) {
    // TODO set default close and error callback
}

Channel::~Channel() {}

void Channel::activate(const unsigned & revents) const {
    if(Event::isError(revents)) {
        Log(DEBUG) << "Activate ERROR callback";
        errorCallback(fd);
    } else if(Event::isClose(revents)) {
        Log(DEBUG) << "Activate CLOSE callback";
        closeCallback(fd);
    } else if(Event::isRead(revents)) {
        if(isRead()) {
            Log(DEBUG) << "Activate READ callback";
            readCallback(fd);
        }
    } else if(Event::isWrite(revents)) {
        if(isWrite()) {
            Log(DEBUG) << "Activate WRITE callback";
            writeCallback(fd);
        }
    }
}

}}
