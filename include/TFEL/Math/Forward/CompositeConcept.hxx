/*! 
 * \file  include/TFEL/Math/Forward/CompositeConcept.hxx
 * \brief This file introduces some forward declaration relative to
 * the CompositeConcept class.
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_FORWARD_COMPOSITECONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_COMPOSITECONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct CompositeConcept;

    template<typename T_type, typename Expr>
    struct CompositeExpr;

    template<typename A,typename Func>
    class FctCompositeExpr;

    template<typename A>
    class CompositeNegExpr;

    template<typename A, typename B, class Op>
    class CompositeScalarExpr;

    template<typename A, typename B, class Op>
    class ScalarCompositeExpr;

    template<typename A, typename B,typename Op>
    class CompositeCompositeExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_COMPOSITECONCEPT_H */

