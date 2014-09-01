/*! 
 * \file  include/TFEL/Math/Forward/FunctionConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_FORWARD_FUNCTIONCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_FUNCTIONCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct FunctionConcept;

    template<typename Expr>
    class FunctionExpr;

    template<typename A>
    class FunctionNegExpr;

    template<typename A,typename Func>
    class FctFunctionExpr;

    template<typename A, typename B, class Op>
    class FunctionScalarExpr;

    template<typename A, typename B, class Op>
    class ScalarFunctionExpr;

    template<typename A, typename B,typename Op>
    class FunctionFunctionExpr;

    template<typename Func1,typename Func2>
    class FunctionsCompositionExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_FUNCTIONCONCEPT_H */

