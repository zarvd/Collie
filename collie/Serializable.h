#ifndef COLLIE_BASE_SERIALIZABLE_H_
#define COLLIE_BASE_SERIALIZABLE_H_

#include "String.h"

namespace collie {

class Serializable {
 public:
  constexpr Serializable() noexcept {}
  virtual ~Serializable() noexcept {}

  virtual std::string ToString() const noexcept = 0;
};
}

#endif /* COLLIE_BASE_SERIALIZABLE_H_ */
