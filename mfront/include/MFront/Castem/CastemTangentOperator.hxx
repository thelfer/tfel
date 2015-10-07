/*! 
 * \file  mfront/include/MFront/Castem/CastemTangentOperator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 06 juin 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CASTEMTANGENTOPERATOR_H_
#define LIB_MFRONT_CASTEMTANGENTOPERATOR_H_ 

#include"TFEL/Math/Forward/tmatrix.hxx"
#include"TFEL/Math/Forward/t2tost2.hxx"
#include"TFEL/Math/Forward/st2tost2.hxx"

#include"MFront/Castem/Castem.hxx"
#include"MFront/Castem/CastemConfig.hxx"

namespace castem
{
  
  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  struct MFRONT_CASTEM_VISIBILITY_EXPORT CastemTangentOperator
  {
    /*!
     * \brief normalize the finite strain behaviour tangent operator
     * in 1D
     * \param[in] Dt : finite strain behaviour tangent operator
     */
    TFEL_NORETURN static void
    normalize(tfel::math::t2tost2<1u,CastemReal>&);
    /*!
     * \brief normalize the finite strain behaviour tangent operator
     * in 2D
     * \param[in] Dt : finite strain behaviour tangent operator
     */
    TFEL_NORETURN static void
    normalize(tfel::math::t2tost2<2u,CastemReal>&);
    /*!
     * \brief normalize the finite strain behaviour tangent operator
     * in 3D
     * \param[in] Dt : finite strain behaviour tangent operator
     */
    TFEL_NORETURN static void
    normalize(tfel::math::t2tost2<3u,CastemReal>&);
    /*!
     * \brief transpose the finite strain behaviour tangent operator
     * in 1D
     * \param[in] Dt : finite strain behaviour tangent operator
     * \note this method does nothing
     */
    static void
    transpose(tfel::math::t2tost2<1u,CastemReal>&);
    /*!
     * \brief transpose the finite strain behaviour tangent operator
     * in 2D
     * \param[in] Dt : finite strain behaviour tangent operator
     * \note this method does nothing
     */
    static void
    transpose(tfel::math::t2tost2<2u,CastemReal>&);
    /*!
     * \brief transpose the finite strain behaviour tangent operator
     * in 3D
     * \param[in] Dt : finite strain behaviour tangent operator
     * \note this method does nothing
     */
    static void
    transpose(tfel::math::t2tost2<3u,CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<1u,CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<2u,CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<3u,CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<1u,CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<2u,CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<3u,CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::tmatrix<1u,1u,CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::tmatrix<2u,2u,CastemReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::tmatrix<3u,3u,CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::tmatrix<1u,1u,CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::tmatrix<2u,2u,CastemReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::tmatrix<3u,3u,CastemReal>&);
  }; // end of struct CastemTangentOperator

} // end of namespace castem


#endif /* LIB_MFRONT_CASTEMTANGENTOPERATOR_H_ */

