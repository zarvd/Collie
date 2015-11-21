#ifndef COLLIE_TYPE_H
#define COLLIE_TYPE_H

#include <utility>
#include <type_traits>

namespace std {
// Functional
template <class T>
struct less;
template <class T>
struct hash;
template <class T>
struct equal_to;
template <class T>
class function;
template <class Ret, class... Args>
class function<Ret(Args...)>;

// String
template <class charT>
struct char_traits;
template <>
struct char_traits<char>;
template <>
struct char_traits<wchar_t>;
template <>
struct char_traits<char16_t>;
template <>
struct char_traits<char32_t>;
template <class charT, class traits, class Alloc>
class basic_string;

// Containers
template <class T, class Alloc>
class vector;
template <class Key,     // map::key_type
          class T,       // map::mapped_type
          class Compare, // map::key_compare
          class Alloc>   // map::allocator_type
class map;
template <class Key, class T, class Hash, class Pred, class Alloc>
class unordered_map;
template <class T, class Compare, class Alloc>
class set;
template <class Key, class Hash, class Pred, class Alloc>
class unordered_set;
template <class T, class Container>
class queue;
template <class T, class Alloc>
class deque;
template <class T, class Alloc>
class stack;

// Memory
template <class T>
class allocator;
template <class T>
struct default_delete;
template <class T>
class shared_ptr;
template <class T, class D>
class unique_ptr;
template <class T>
class weak_ptr;
template <class T>
class enable_shared_from_this;
template <class T, class... Args>
shared_ptr<T> make_shared(Args &&... args);
template <class T, class... Args>
unique_ptr<T, default_delete<T>> make_unique(Args &&... args);
}

namespace Collie {

// Memory
template <class T>
using Allocator = std::allocator<T>;

template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T>
using UniquePtr = std::unique_ptr<T, std::default_delete<T>>;

template <class T>
using WeakPtr = std::weak_ptr<T>;

template <class T>
using SharedFromThis = std::enable_shared_from_this<T>;

template <typename T, typename... Args>
constexpr SharedPtr<T>
MakeShared(Args &&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
template <typename T, typename... Args>
constexpr UniquePtr<T>
MakeUnique(Args &&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// Functional

template <class T>
using Less = std::less<T>;

template <class T>
using EqualTo = std::equal_to<T>;

template <class T>
using Hash = std::hash<T>;

// String
using String = std::basic_string<char, std::char_traits<char>, Allocator<char>>;

// Container
template <class T1, class T2>
using Pair = std::pair<T1, T2>;

template <class T>
using Vec = std::vector<T, Allocator<T>>;

template <class Key, class T>
using Map = std::map<Key, T, Less<Key>, Allocator<Pair<const Key, T>>>;

template <class T>
using Set = std::set<T, Less<T>, Allocator<T>>;

template <class T>
using Deque = std::deque<T, Allocator<T>>;

template <class T>
using Queue = std::queue<T, Deque<T>>;

template <class T>
using Stack = std::stack<T, Allocator<T>>;

template <class Key>
using UnorderedSet =
    std::unordered_set<Key, Hash<Key>, EqualTo<Key>, Allocator<Key>>;

template <class Key, class T>
using UnorderedMap = std::unordered_map<Key, T, Hash<Key>, EqualTo<Key>,
                                        Allocator<Pair<const Key, T>>>;
}

#endif /* COLLIE_TYPE_H */
