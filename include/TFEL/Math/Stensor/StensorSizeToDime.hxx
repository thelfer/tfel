/*!
 * \file   include/TFEL/Math/Stensor/StensorSizeToDime.hxx
 * \brief  This file implements the StensorDimeToSize and StensorSizeToDime
 * classes. \author Thomas Helfer \date   07 Aug 2006 \copyright Copyright (C)
 * 2006-2018 CEA/DEN, EDF R&D. All rights reserved. This project is publicly
 * released under either the GNU GPL Licence or the CECILL-A licence. A copy of
 * thoses licences are delivered with the sources of TFEL. CEA or EDF may also
 * distribute this project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORSIZETODIME_HXX
#define LIB_TFEL_MATH_STENSORSIZETODIME_HXX

namespace tfel::math {

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

#endif /* LIB_TFEL_MATH_STENSORSIZETODIME_HXX */
