/*! 
 * \file  CyranoTangentOperator.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANOTANGENTOPERATOR_H_
#define _LIB_MFRONT_CYRANOTANGENTOPERATOR_H_ 

#include"TFEL/Math/Forward/st2tost2.hxx"

#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/Cyrano/CyranoConfig.hxx"

namespace cyrano
{
  
  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the cyrano interface
   */
  struct MFRONT_CYRANO_VISIBILITY_EXPORT CyranoTangentOperator
  {
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<1u,CyranoReal>&);
  }; // end of struct CyranoTangentOperator

} // end of namespace cyrano

#endif /* _LIB_MFRONT_CYRANOTANGENTOPERATOR_H */

