/*!
 * \file   include/TFEL/Math/Vector/vector.ixx
 * \brief
 * \author Helfer Thomas
 * \date   05 oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_VECTOR_IXX_
#define LIB_TFEL_VECTOR_IXX_

#include <cassert>

namespace tfel {

  namespace math {

    template <typename T>
    vector<T>::vector(const typename vector<T>::size_type s)
        : std::vector<T>(s) {}

    template <typename T>
    vector<T>::vector(const std::initializer_list<T>& v) : std::vector<T>(v) {}

    template <typename T>
    vector<T>::vector(const typename vector<T>::size_type s, const T& v)
        : std::vector<T>(s, v) {}

    template <typename T>
    template <typename InputIterator>
    vector<T>::vector(const InputIterator b, const InputIterator e)
        : std::vector<T>(b, e) {}

    template <typename T>
    vector<T>& vector<T>::operator=(const vector<T>& src) {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(), src.size());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      std::vector<T>::operator=(src);
      return *this;
    }

    template <typename T>
    vector<T>& vector<T>::operator+=(const vector<T>& src) {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(), src.size());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      for (i = 0; i < this->size(); ++i) {
        std::vector<T>::operator[](i) += src(i);
      }
      return *this;
    }

    template <typename T>
    vector<T>& vector<T>::operator-=(const vector<T>& src) {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(), src.size());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      for (i = 0; i < this->size(); ++i) {
        std::vector<T>::operator[](i) -= src(i);
      }
      return *this;
    }

    template <typename T>
    const typename vector<T>::RunTimeProperties vector<T>::getRunTimeProperties(
        void) const {
      return std::vector<T>::size();
    }

    template <typename T>
    T& vector<T>::operator()(const typename vector<T>::size_type i) noexcept {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      assert(i < this->size());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      return std::vector<T>::operator[](i);
    }

    template <typename T>
    constexpr const T& vector<T>::operator()(
        const typename vector<T>::size_type i) const noexcept {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      assert(i < this->size());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      return std::vector<T>::operator[](i);
    }

    template <typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            vector<T>&>::type
    vector<T>::operator=(const Expr<vector<T2>, Operation>& expr) {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
                                           expr.getRunTimeProperties());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      size_type s = this->size();
      for (size_type i = 0; i != s; ++i) {
        std::vector<T>::operator[](i) = expr(i);
      }
      return *this;
    }

    template <typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            vector<T>&>::type
    vector<T>::operator+=(const Expr<vector<T2>, Operation>& expr) {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
                                           expr.getRunTimeProperties());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      size_type s = this->size();
      for (size_type i = 0; i != s; ++i) {
        std::vector<T>::operator[](i) += expr(i);
      }
      return *this;
    }

    template <typename T>
    template <typename T2, typename Operation>
    typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                            vector<T>&>::type
    vector<T>::operator-=(const Expr<vector<T2>, Operation>& expr) {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
                                           expr.getRunTimeProperties());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      size_type s = this->size();
      for (size_type i = 0; i != s; ++i) {
        std::vector<T>::operator[](i) -= expr(i);
      }
      return *this;
    }

    template <typename T>
    template <typename InputIterator>
    void vector<T>::copy(const InputIterator b, const InputIterator e) {
      std::copy(b, e, this->v);
    }

    template <typename T>
    void vector<T>::swap(vector<T>& a) {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(), a.size());
#endif /* LIB_TFEL_VECTOR_IXX_ */
      std::vector<T>::swap(a);
    }

    template <typename T>
    vector<T>::~vector() noexcept {}

    template <typename T>
    TFEL_MATH_INLINE2 typename std::enable_if<
        tfel::typetraits::IsScalar<T>::cond,
        typename tfel::typetraits::RealPartType<T>::type>::type
    norm(const vector<T>& vec) {
      T n(0);
      typename vector<T>::size_type i;
      for (i = 0; i != vec.size(); ++i) {
        const T v = vec(i);
        n += v * v;
      }
      return std::sqrt(real(n));
    }

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Vector/vectorResultType.hxx"

#endif /* LIB_TFEL_VECTOR_IXX_ */
