#ifndef COLLIE_UTIL_NONCOPYABLE_H_
#define COLLIE_UTIL_NONCOPYABLE_H_

namespace collie {

class NonCopyable {
 public:
  NonCopyable(){};
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
  virtual ~NonCopyable() = 0;
};
}

#endif /* COLLIE_UTIL_NONCOPYABLE_H_ */
