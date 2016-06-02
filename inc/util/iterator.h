#ifndef COLLIE_UTIL_ITERATOR_H_
#define COLLIE_UTIL_ITERATOR_H_

#include <functional>

namespace collie {
namespace util {

// Iterator for constant value
template <class Iter, class Elem>
class ConstIterator {
 public:
  constexpr ConstIterator() noexcept {}
  virtual ~ConstIterator() noexcept = 0;

  virtual bool HasNext() const noexcept = 0;
  virtual bool HasPrevious() const noexcept = 0;

  virtual const Elem& GetValue() const noexcept = 0;

  virtual void ToNext(const unsigned offset = 1) noexcept = 0;
  virtual void ToPrevious(const unsigned offset = 1) noexcept = 0;

  virtual void ToFirst() noexcept = 0;
  virtual void ToLast() noexcept = 0;
};

// Iterator for mutable value
template <class Iter, class Elem>
class Iterator : public ConstIterator<Iter, Elem> {
 public:
  constexpr Iterator() noexcept {}
  virtual ~Iterator() noexcept = 0;
  virtual Elem& GetValue() noexcept = 0;
};

template <class Iterator, typename Elem>
class ConstIterable {
 public:
  constexpr ConstIterable() noexcept {}
  virtual ~ConstIterable() noexcept = 0;

  virtual Iterator GetIterator() const noexcept = 0;
  virtual Iterator GetReversedIterator() const noexcept = 0;

  virtual void ForEach(std::function<void(const Elem&)> functor,
                       bool positive_order = true) const noexcept {
    if (positive_order) {
      auto it = GetIterator();
      while (it.HasNext()) {
        functor(it.GetValue());
        it.ToNext();
      }
    } else {
      auto it = GetReversedIterator();
      while (it.HasPrevious()) {
        functor(it.GetValue());
        it.ToPrevious();
      }
    }
  }
};

template <class Iterator, typename Elem>
class Iterable : public ConstIterable<Iterator, Elem> {
 public:
  constexpr Iterable() noexcept {}
  virtual ~Iterable() noexcept = 0;

  virtual Iterator GetIterator() const noexcept = 0;
  virtual Iterator GetReversedIterator() const noexcept = 0;

  virtual void ForEach(std::function<void(Elem&)> functor,
                       bool positive_order = true) noexcept {
    if (positive_order) {
      auto it = GetIterator();
      while (it.HasNext()) {
        functor(it.GetValue());
        it.ToNext();
      }
    } else {
      auto it = GetReversedIterator();
      while (it.HasPrevious()) {
        functor(it.GetValue());
        it.ToPrevious();
      }
    }
  }
};
}
}

#endif /* COLLIE_UTIL_ITERATOR_H_ */
