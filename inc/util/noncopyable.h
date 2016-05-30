#ifndef COLLIE_UTIL_NONCOPYABLE_H_
#define COLLIE_UTIL_NONCOPYABLE_H_

namespace collie {
namespace util {

class NonCopyable {
 public:
  constexpr NonCopyable() noexcept {};
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
  virtual ~NonCopyable() noexcept = 0;
};
}
}

#endif /* COLLIE_UTIL_NONCOPYABLE_H_ */
