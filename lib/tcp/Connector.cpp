#include "../../include/Global.hpp"
#include "../../include/tcp/Connector.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/event/ThreadPool.hpp"

namespace Collie {
namespace Tcp {

Connector::Connector(std::shared_ptr<SocketAddress> addr) : remoteAddr(addr) {
    Log(TRACE) << "Connector is constructing";
}

Connector::~Connector() { Log(TRACE) << "Connector is destructing"; }

void
Connector::connect(const size_t threadNum, const size_t connectNum) {
    threadPool.reset(new Event::ThreadPool(threadNum));
    threadPool->run();

    for(size_t i = 0; i < connectNum; ++i) {
        threadPool->enqueue([this] {
            auto socket = std::make_shared<TcpSocket>();
            socket->connect(this->remoteAddr);
            this->connectCallback(socket);
            socket->close();
        });
    }
}

void
Connector::disconnect() {
    if(threadPool) threadPool->shutDown();
}
}
}
