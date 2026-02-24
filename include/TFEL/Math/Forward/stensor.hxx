/*!
 * \file  include/TFEL/Math/Forward/stensor.hxx
 * \brief This file introduces some forward declaration relative to
 * the stensor class.
 * \author Thomas Helfer
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_STENSOR_HXX
#define LIB_TFEL_MATH_FORWARD_STENSOR_HXX

#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  /*!
   * \class stensor
   * \brief symmetric tensor.
   * \tparam N: the spatial dimension, see StensorDimeToSize for details.
   * \tparam T: numerical type used, by default, double
   * \pre   This class is only defined for N=1u,2u and 3u.
   * \see   StensorDimeToSize and StensorSizeToDime.
   */
  template <unsigned short N, typename T = double>
  struct [[nodiscard]] stensor;

  /*!
   * \brief partial specialisation of the `MathObjectTraits` class.
   * \tparam N: space dimension
   * \tparam T: numeric type
   */
  template <unsigned short N, typename T>
  struct MathObjectTraits<stensor<N, T>>
      : MathObjectTraitsBase<T, unsigned short> {
    //! \brief space dimension
    static constexpr unsigned short dime = N;
  };  // end of MathObjectTraits

  /*!
   * \class StensorDimeToSize
   * \brief Metafunction which returns the number of components of
   * an stensor given the spatial dimension used.
   * The correspondance between these two numbers is given by the
   * following table:
   *
   * | Spatial Dimension | Number of components  |
   * | :---------------: | :-------------------: |
   * | 1                 | 3                     |
   * | 2                 | 4                     |
   * | 3                 | 6                     |
   *
   * \tparam N : the spatial dimension.
   */
  template <unsigned short N>
  requires((N == 1u) || (N == 2u) || (N == 3u)) struct StensorDimeToSize {
    static constexpr auto value = []() constexpr -> unsigned short {
      constexpr unsigned short values[4] = {0u, 3u, 4u, 6u};
      return values[N];
    }
    ();
  };

  /*!
   * \class StensorSizeToDime
   * \brief Metafunction which returns the spatial dimension given
   * the number of components of a stensor.
   * \param N, the number of components of an stensor
   * \param value, the spatial dimension.
   */
  template <unsigned short N>
  requires((N == 3u) || (N == 4u) || (N == 6u)) struct StensorSizeToDime {
    static constexpr auto value = []() constexpr -> unsigned short {
      constexpr unsigned short values[7] = {0u, 0u, 0u, 1u, 2u, 0u, 3u};
      return values[N];
    }
    ();
  };

  /*!
   * \brief a small helper function around `StensorDimeToSize`
   */
  template <unsigned short N>
  TFEL_HOST_DEVICE constexpr unsigned short getStensorSize() {
    return StensorDimeToSize<N>::value;
  }  // end of getStensorSize

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_STENSOR_HXX */
