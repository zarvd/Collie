#ifndef COLLIE_SOCKET_H
#define COLLIE_SOCKET_H

#include <memory>
#include "Type.hpp"

namespace Collie {

namespace Utils {
class File;
}

enum class IP;
class InetAddress;
class Descriptor;

namespace Socket {

void setNonBlocking(SharedPtr<Descriptor>);
ssize_t send(SharedPtr<Descriptor>, const String & content,
             const int flags = 0);
ssize_t sendTo(SharedPtr<Descriptor>, const String & content,
               SharedPtr<InetAddress> remoteAddr, const int flags = 0);
ssize_t recv(SharedPtr<Descriptor>, String & content, const int flags = 0);
ssize_t recvFrom(SharedPtr<Descriptor>, String & content,
                 const SharedPtr<InetAddress> & remoteAddr,
                 const int flags = 0);
bool sendFile(SharedPtr<Descriptor>, const Utils::File & file);
bool recvFile(SharedPtr<Descriptor>, Utils::File & file, const size_t fileSize);
}
}

#endif /* COLLIE_SOCKET_H */
