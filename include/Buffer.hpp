#ifndef COLLIE_BUFFER_H
#define COLLIE_BUFFER_H

#include <string>

namespace Collie {

class Buffer {
public:
    Buffer();
    Buffer(const Buffer &) = delete;
    Buffer & operator=(const Buffer &) = delete;
    ~Buffer();

    unsigned size();
    Buffer & operator<<(const char *);
    Buffer & operator<<(const std::string &);
    const std::string & get() const { return content; }

private:
    std::string content;
};

}

#endif /* COLLIE_BUFFER_H */
