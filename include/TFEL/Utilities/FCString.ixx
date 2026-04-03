/*!
 * \file   FCString.ixx
 * \brief
 * \author Thomas Helfer
 * \date   23 janv. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_FCSTRING_IXX
#define LIB_TFEL_UTILITIES_FCSTRING_IXX

#include <array>
#include <string>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"

namespace tfel::utilities {

  template <std::size_t N, typename CharT>
  CStringNarrowedView<N, CharT> narrow(const CharT* s) {
    return {.value = s};
  }

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>::basic_fcstring() {
    std::array<CharT, N + 1>::operator[](0) = '\0';
  }

  template <std::size_t N, typename CharT, typename Traits>
  template <typename Alloc>
  basic_fcstring<N, CharT, Traits>::basic_fcstring(
      const std::basic_string<CharT, Traits, Alloc>& src) {
    this->operator=(src);
  }  // end of basic_fcstring<N,CharT,Traits>::basic_fcstring

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>::basic_fcstring(const CharT* src) {
    this->operator=(src);
  }

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>::basic_fcstring(const CharT* src,
                                                   const bool b) {
    if (!b) {
      this->operator=(src);
    } else {
      this->operator=(narrow<N>(src));
    }
  }  // end of basic_fcstring<N,CharT,Traits>::basic_fcstring

  template <std::size_t N, typename CharT, typename Traits>
  template <std::size_t N2>
  basic_fcstring<N, CharT, Traits>::basic_fcstring(
      const CStringNarrowedView<N2, CharT>& src) {
    this->operator=(src);
  }  // end of basic_fcstring<N,CharT,Traits>::basic_fcstring

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>::basic_fcstring(basic_fcstring&&) = default;

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>::basic_fcstring(const basic_fcstring&) =
      default;

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>& basic_fcstring<N, CharT, Traits>::operator=(
      basic_fcstring&&) = default;

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>& basic_fcstring<N, CharT, Traits>::operator=(
      const basic_fcstring&) = default;

  template <std::size_t N, typename CharT, typename Traits>
  template <typename Alloc>
  basic_fcstring<N, CharT, Traits>& basic_fcstring<N, CharT, Traits>::operator=(
      const std::basic_string<CharT, Traits, Alloc>& rhs) {
    const auto s = rhs.size();
    tfel::raise_if<std::length_error>(s > N,
                                      "basic_fcstring::operator = "
                                      "rhs too long");
    std::copy(rhs.begin(), rhs.end(), this->begin());
    std::array<CharT, N + 1>::operator[](s) = '\0';
    return *this;
  }  // end of basic_fcstring<N,CharT,Traits>::operator =

  template <std::size_t N, typename CharT, typename Traits>
  template <std::size_t N2>
  basic_fcstring<N, CharT, Traits>& basic_fcstring<N, CharT, Traits>::operator=(
      const CStringNarrowedView<N2, CharT>& rhs) {
    const auto s = [&rhs] {
      for (std::size_t i = 0; i != N2; ++i) {
        if (rhs.value[i] == '\0') {
          return i;
        }
      }
      return N2;
    }();
    tfel::raise_if<std::length_error>(s > N,
                                      "basic_fcstring::operator = "
                                      "rhs too long");
    std::copy(rhs.value, rhs.value + s, this->begin());
    std::array<CharT, N + 1>::operator[](s) = '\0';
    return *this;
  }  // end of basic_fcstring<N,CharT,Traits>::operator =

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>& basic_fcstring<N, CharT, Traits>::operator=(
      const CharT* rhs) {
    const auto s = basic_fcstring::strnlen(rhs);
    tfel::raise_if<std::length_error>(s > N,
                                      "basic_fcstring::operator = "
                                      "rhs too long");
    std::copy(rhs, rhs + s, this->begin());
    std::array<CharT, N + 1>::operator[](s) = '\0';
    return *this;
  }  // end of basic_fcstring<N,CharT,Traits>::operator =

  template <std::size_t N, typename CharT, typename Traits>
  CharT& basic_fcstring<N, CharT, Traits>::operator[](const std::size_t i) {
    return this->at(i);
  }  // end of basic_fcstring<N,CharT,Traits>::operator[]

  template <std::size_t N, typename CharT, typename Traits>
  const CharT& basic_fcstring<N, CharT, Traits>::operator[](
      const std::size_t i) const {
    return this->at(i);
  }  // end of basic_fcstring<N,CharT,Traits>::operator[]

  template <std::size_t N, typename CharT, typename Traits>
  CharT& basic_fcstring<N, CharT, Traits>::at(const std::size_t i) {
    tfel::raise_if<std::out_of_range>(i >= N,
                                      "basic_fcstring::at: invalid index");
    return std::array<CharT, N + 1>::operator[](i);
  }  // end of basic_fcstring<N,CharT,Traits>::at

  template <std::size_t N, typename CharT, typename Traits>
  const CharT& basic_fcstring<N, CharT, Traits>::at(const std::size_t i) const {
    tfel::raise_if<std::out_of_range>(i >= N,
                                      "basic_fcstring::at: invalid index");
    return std::array<CharT, N + 1>::operator[](i);
  }  // end of basic_fcstring<N,CharT,Traits>::at

  template <std::size_t N, typename CharT, typename Traits>
  basic_fcstring<N, CharT, Traits>::operator std::basic_string<CharT, Traits>()
      const {
    return std::basic_string<CharT, Traits>(this->data());
  }  // end of basic_fcstring<N,CharT,Traits>::operator std::string

  template <std::size_t N, typename CharT, typename Traits>
  std::size_t basic_fcstring<N, CharT, Traits>::size() const {
    return basic_fcstring::strnlen(this->data());
  }  // end of basic_fcstring<N,CharT,Traits>::size

  template <std::size_t N, typename CharT, typename Traits>
  bool basic_fcstring<N, CharT, Traits>::empty() const {
    return this->operator[](0) == '\0';
  }  // end of basic_fcstring<N,CharT,Traits>::size

  template <std::size_t N, typename CharT, typename Traits>
  auto basic_fcstring<N, CharT, Traits>::end() ->
      typename std::array<CharT, N + 1>::iterator {
    return this->begin() + this->size();
  }  // end of basic_fcstring<N,CharT,Traits>::end

  template <std::size_t N, typename CharT, typename Traits>
  auto basic_fcstring<N, CharT, Traits>::end() const ->
      typename std::array<CharT, N + 1>::const_iterator {
    return this->begin() + this->size();
  }  // end of basic_fcstring<N,CharT,Traits>::end

  template <std::size_t N, typename CharT, typename Traits>
  auto basic_fcstring<N, CharT, Traits>::cend() const ->
      typename std::array<CharT, N + 1>::const_iterator {
    return this->end();
  }  // end of basic_fcstring<N,CharT,Traits>::cend

  template <std::size_t N, typename CharT, typename Traits>
  auto basic_fcstring<N, CharT, Traits>::rbegin() ->
      typename std::array<CharT, N + 1>::reverse_iterator {
    return typename std::array<CharT, N + 1>::reverse_iterator{this->end()};
  }  // end of basic_fcstring<N,CharT,Traits>::rbegin

  template <std::size_t N, typename CharT, typename Traits>
  auto basic_fcstring<N, CharT, Traits>::rbegin() const ->
      typename std::array<CharT, N + 1>::const_reverse_iterator {
    return
        typename std::array<CharT, N + 1>::const_reverse_iterator{this->end()};
  }  // end of basic_fcstring<N,CharT,Traits>::rbegin

  template <std::size_t N, typename CharT, typename Traits>
  auto basic_fcstring<N, CharT, Traits>::crbegin() const ->
      typename std::array<CharT, N + 1>::const_reverse_iterator {
    return
        typename std::array<CharT, N + 1>::const_reverse_iterator{this->end()};
  }  // end of basic_fcstring<N,CharT,Traits>::crbegin

  template <std::size_t N, typename CharT, typename Traits>
  CharT& basic_fcstring<N, CharT, Traits>::back() {
    tfel::raise_if<std::length_error>(this->empty(),
                                      "basic_fcstring::back: empty string");
    return *(this->rbegin());
  }  // end of basic_fcstring<N,CharT,Traits>::back()

  template <std::size_t N, typename CharT, typename Traits>
  const CharT& basic_fcstring<N, CharT, Traits>::back() const {
    tfel::raise_if<std::length_error>(this->empty(),
                                      "basic_fcstring::back: empty string");
    return *(this->crbegin());
  }  // end of basic_fcstring<N,CharT,Traits>::back()

  template <std::size_t N, typename CharT, typename Traits>
  void basic_fcstring<N, CharT, Traits>::pop_back() {
    tfel::raise_if<std::length_error>(this->empty(),
                                      "basic_fcstring::back: empty string");
    *(this->rbegin()) = '\0';
  }  // end of basic_fcstring<N,CharT,Traits>::pop_back()

  template <std::size_t N, typename CharT, typename Traits>
  bool basic_fcstring<N, CharT, Traits>::strcmp(const CharT* s) const {
    return this->strcmp(s, std::strlen(s));
  }

  template <std::size_t N, typename CharT, typename Traits>
  bool basic_fcstring<N, CharT, Traits>::strcmp(
      const CharT* o,
      const typename std::array<CharT, N + 1>::size_type n) const {
    using array = std::array<CharT, N + 1>;
    const auto s = this->size();
    if (s != n) {
      return false;
    }
    auto i = typename std::array<CharT, N + 1>::size_type{};
    while (i != s) {
      if (o[i] != array::operator[](i)) {
        return false;
      }
      ++i;
    }
    return true;
  }

  template <std::size_t N, typename CharT, typename Traits>
  std::size_t basic_fcstring<N, CharT, Traits>::strnlen(const CharT* s) {
    constexpr auto e = '\0';
    auto i = size_t{};
    while ((i != N + 1) && (s[i] != e)) {
      ++i;
    }
    return i;
  }  // end of namespace utilities

  template <std::size_t N, typename CharT, typename Traits>
  std::ostream& operator<<(std::ostream& os,
                           const basic_fcstring<N, CharT, Traits>& s) {
    os << s.data();
    return os;
  }  // end of std::ostream& operator

  template <std::size_t N, typename CharT, typename Traits>
  std::istream& operator>>(std::istream& os,
                           basic_fcstring<N, CharT, Traits>& s) {
    std::basic_string<CharT, Traits> v;
    os >> v;
    s = v;
    return os;
  }  // end of std::ostream& operator

  template <std::size_t N, std::size_t N2, typename CharT, typename Traits>
  bool operator==(const basic_fcstring<N, CharT, Traits>& lhs,
                  const basic_fcstring<N2, CharT, Traits>& rhs) {
    return lhs.strcmp(rhs.data(), rhs.size());
  }
  template <std::size_t N, typename CharT, typename Traits, typename Alloc>
  bool operator==(const basic_fcstring<N, CharT, Traits>& lhs,
                  const std::basic_string<CharT, Traits, Alloc>& rhs) {
    return rhs == lhs.data();
  }
  template <std::size_t N, typename CharT, typename Traits, typename Alloc>
  bool operator==(const std::basic_string<CharT, Traits, Alloc>& lhs,
                  const basic_fcstring<N, CharT, Traits>& rhs) {
    return lhs == rhs.data();
  }
  template <std::size_t N, typename CharT, typename Traits>
  bool operator==(const basic_fcstring<N, CharT, Traits>& lhs,
                  const CharT* rhs) {
    return lhs.strcmp(rhs);
  }
  template <std::size_t N, typename CharT, typename Traits>
  bool operator==(const CharT* lhs,
                  const basic_fcstring<N, CharT, Traits>& rhs) {
    return rhs.strcmp(lhs);
  }
  template <std::size_t N, std::size_t N2, typename CharT, typename Traits>
  bool operator!=(const basic_fcstring<N, CharT, Traits>& lhs,
                  const basic_fcstring<N2, CharT, Traits>& rhs) {
    return !(lhs == rhs);
  }
  template <std::size_t N, typename CharT, typename Traits, typename Alloc>
  bool operator!=(const basic_fcstring<N, CharT, Traits>& lhs,
                  const std::basic_string<CharT, Traits, Alloc>& rhs) {
    return !(lhs == rhs);
  }
  template <std::size_t N, typename CharT, typename Traits, typename Alloc>
  bool operator!=(const std::basic_string<CharT, Traits, Alloc>& lhs,
                  const basic_fcstring<N, CharT, Traits>& rhs) {
    return !(lhs == rhs);
  }
  template <std::size_t N, typename CharT, typename Traits>
  bool operator!=(const basic_fcstring<N, CharT, Traits>& lhs,
                  const CharT* rhs) {
    return !(lhs == rhs);
  }
  template <std::size_t N, typename CharT, typename Traits>
  bool operator!=(const CharT* lhs,
                  const basic_fcstring<N, CharT, Traits>& rhs) {
    return !(lhs == rhs);
  }

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_FCSTRING_IXX */
