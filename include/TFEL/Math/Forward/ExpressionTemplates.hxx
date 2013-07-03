/*! 
 * \file  ExpressionTemplates.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juil. 2013
 */

#ifndef _LIB_TFEL_MATH_FORWARD_EXPRESSIONTEMPLATES_H_
#define _LIB_TFEL_MATH_FORWARD_EXPRESSIONTEMPLATES_H_ 

namespace tfel
{

  namespace math
  {

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A>
    class MathObjectNegExpr;
    
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A>
    class MathObjectNegExprWithoutConstIterator;

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A,typename Func>
    class FctMathObjectExpr;

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A,typename Func>
    class FctMathObjectExprWithoutConstIterator;

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class MathObjectMathObjectExpr;

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class MathObjectMathObjectExprWithoutConstIterator;
    
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class ScalarMathObjectExpr;

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class ScalarMathObjectExprWithoutConstIterator;

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class MathObjectScalarExpr;

    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B, class Op>
    class MathObjectScalarExprWithoutConstIterator;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_EXPRESSIONTEMPLATES_H */

