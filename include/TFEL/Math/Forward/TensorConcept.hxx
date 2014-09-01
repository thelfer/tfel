/*! 
 * \file  include/TFEL/Math/Forward/TensorConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  3 dec. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_FORWARD_TENSORCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_TENSORCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    class TensorConcept;

    template<typename T_type, typename Expr>
    struct TensorExpr;

    template<typename T_type>
    struct TensorTraits;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_TENSORCONCEPT_H */

