#ifndef COLLIE_BASE_SERIALIZABLE_H_
#define COLLIE_BASE_SERIALIZABLE_H_

namespace collie {
namespace base {

class String;

class Serializable {
 public:
  constexpr Serializable() noexcept {}
  virtual ~Serializable() noexcept {}

  virtual String ToString() const noexcept = 0;
};
}
}

#endif /* COLLIE_BASE_SERIALIZABLE_H_ */
