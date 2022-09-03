/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_ITERATOR_PAIR_H_
#define RENDU_ITERATOR_PAIR_H_

#include <utility>

namespace rendu {
  /**
   * @class IteratorPair
   *
   * @brief Utility class to enable range for loop syntax for multimap.equal_range uses
   */
  template<class iterator>
  class IteratorPair {
  public:
    constexpr IteratorPair() : _iterators() {}

    constexpr IteratorPair(iterator first, iterator second) : _iterators(first, second) {}

    constexpr IteratorPair(std::pair<iterator, iterator> iterators) : _iterators(iterators) {}

    constexpr iterator begin() const { return _iterators.first; }

    constexpr iterator end() const { return _iterators.second; }

  private:
    std::pair<iterator, iterator> _iterators;
  };

  namespace Containers {
    template<typename iterator>
    constexpr rendu::IteratorPair<iterator> MakeIteratorPair(iterator first, iterator second) {
      return {first, second};
    }

    template<typename iterator>
    constexpr rendu::IteratorPair<iterator> MakeIteratorPair(std::pair<iterator, iterator> iterators) {
      return iterators;
    }

    template<class M>
    inline auto MapEqualRange(M &map, typename M::key_type const &key) -> IteratorPair<decltype(map.begin())> {
      return {map.equal_range(key)};
    }
  }
  //! namespace Containers
}
//! namespace rendu

#endif // RENDU_ITERATOR_PAIR_H_
