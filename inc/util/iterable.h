#ifndef COLLIE_UTIL_ITERABLE_H_
#define COLLIE_UTIL_ITERABLE_H_

#include <utility>
#include "noncopyable.h"

namespace collie {
namespace util {

// Abstract class
template <class T>
class Iterator {
 public:
  using Self = Iterator<T>;
  constexpr Iterator() noexcept {}
  virtual ~Iterator() noexcept = 0;

  virtual Self& operator++() noexcept = 0;
  virtual Self& operator+(unsigned) noexcept = 0;

  virtual Self& operator--() noexcept = 0;
  virtual Self& operator-(unsigned) noexcept = 0;

  virtual bool operator==(const Self&) const noexcept = 0;
  virtual bool operator!=(const Self&) const noexcept = 0;

  virtual const T& operator*() const noexcept = 0;

  virtual T operator->() const noexcept = 0;
};

template <class T>
Iterator<T>::~Iterator() noexcept {}

// Interface
template <class T>
class Iterable {
 public:
  using Iterator = Iterator<T>;
  constexpr Iterable() noexcept {}
  virtual ~Iterable() noexcept = 0;

  virtual Iterator* begin() noexcept { return Begin(); }
  virtual Iterator* end() noexcept { return End(); }

  virtual const Iterator* begin() const noexcept { return Begin(); }
  virtual const Iterator* end() const noexcept { return End(); }

  virtual Iterator* Begin() noexcept = 0;
  virtual Iterator* End() noexcept = 0;
  virtual const Iterator* Begin() const noexcept = 0;
  virtual const Iterator* End() const noexcept = 0;
};

template <class T>
Iterable<T>::~Iterable() noexcept {}
}
}

#endif /* COLLIE_UTIL_ITERABLE_H_ */
