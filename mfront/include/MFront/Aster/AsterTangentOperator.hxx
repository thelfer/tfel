/*!
 * \file  mfront/include/MFront/Aster/AsterTangentOperator.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTER_ASTERTANGENTOPERATOR_HXX
#define LIB_MFRONT_ASTER_ASTERTANGENTOPERATOR_HXX

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"

#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterConfig.hxx"

namespace aster {

  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterTangentOperator {
    /*!
     * \brief normalize the finite strain behaviour tangent operator
     * in 1D
     * \param[in] Dt : finite strain behaviour tangent operator
     */
    static void normalize(tfel::math::T2toST2View<1u, AsterReal>&);
    /*!
     * \brief normalize the finite strain behaviour tangent operator
     * in 2D
     * \param[in] Dt : finite strain behaviour tangent operator
     */
    static void normalize(tfel::math::T2toST2View<2u, AsterReal>&);
    /*!
     * \brief normalize the finite strain behaviour tangent operator
     * in 3D
     * \param[in] Dt : finite strain behaviour tangent operator
     */
    static void normalize(tfel::math::T2toST2View<3u, AsterReal>&);
    /*!
     * \brief transpose the finite strain behaviour tangent operator
     * in 1D
     * \param[in] Dt : finite strain behaviour tangent operator
     * \note this method does nothing
     */
    static void transpose(tfel::math::T2toST2View<1u, AsterReal>&);
    /*!
     * \brief transpose the finite strain behaviour tangent operator
     * in 2D
     * \param[in] Dt : finite strain behaviour tangent operator
     * \note this method does nothing
     */
    static void transpose(tfel::math::T2toST2View<2u, AsterReal>&);
    /*!
     * \brief transpose the finite strain behaviour tangent operator
     * in 3D
     * \param[in] Dt : finite strain behaviour tangent operator
     * \note this method does nothing
     */
    static void transpose(tfel::math::T2toST2View<3u, AsterReal>&);
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<1u, AsterReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<2u, AsterReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<3u, AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<1u, AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<2u, AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<3u, AsterReal>&);
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::TMatrixView<1u, 1u, AsterReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::TMatrixView<2u, 2u, AsterReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::TMatrixView<3u, 3u, AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::TMatrixView<1u, 1u, AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::TMatrixView<2u, 2u, AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::TMatrixView<3u, 3u, AsterReal>&);
  };  // end of struct AsterTangentOperator

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTER_ASTERTANGENTOPERATOR_HXX */
