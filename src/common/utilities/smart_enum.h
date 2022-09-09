/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_SMART_ENUM_H_
#define RENDU_SMART_ENUM_H_

#include "iterator_pair.h"
#include <iterator>
#include <magic_enum.hpp>
#include <magic_enum_fuse.hpp>
using namespace magic_enum;
using namespace magic_enum::bitwise_operators;

class EnumUtils {
public:
  template<typename Enum>
  static size_t Count() {
    return enum_count<Enum>();
  }

  template<typename Enum>
  static std::string ToString(Enum value) {
    return enum_flags_name(value);
  }

  template<typename Enum>
  static Enum FromIndex(size_t index) {
    return static_cast<Enum>(index);
  }

  template<typename Enum>
  static auto ToIndex(Enum value) {
    return enum_index(value);
  }


  template<typename Enum>
  static Enum FromInter(int value) {
    return ; }


  template<typename Enum>
  static bool IsValid(Enum value) {
    try {
      return magic_enum::enum_contains(value);;
    } catch (...) {
      return false;
    }
  }

  template<typename Enum>
  static bool IsValid(std::underlying_type_t<Enum> value) {
    return IsValid(static_cast<Enum>(value));
  }

  template<typename Enum>
  class Iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = Enum;
    using pointer = Enum *;
    using reference = Enum &;
    using difference_type = std::ptrdiff_t;

    Iterator() : _index(EnumUtils::Count<Enum>()) {}

    explicit Iterator(size_t index) : _index(index) {}

    bool operator==(const Iterator &other) const { return other._index == _index; }

    bool operator!=(const Iterator &other) const { return !operator==(other); }

    difference_type operator-(Iterator const &other) const { return _index - other._index; }

    bool operator<(const Iterator &other) const { return _index < other._index; }

    bool operator<=(const Iterator &other) const { return _index <= other._index; }

    bool operator>(const Iterator &other) const { return _index > other._index; }

    bool operator>=(const Iterator &other) const { return _index >= other._index; }

    value_type operator[](difference_type d) const { return FromIndex<Enum>(_index + d); }

    value_type operator*() const { return operator[](0); }

    Iterator &operator+=(difference_type d) {
      _index += d;
      return *this;
    }

    Iterator &operator++() { return operator+=(1); }

    Iterator operator++(int) {
      Iterator i = *this;
      operator++();
      return i;
    }

    Iterator operator+(difference_type d) const {
      Iterator i = *this;
      i += d;
      return i;
    }

    Iterator &operator-=(difference_type d) {
      _index -= d;
      return *this;
    }

    Iterator &operator--() { return operator-=(1); }

    Iterator operator--(int) {
      Iterator i = *this;
      operator--();
      return i;
    }

    Iterator operator-(difference_type d) const {
      Iterator i = *this;
      i -= d;
      return i;
    }

  private:
    difference_type _index;
  };

  template<typename Enum>
  static Iterator<Enum> Begin() { return Iterator<Enum>(0); }

  template<typename Enum>
  static Iterator<Enum> End() { return Iterator<Enum>(); }

  template<typename Enum>
  static rendu::IteratorPair<Iterator<Enum>> Iterate() { return {Begin<Enum>(), End<Enum>()}; }


};

#endif
