/*! 
 * \file  include/TFEL/Math/Forward/ExpressionTemplates.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juil. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

