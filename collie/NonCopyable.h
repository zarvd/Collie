#pragma once

namespace collie {

class NonCopyable {
 public:
  constexpr NonCopyable() noexcept {};
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
  virtual ~NonCopyable() noexcept = 0;
};
}
