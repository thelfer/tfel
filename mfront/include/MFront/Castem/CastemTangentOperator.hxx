/*!
 * \file  mfront/include/MFront/Castem/CastemTangentOperator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 06 juin 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMTANGENTOPERATOR_H_
#define LIB_MFRONT_CASTEMTANGENTOPERATOR_H_

#include "TFEL/Math/Matrix/TMatrixView.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"

#include "MFront/Castem/Castem.hxx"
#include "MFront/Castem/CastemConfig.hxx"

namespace castem {

  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemTangentOperator {
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<1u, CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<2u, CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<3u, CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<1u, CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<2u, CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<3u, CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::TMatrixView<1u, 1u, CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::TMatrixView<2u, 2u, CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::TMatrixView<3u, 3u, CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::TMatrixView<1u, 1u, CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::TMatrixView<2u, 2u, CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::TMatrixView<3u, 3u, CastemReal>&);
  };  // end of struct CastemTangentOperator

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEMTANGENTOPERATOR_H_ */
