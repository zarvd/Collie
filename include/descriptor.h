#ifndef COLLIE_DESCRIPTOR_H_
#define COLLIE_DESCRIPTOR_H_

namespace collie {

/**
 * Descriptor interface
 */
class Descriptor {
 public:
  virtual ~Descriptor() {}
  virtual int Get() const = 0;
};
}

#endif /* COLLIE_DESCRIPTOR_H_ */
