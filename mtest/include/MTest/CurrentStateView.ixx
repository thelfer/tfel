/*!
 * \file   mtest/include/MTest/CurrentStateView.ixx
 * \brief
 * \author Thomas Helfer
 * \date   21/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_CURRENTSTATEVIEW_IXX
#define LIB_MTEST_CURRENTSTATEVIEW_IXX

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  namespace internals {

    template <typename T, bool const_view>
    VectorView<T, const_view>::VectorView(vector_type& v) noexcept
        : b(v.size() == 0 ? nullptr : &v[0]),
          s(v.size()) {}  // end of VectorView<T, const_view>::VectorView

    template <typename T, bool const_view>
    VectorView<T, const_view>::VectorView(VectorView&&) noexcept = default;

    template <typename T, bool const_view>
    VectorView<T, const_view>::VectorView(const VectorView&) noexcept = default;

    template <typename T, bool const_view>
    typename VectorView<T, const_view>::value_type*
    VectorView<T, const_view>::begin() noexcept {
      return this->b;
    }  // end of VectorView<T, const_view>::begin

    template <typename T, bool const_view>
    typename VectorView<T, const_view>::value_type*
    VectorView<T, const_view>::end() noexcept {
      return this->b + this->s;
    }  // end of VectorView<T, const_view>::end

    template <typename T, bool const_view>
    typename VectorView<T, const_view>::size_type
    VectorView<T, const_view>::size() const noexcept {
      return this->s;
    }  // end of VectorView<T, const_view>::size

    template <typename T, bool const_view>
    bool VectorView<T, const_view>::empty() const noexcept {
      return this->s == 0;
    }  // end of VectorView<T, const_view>::size

    template <typename T, bool const_view>
    typename VectorView<T, const_view>::value_type& VectorView<T, const_view>::
    operator[](const size_type i) noexcept {
      return this->b[i];
    }  // end of VectorView<T, const_view>::operator

    template <typename T, bool const_view>
    typename VectorView<T, const_view>::value_type& VectorView<T, const_view>::
    operator()(const size_type i) noexcept {
      return this->b[i];
    }  // end of VectorView<T, const_view>::operator

  }  // end of namespace internals

}  // end of namespace mtest

#endif /* LIB_MTEST_CURRENTSTATEVIEW_IXX */
