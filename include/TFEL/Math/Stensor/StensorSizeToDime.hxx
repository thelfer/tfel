/*!
 * \file   include/TFEL/Math/Stensor/StensorSizeToDime.hxx
 * \brief  This file implements the StensorDimeToSize and StensorSizeToDime
 * classes. \author Thomas Helfer \date   07 Aug 2006 \copyright Copyright (C)
 * 2006-2018 CEA/DEN, EDF R&D. All rights reserved. This project is publicly
 * released under either the GNU GPL Licence with linking exception or the
 * CECILL-A licence. A copy of thoses licences are delivered with the sources of
 * TFEL. CEA or EDF may also distribute this project under specific licensing
 * conditions.
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
   * \pre   This class is only defined for N=1u,2u and 3u.
   * \see   StensorSizeToDime and
   * include/TFEL/Math/Stensor/StensorSizeToDime.hxx for implementation.
   */
  template <unsigned short N>
  struct StensorDimeToSize;

  /*!
   * \class StensorSizeToDime
   * \brief Metafunction which returns the spatial dimension given
   * the number of components of a stensor.
   * \param N, the number of components of an stensor
   * \param value, the spatial dimension.
   * \pre   This class is only defined for N=3u,4u and 6u.
   * \see   StensorDimeToSize and
   * include/TFEL/Math/Stensor/StensorSizeToDime.hxx for implementation.
   */
  template <unsigned short N>
  struct StensorSizeToDime;

  //! \brief partial specialisation for 1D.
  template <>
  struct StensorDimeToSize<1u> {
    static constexpr unsigned short value = 3u;
  };

  //! \brief partial specialisation for 2D.
  template <>
  struct StensorDimeToSize<2u> {
    static constexpr unsigned short value = 4u;
  };

  //! \brief partial specialisation for 3D.
  template <>
  struct StensorDimeToSize<3u> {
    static constexpr unsigned short value = 6u;
  };

  //! \brief partial specialisation for 1D.
  template <>
  struct StensorSizeToDime<3u> {
    static constexpr unsigned short value = 1u;
  };

  //! \brief partial specialisation for 2D.
  template <>
  struct StensorSizeToDime<4u> {
    static constexpr unsigned short value = 2u;
  };

  //! \brief partial specialisation for 3D.
  template <>
  struct StensorSizeToDime<6u> {
    static constexpr unsigned short value = 3u;
  };

  /*!
   * \brief a small helper function around `StensorDimeToSize`
   */
  template <unsigned short N>
  constexpr unsigned short getStensorSize() {
    return StensorDimeToSize<N>::value;
  }  // end of getStensorSize

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSORSIZETODIME_HXX */
