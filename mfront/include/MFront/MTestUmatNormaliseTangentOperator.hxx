/*! 
 * \file  mfront/include/MFront/MTestUmatNormaliseTangentOperator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTUMATNORMALISETANGENTOPERATOR_H_
#define _LIB_MFRONT_MTESTUMATNORMALISETANGENTOPERATOR_H_ 

#include"TFEL/Math/matrix.hxx"

#include"MFront/MTestTypes.hxx"

namespace mfront
{
  /*!
   *
   */
  struct MTestUmatNormaliseTangentOperator
  {
    /*!
     * \param[out] K : normalised matrix
     * \param[in]  D : tangent operator
     * \param[in]  d : dimension
     */
    static void
    exe(tfel::math::matrix<real>&,
	const tfel::math::matrix<real>&,
	const unsigned short);
  };

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTUMATNORMALISETANGENTOPERATOR_H */

