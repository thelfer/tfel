/*! 
 * \file  include/TFEL/Math/Forward/Function.hxx
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

#ifndef _LIB_TFEL_MATH_FORWARD_FUNCTION_H_
#define _LIB_TFEL_MATH_FORWARD_FUNCTION_H_ 

namespace tfel{

  namespace math{

    template<typename Func>
    struct FunctionConcept;

    template<typename Expr>
    class FunctionExpr;

    template<short N,unsigned short D>
    struct Cst;

    template<typename Func1,typename Func2, typename Op>
    class FunctionFunctionExpr;

    template<typename Func1,typename Func2, typename Op>
    class FunctionFunctionExpr;

    template<typename F>
    class FunctionNegExpr;

    template<typename Func,typename Scal, typename Op>
    class FunctionScalarExpr;

    template<typename Func1,typename Func2>
    class FunctionsCompositionExpr;

    template<typename Scal,typename Func, typename Op>
    class ScalarFunctionExpr;

  } // end of namespace typetraits
    
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_FUNCTION_H */

