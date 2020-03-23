/*!
 * \file   include/TFEL/Utilities/Span.ixx
 * \brief
 * \author Thomas Helfer
 * \date   22/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_SPAN_IXX
#define LIB_TFEL_UTILITIES_SPAN_IXX

namespace tfel {

  namespace utilities {

    template <typename T, bool const_view>
    Span<T, const_view>::Span() noexcept : b(nullptr),
                                           s(size_t{}) {
    }  // end of Span<T, const_view>::Span

    template <typename T, bool const_view>
    template <typename Container>
    Span<T, const_view>::Span(Container& v) noexcept
        : b(v.size() == 0 ? nullptr : &v[0]),
          s(v.size()) {}  // end of Span<T, const_view>::Span

    template <typename T, bool const_view>
    template <size_t N>
    Span<T, const_view>::Span(value_type v[N]) noexcept  //
        : b(v),
          s(N) {}  // end of Span<T, const_view>::Span

    template <typename T, bool const_view>
    Span<T, const_view>::Span(value_type* const v,
                              const size_type n) noexcept  //
        : b(v),
          s(n) {}  // end of Span<T, const_view>::Span

    template <typename T, bool const_view>
    Span<T, const_view>::Span(Span&&) noexcept = default;

    template <typename T, bool const_view>
    Span<T, const_view>::Span(const Span&) noexcept = default;

    template <typename T, bool const_view>
    Span<T, const_view>& Span<T, const_view>::operator=(Span&&) noexcept =
        default;

    template <typename T, bool const_view>
    Span<T, const_view>& Span<T, const_view>::operator=(const Span&) noexcept =
        default;

    template <typename T, bool const_view>
    typename Span<T, const_view>::iterator
    Span<T, const_view>::begin() noexcept {
      return this->b;
    }  // end of Span<T, const_view>::begin

    template <typename T, bool const_view>
    typename Span<T, const_view>::iterator Span<T, const_view>::end() noexcept {
      return this->b + this->s;
    }  // end of Span<T, const_view>::end

    template <typename T, bool const_view>
    typename Span<T, const_view>::const_iterator Span<T, const_view>::begin()
        const noexcept {
      return this->b;
    }  // end of Span<T, const_view>::begin

    template <typename T, bool const_view>
    typename Span<T, const_view>::const_iterator Span<T, const_view>::end()
        const noexcept {
      return this->b + this->s;
    }  // end of Span<T, const_view>::end

    template <typename T, bool const_view>
    typename Span<T, const_view>::size_type Span<T, const_view>::size() const
        noexcept {
      return this->s;
    }  // end of Span<T, const_view>::size

    template <typename T, bool const_view>
    bool Span<T, const_view>::empty() const noexcept {
      return this->s == 0;
    }  // end of Span<T, const_view>::size

    template <typename T, bool const_view>
    typename Span<T, const_view>::value_type& Span<T, const_view>::operator[](
        const size_type i) noexcept {
      return this->b[i];
    }  // end of Span<T, const_view>::operator

    template <typename T, bool const_view>
    typename Span<T, const_view>::value_type& Span<T, const_view>::operator()(
        const size_type i) noexcept {
      return this->b[i];
    }  // end of Span<T, const_view>::operator

    template <typename T, bool const_view>
    const typename Span<T, const_view>::value_type& Span<T, const_view>::
    operator[](const size_type i) const noexcept {
      return this->b[i];
    }  // end of Span<T, const_view>::operator

    template <typename T, bool const_view>
    const typename Span<T, const_view>::value_type& Span<T, const_view>::
    operator()(const size_type i) const noexcept {
      return this->b[i];
    }  // end of Span<T, const_view>::operator

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_SPAN_IXX */
