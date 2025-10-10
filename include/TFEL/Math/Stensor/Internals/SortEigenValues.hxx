/*!
 * \file   SortEigenValues.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 janv. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_INTERNALS_SORTEIGENVALUES_HXX
#define LIB_TFEL_MATH_INTERNALS_SORTEIGENVALUES_HXX

namespace tfel::math::internals {

  /*!
   * \brief an helper class used to sort eigen values
   * \tparam N: space dimension
   */
  template <unsigned short N>
  struct SortEigenValues;
  //! \brief partial specialisation of the `SortEigenValues` in 1D
  template <>
  struct SortEigenValues<1u> {
    //! a simple alias
    using EigenValuesOrdering = stensor_common::EigenValuesOrdering;
    /*!
     * \tparam T: numeric type
     * \param[in] vp0: eigen value
     * \param[in] vp1: eigen value
     * \param[in] vp2: eigen value
     * \param[in] o:   eigen value ordering
     */
    template <typename T>
    static void exe(T&, T&, T&, const EigenValuesOrdering) {}  // end of exe
  };  // end of struct SortEigenValues<1u>
  //! \brief partial specialisation of the `SortEigenValues` in 1D
  template <>
  struct SortEigenValues<2u> {
    //! a simple alias
    using EigenValuesOrdering = stensor_common::EigenValuesOrdering;
    /*!
     * \tparam T: numeric type
     * \param[in] vp0: eigen value
     * \param[in] vp1: eigen value
     * \param[in] vp2: eigen value
     * \param[in] o:   eigen value ordering
     */
    template <typename T>
    static void exe(T& vp0, T& vp1, T&, const EigenValuesOrdering o) {
      if (o == EigenValuesOrdering::ASCENDING) {
        if (vp0 > vp1) {
          std::swap(vp0, vp1);
        }
      } else if (o == EigenValuesOrdering::DESCENDING) {
        if (vp0 < vp1) {
          std::swap(vp0, vp1);
        }
      }
    }  // end of exe
  };   // end of struct SortEigenValues<2u>
  //! \brief partial specialisation of the `SortEigenValues` in 3D
  template <>
  struct SortEigenValues<3u> {
    //! a simple alias
    using EigenValuesOrdering = stensor_common::EigenValuesOrdering;
    /*!
     * \tparam T: numeric type
     * \param[in] vp0: eigen value
     * \param[in] vp1: eigen value
     * \param[in] vp2: eigen value
     * \param[in] o:   eigen value ordering
     */
    template <typename T>
    static void exe(T& vp0, T& vp1, T& vp2, const EigenValuesOrdering o) {
      auto swap_if_greater = [](T& x, T& y) {
        if (x > y) {
          std::swap(x, y);
        }
      };
      auto swap_if_lesser = [](T& x, T& y) {
        if (x < y) {
          std::swap(x, y);
        }
      };
      if (o == EigenValuesOrdering::ASCENDING) {
        swap_if_greater(vp0, vp1);
        swap_if_greater(vp0, vp2);
        swap_if_greater(vp1, vp2);
      } else if (o == EigenValuesOrdering::DESCENDING) {
        swap_if_lesser(vp0, vp1);
        swap_if_lesser(vp0, vp2);
        swap_if_lesser(vp1, vp2);
      }
    }  // end of exe
  };   // end of struct SortEigenValues<3u>

}  // end of namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_INTERNALS_SORTEIGENVALUES_HXX */
