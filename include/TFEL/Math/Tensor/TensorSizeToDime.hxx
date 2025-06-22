/*!
 * \file   include/TFEL/Math/Tensor/TensorSizeToDime.hxx
 * \brief  This file implements the TensorDimeToSize and TensorSizeToDime
 * classes. \author Helfer Thomas \date   07 Aug 2006 \copyright Copyright (C)
 * 2006-2018 CEA/DEN, EDF R&D. All rights reserved. This project is publicly
 * released under either the GNU GPL Licence with linking exception or the
 * CECILL-A licence. A copy of thoses licences are delivered with the sources of
 * TFEL. CEA or EDF may also distribute this project under specific licensing
 * conditions.
 */

#ifndef LIB_TFEL_TENSORSIZETODIME_H_
#define LIB_TFEL_TENSORSIZETODIME_H_

namespace tfel {

  namespace math {

    /*!
     * \class TensorDimeToSize
     * \brief Metafunction which returns the number of components of
     * an tensor given the spatial dimension used.
     * The correspondance between these two numbers is given by the
     * following table:
     *
     * | Spatial Dimension | Number of components  |
     * | :---------------: | :-------------------: |
     * | 1                 | 3                     |
     * | 2                 | 5                     |
     * | 3                 | 9                     |
     *
     * \tparam N, the spatial dimension.
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   TensorSizeToDime and include/TFEL/Math/Tensor/TensorSizeToDime.hxx
     * for implementation.
     */
    template <unsigned short N>
    struct TensorDimeToSize;

    /*!
     * \class TensorSizeToDime
     * \brief Metafunction which returns the spatial dimension given
     * the number of components of a tensor.
     * \param N, the number of components of an tensor
     * \param value, the spatial dimension.
     * \pre   This class is only defined for N=3u,5u and 9u.
     * \see   TensorDimeToSize and include/TFEL/Math/Tensor/TensorSizeToDime.hxx
     * for implementation.
     */
    template <unsigned short N>
    struct TensorSizeToDime;

    /*!
     * \brief Partial specialisation for 1D.
     */
    template <>
    struct TensorDimeToSize<1u> {
      static constexpr unsigned short value = 3u;
    };

    /*!
     * \brief Partial specialisation for 2D.
     */
    template <>
    struct TensorDimeToSize<2u> {
      static constexpr unsigned short value = 5u;
    };

    /*!
     * \brief Partial specialisation for 3D.
     */
    template <>
    struct TensorDimeToSize<3u> {
      static constexpr unsigned short value = 9u;
    };

    /*!
     * \brief Partial specialisation for 1D.
     */
    template <>
    struct TensorSizeToDime<3u> {
      static constexpr unsigned short value = 1u;
    };

    /*!
     * \brief Partial specialisation for 2D.
     */
    template <>
    struct TensorSizeToDime<5u> {
      static constexpr unsigned short value = 2u;
    };

    /*!
     * \brief Partial specialisation for 3D.
     */
    template <>
    struct TensorSizeToDime<9u> {
      static constexpr unsigned short value = 3u;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_TENSORSIZETODIME_H_ */
