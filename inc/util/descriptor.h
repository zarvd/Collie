#ifndef COLLIE_UTIL_DESCRIPTOR_H_
#define COLLIE_UTIL_DESCRIPTOR_H_

namespace collie {
class Descriptor {
 public:
  Descriptor() {}
  virtual ~Descriptor() {}

  virtual int GetDescriptor() const noexcept = 0;
};
}

#endif /* COLLIE_UTIL_DESCRIPTOR_H_ */
