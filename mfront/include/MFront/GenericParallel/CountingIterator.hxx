/*!
 * \file   mfront/include/MFront/GenericParallel/CountingIterator.hxx
 * \brief  This file defines a random access iterator over a range of
 * indices, used by the backends based on the parallel algorithms of the
 * C++ standard library.
 * \date   05/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_COUNTINGITERATOR_HXX
#define LIB_MFRONT_GENERICPARALLEL_COUNTINGITERATOR_HXX

#include <compare>
#include <cstddef>
#include <concepts>
#include <iterator>

namespace mfront::generic_parallel {

  /*!
   * \brief a random access iterator over a range of indices.
   *
   * `std::views::iota` can't be used with the parallel algorithms of some
   * implementations of the standard library: the category of its iterators,
   * in the sense of the C++17 iterators, is not
   * `std::random_access_iterator_tag` and the algorithms then run
   * sequentially (this is the case of libstdc++).
   *
   * \tparam IndexType: type of the index
   */
  template <std::integral IndexType>
  struct CountingIterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = IndexType;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = IndexType;
    constexpr reference operator*() const noexcept { return this->i; }
    constexpr reference operator[](const difference_type n) const noexcept {
      return static_cast<IndexType>(this->i + n);
    }
    constexpr CountingIterator& operator++() noexcept {
      ++(this->i);
      return *this;
    }
    constexpr CountingIterator operator++(int) noexcept {
      auto r = *this;
      ++(this->i);
      return r;
    }
    constexpr CountingIterator& operator--() noexcept {
      --(this->i);
      return *this;
    }
    constexpr CountingIterator operator--(int) noexcept {
      auto r = *this;
      --(this->i);
      return r;
    }
    constexpr CountingIterator& operator+=(const difference_type n) noexcept {
      this->i = static_cast<IndexType>(this->i + n);
      return *this;
    }
    constexpr CountingIterator& operator-=(const difference_type n) noexcept {
      this->i = static_cast<IndexType>(this->i - n);
      return *this;
    }
    friend constexpr CountingIterator operator+(
        const CountingIterator a, const difference_type n) noexcept {
      return {static_cast<IndexType>(a.i + n)};
    }
    friend constexpr CountingIterator operator+(
        const difference_type n, const CountingIterator a) noexcept {
      return {static_cast<IndexType>(a.i + n)};
    }
    friend constexpr CountingIterator operator-(
        const CountingIterator a, const difference_type n) noexcept {
      return {static_cast<IndexType>(a.i - n)};
    }
    friend constexpr difference_type operator-(
        const CountingIterator a, const CountingIterator b) noexcept {
      return static_cast<difference_type>(a.i) -
             static_cast<difference_type>(b.i);
    }
    friend constexpr auto operator<=>(const CountingIterator&,
                                      const CountingIterator&) = default;
    //! \brief current index
    IndexType i;
  };

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_COUNTINGITERATOR_HXX */
