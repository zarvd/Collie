#ifndef COLLIE_SOCKET_H_
#define COLLIE_SOCKET_H_

#include <memory>

namespace collie {

namespace utils {
class File;
}

enum class IP;
class InetAddress;
class Descriptor;

namespace socket {

void SetNonBlocking(std::shared_ptr<Descriptor>);
ssize_t Send(std::shared_ptr<Descriptor>, const std::string& content,
             const int flags = 0);
ssize_t SendTo(std::shared_ptr<Descriptor>, const std::string& content,
               std::shared_ptr<InetAddress> remote_addr, const int flags = 0);
ssize_t Recv(std::shared_ptr<Descriptor>, std::string& content,
             const int flags = 0);
ssize_t RecvFrom(std::shared_ptr<Descriptor>, std::string& content,
                 const std::shared_ptr<InetAddress>& remote_addr,
                 const int flags = 0);
bool SendFile(std::shared_ptr<Descriptor>, const utils::File& file);
bool RecvFile(std::shared_ptr<Descriptor>, const utils::File& file,
              const size_t recv_size);
}
}

#endif /* COLLIE_SOCKET_H_ */
