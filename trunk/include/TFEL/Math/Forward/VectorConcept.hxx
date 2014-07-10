/*! 
 * \file  VectorConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_VECTORCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_VECTORCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct VectorConcept;

    template<typename T_type, typename Expr>
    class VectorExpr;

    template<unsigned short N,
	     unsigned short M,
	     typename A, typename B>
    class TMatrixTVectorExpr;

    template<unsigned short N,
	     unsigned short M,
	     typename A, typename B>
    class TVectorTMatrixExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_VECTORCONCEPT_H */

