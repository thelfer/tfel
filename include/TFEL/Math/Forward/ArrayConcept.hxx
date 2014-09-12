/*! 
 * \file  include/TFEL/Math/Forward/ArrayConcept.hxx
 * \brief This file introduces some forward declaration relative to
 * the ArrayConcept class.
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_FORWARD_ARRAYCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_ARRAYCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<typename T,
	     typename Ordering,
	     typename TAllocator>
    class ArrayStorage_;

    template<unsigned short N,
	     typename T,
	     typename Ordering,
	     typename TAllocator>
    struct ArrayStorage;

    template<class T>
    struct ArrayConcept;

    template<typename T_type, typename Expr>
    class ArrayExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_ARRAYCONCEPT_H */

