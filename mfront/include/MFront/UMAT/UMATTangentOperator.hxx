/*! 
 * \file  UMATTangentOperator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 06 juin 2013
 */

#ifndef _LIB_MFRONT_UMATTANGENTOPERATOR_H_
#define _LIB_MFRONT_UMATTANGENTOPERATOR_H_ 

#include"TFEL/Math/Forward/st2tost2.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATConfig.hxx"

namespace umat
{
  
  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATTangentOperator
  {
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<1u,UMATReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<2u,UMATReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<3u,UMATReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<1u,UMATReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<2u,UMATReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<3u,UMATReal>&);
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::tmatrix<1u,1u,UMATReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::tmatrix<2u,2u,UMATReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::tmatrix<3u,3u,UMATReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::tmatrix<1u,1u,UMATReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::tmatrix<2u,2u,UMATReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::tmatrix<3u,3u,UMATReal>&);
  }; // end of struct UMATTangentOperator

} // end of namespace umat


#endif /* _LIB_MFRONT_UMATTANGENTOPERATOR_H */

