/*!
 * \file  mfront/include/MFront/ZMAT/ZMATInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 26 mai 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ZMAT_ZMATINTERFACE_HXX
#define LIB_MFRONT_ZMAT_ZMATINTERFACE_HXX

#include <type_traits>

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

#include <Matrix.h>
#include "MFront/ZMAT/ZMATUndefs.hxx"

namespace zmat {

  /*!
   * conversion from TFEL to ZMAT and back
   */
  struct ZMATInterface {
    /*!
     * convert an 1D ZMAT symmetric tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(tfel::math::stensor<1u, double>&, const double* const);
    /*!
     * convert an 2D ZMAT symmetric tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(tfel::math::stensor<2u, double>&, const double* const);
    /*!
     * convert an 3D ZMAT symmetric tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(tfel::math::stensor<3u, double>&, const double* const);
    /*!
     * convert an 1D TFEL symmetric tensor to
     * a ZMAT one \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(double* const, const tfel::math::stensor<1u, double>&);
    /*!
     * convert an 2D TFEL symmetric tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(double* const, const tfel::math::stensor<2u, double>&);
    /*!
     * convert an 3D TFEL symmetric tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(double* const, const tfel::math::stensor<3u, double>&);
    /*!
     * convert an 1D ZMAT  tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(tfel::math::tensor<1u, double>&, const double* const);
    /*!
     * convert an 2D ZMAT  tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(tfel::math::tensor<2u, double>&, const double* const);
    /*!
     * convert an 3D ZMAT  tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(tfel::math::tensor<3u, double>&, const double* const);
    /*!
     * convert an 1D TFEL  tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(double* const, const tfel::math::tensor<1u, double>&);
    /*!
     * convert an 2D TFEL  tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(double* const, const tfel::math::tensor<2u, double>&);
    /*!
     * convert an 3D TFEL  tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(double* const, const tfel::math::tensor<3u, double>&);
    /*!
     * convert an 1D TFEL stiffness tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(ZSET::MATRIX&, const tfel::math::st2tost2<1u, double>&);
    /*!
     * convert an 2D TFEL stiffness tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(ZSET::MATRIX&, const tfel::math::st2tost2<2u, double>&);
    /*!
     * convert an 3D TFEL stiffness tensor to a ZMAT one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(ZSET::MATRIX&, const tfel::math::st2tost2<3u, double>&);
    /*!
     * convert an 1D ZMAT stiffness tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(tfel::math::st2tost2<1u, double>&, const ZSET::MATRIX&);
    /*!
     * convert a 2D ZMAT stiffness tensor to a TFEL one
     * \param[in] src  : source
     * \param[in] dest : destination
     */
    static void convert(tfel::math::st2tost2<2u, double>&, const ZSET::MATRIX&);
    /*!
     * \brief convert an ZSET stiffness tensor to a TFEL one
     * \param[in] src:  source
     * \param[in] dest: destination
     */
    static void convert(tfel::math::st2tost2<3u, double>&, const ZSET::MATRIX&);
  };

}  // end of namespace zmat

#include "MFront/ZMAT/ZMATInterface.ixx"

#endif /* LIB_MFRONT_ZMAT_ZMATINTERFACE_HXX */
