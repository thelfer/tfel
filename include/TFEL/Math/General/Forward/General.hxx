/*! 
 * \file  include/TFEL/Math/General/Forward/General.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_GENERAL_FORWARD_GENERAL_H_
#define _LIB_TFEL_MATH_GENERAL_FORWARD_GENERAL_H_ 

namespace tfel
{

  namespace math
  {

    template<typename A>
    class NegObjectRandomAccessConstIterator;						    

    template<typename A,typename Fct>
    class FctObjectRandomAccessConstIterator;						    

    template<typename A,
	     typename B,
	     typename Op>
    class ScalarObjectRandomAccessConstIterator;

    template<typename A,
	     typename B,
	     typename Op>
    class ObjectScalarRandomAccessConstIterator;

    template<typename A,typename B,typename Op>
    class ObjectObjectRandomAccessConstIterator;

  } // end of namespace math
  
} // end of namespace tfel


#endif /* _LIB_TFEL_MATH_GENERAL_FORWARD_GENERAL_H */

