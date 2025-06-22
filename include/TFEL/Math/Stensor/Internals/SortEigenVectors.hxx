/*!
 * \file   SortEigenVectors.hxx
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

#ifndef LIB_TFEL_MATH_INTERNALS_SORTEIGENVECTORS_HXX
#define LIB_TFEL_MATH_INTERNALS_SORTEIGENVECTORS_HXX

namespace tfel {

  namespace math {

    namespace internals {

      /*!
       * \brief an helper class used to sort eigen values
       * \tparam N: space dimension
       */
      template <unsigned short N>
      struct SortEigenVectors;

      //! \brief partial specialisation of the `SortEigenVectors` in 1D
      template <>
      struct SortEigenVectors<1u> {
        //! a simple alias
        using EigenValuesOrdering = stensor_common::EigenValuesOrdering;
        /*!
         * \tparam T: numeric type
         * \param[in] vp: eigen values
         * \param[in] m:  eigen vectors
         * \param[in] o:   eigen value ordering
         */
        template <typename T>
        static void exe(tfel::math::tvector<3u, T>&,
                        tfel::math::tmatrix<3u, 3u, T>&,
                        const EigenValuesOrdering) {}  // end of exe
      };  // end of struct SortEigenVectors<1u>
      //! \brief partial specialisation of the `SortEigenVectors` in 1D
      template <>
      struct SortEigenVectors<2u> {
        //! a simple alias
        using EigenValuesOrdering = stensor_common::EigenValuesOrdering;
        /*!
         * \tparam T: numeric type
         * \param[in] vp: eigen values
         * \param[in] m:  eigen vectors
         * \param[in] o:   eigen value ordering
         */
        template <typename T>
        static void exe(tfel::math::tvector<3u, T>& vp,
                        tfel::math::tmatrix<3u, 3u, T>& m,
                        const EigenValuesOrdering o) {
          if (o == EigenValuesOrdering::ASCENDING) {
            if (vp(1) < vp(0)) {
              std::swap(vp(0), vp(1));
              std::swap(m(0, 0), m(0, 1));
              std::swap(m(1, 0), m(1, 1));
            }
          } else if (o == EigenValuesOrdering::DESCENDING) {
            if (vp(1) > vp(0)) {
              std::swap(vp(0), vp(1));
              std::swap(m(0, 0), m(0, 1));
              std::swap(m(1, 0), m(1, 1));
            }
          }
        }  // end of exe
      };   // end of struct SortEigenVectors<2u>
      //! \brief partial specialisation of the `SortEigenVectors` in 3D
      template <>
      struct SortEigenVectors<3u> {
        //! a simple alias
        using EigenValuesOrdering = stensor_common::EigenValuesOrdering;
        /*!
         * \tparam T: numeric type
         * \param[in] vp: eigen values
         * \param[in] m:  eigen vectors
         * \param[in] o:   eigen value ordering
         */
        template <typename T>
        static void exe(tfel::math::tvector<3u, T>& vp,
                        tfel::math::tmatrix<3u, 3u, T>& m,
                        const EigenValuesOrdering o) {
          using size_type = decltype(vp.size());
          size_type zero = 0;
          size_type one = 1;
          size_type two = 2;
          auto m2 = m;
          auto vp2 = vp;
          tfel::math::tvector<3u, decltype(vp.size())> idx = {zero, one, two};
          if (o == EigenValuesOrdering::ASCENDING) {
            if ((vp(0) < vp(1)) && (vp(1) < vp(2))) {
              idx = {zero, one, two};
            } else if ((vp(0) < vp(2)) && (vp(2) < vp(1))) {
              idx = {zero, two, one};
            } else if ((vp(1) < vp(0)) && (vp(0) < vp(2))) {
              idx = {one, zero, two};
            } else if ((vp(1) < vp(2)) && (vp(2) < vp(0))) {
              idx = {one, two, zero};
            } else if ((vp(2) < vp(0)) && (vp(0) < vp(1))) {
              idx = {two, zero, one};
            } else {
              // ((vp(2)<vp(1))&&(vp(1)<vp(0))){
              idx = {two, one, zero};
            }
          } else if (o == EigenValuesOrdering::DESCENDING) {
            if ((vp(0) > vp(1)) && (vp(1) > vp(2))) {
              idx = {zero, one, two};
            } else if ((vp(0) > vp(2)) && (vp(2) > vp(1))) {
              idx = {zero, two, one};
            } else if ((vp(1) > vp(0)) && (vp(0) > vp(2))) {
              idx = {one, zero, two};
            } else if ((vp(1) > vp(2)) && (vp(2) > vp(0))) {
              idx = {one, two, zero};
            } else if ((vp(2) > vp(0)) && (vp(0) > vp(1))) {
              idx = {two, zero, one};
            } else {
              // ((vp(2)>vp(1))&&(vp(1)>vp(0))){
              idx = {two, one, zero};
            }
          }
          vp = {vp2(idx(0)), vp2(idx(1)), vp2(idx(2))};
          m = {m2(0, idx(0)), m2(0, idx(1)), m2(0, idx(2)),
               m2(1, idx(0)), m2(1, idx(1)), m2(1, idx(2)),
               m2(2, idx(0)), m2(2, idx(1)), m2(2, idx(2))};
        }  // end of exe
      };   // end of struct SortEigenVectors<3u>

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_SORTEIGENVECTORS_HXX */
