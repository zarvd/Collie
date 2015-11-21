#include "../include/Global.hpp"
#include "../include/Socket.hpp"
#include "../include/InetAddress.hpp"
#include "../include/utils/File.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>

namespace Collie {
namespace Socket {

void
setNonBlocking(const int fd) {
    auto flags = ::fcntl(fd, F_GETFL, 0);
    REQUIRE_SYS(flags != -1);
    flags |= O_NONBLOCK;
    const int ret = ::fcntl(fd, F_SETFL, flags);
    REQUIRE_SYS(ret != -1);
}

ssize_t
send(const int socketFd, const std::string & content, const int flag) {
    char contentChars[content.length() + 1];
    std::strcpy(contentChars, content.c_str());
    Log(TRACE) << "Socket is sending " << contentChars;
    const ssize_t size =
        ::send(socketFd, contentChars, sizeof(contentChars), flag);
    REQUIRE_SYS(size != -1);
    if(size == 0) {
        Log(DEBUG) << "Socket send nothing";
    } else {
        Log(TRACE) << "Socket send msg";
    }
    return size;
}

ssize_t
recv(const int socketFd, std::string & content, const int flag) {
    if(socketFd < 2) {
        Log(WARN) << "Illegal socket fd " << socketFd;
    }
    constexpr size_t msgLength = 8192; // FIXME
    char msg[msgLength];
    const ssize_t size = ::recv(socketFd, msg, msgLength, flag);
    REQUIRE_SYS(size != -1);
    if(size == 0) {
        Log(DEBUG) << "Socket received nothing";
        content = "";
    } else {
        Log(TRACE) << "Socket received msg";
        content = msg;
    }

    return size;
}

ssize_t
sendTo(const int socketFd, const std::string & content,
       std::shared_ptr<InetAddress> remoteAddr, const int flag) {
    char contentC[content.length() + 1];
    std::strcpy(contentC, content.c_str());
    struct sockaddr_in addr = remoteAddr->getAddrV4();
    const ssize_t size =
        ::sendto(socketFd, contentC, sizeof(contentC), flag,
                 (struct sockaddr *)&addr, sizeof(remoteAddr->getAddrV4()));
    REQUIRE_SYS(size != -1);
    if(size == 0) {
        Log(DEBUG) << "Socket send nothing";
    } else {
        Log(TRACE) << "Socket send msg";
    }
    return size;
}

ssize_t
recvFrom(const int socketFd, std::string & content,
         const std::shared_ptr<InetAddress> & remoteAddr, const int flag) {
    char buffer[8192];
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    const ssize_t size = ::recvfrom(socketFd, buffer, sizeof(buffer), flag,
                                    (struct sockaddr *)&addr, &addrLen);
    REQUIRE_SYS(size != -1);
    if(size == 0) {
        Log(DEBUG) << "Socket received nothing";
        content = "";
    } else {
        Log(TRACE) << "Socket received msg";
        content = buffer;
        (*remoteAddr) = addr;
    }
    return size;
}

bool
sendFile(const int socketFd, const Utils::File & file) {
    REQUIRE(file.isExisted() && file.isRead());
    if(file.isExisted() && file.isFile()) {
        off64_t offset = 0;
        size_t sentSize = 0;
        while(sentSize < file.getSize()) {
            const ssize_t ret =
                ::sendfile64(socketFd, file.getFd(), &offset, file.getSize());
            Log(DEBUG) << "sendfile64() return " << ret;
            if(ret == -1) {
                Log(ERROR) << getError();
                return false;
            }
            sentSize += static_cast<size_t>(ret);
        }
        return true;
    } else {
        return false;
    }
}

bool
recvFile(const int socketFd, Utils::File & file, const size_t fileSize) {
    REQUIRE(file.isExisted() && file.isWrite());
    if(file.isExisted() && file.isFile()) {
        char buffer[fileSize];
        int ret = ::recv(socketFd, buffer, fileSize, MSG_WAITALL);
        REQUIRE_SYS(ret != -1);
        ret = ::write(file.getFd(), buffer, ret);
        REQUIRE_SYS(ret != -1);
        return true;
    } else {
        return false;
    }
}
}
}
