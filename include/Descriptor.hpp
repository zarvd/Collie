#ifndef COLLIE_DESCRIPTOR_H
#define COLLIE_DESCRIPTOR_H

namespace Collie {

class Descriptor {
public:
    virtual ~Descriptor() = 0;
    virtual int get() const = 0;
};
}

#endif /* COLLIE_DESCRIPTOR_H */
