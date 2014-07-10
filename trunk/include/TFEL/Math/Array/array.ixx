/*!
  \file   array.ixx
  \author Helfer Thomas
  \date   02 jun 2006
  
  \brief  
*/

#ifndef _LIB_TFEL_ARRAY_IXX_
#define _LIB_TFEL_ARRAY_IXX_ 

#include<ostream>
      
#include"TFEL/Math/Array/Internals/ArraySerialize.hxx"

namespace tfel{
  
  namespace math {

    namespace internals{
      template<unsigned short N>
      struct ArraySerialize;
    }

    template<unsigned short N,typename T,template<unsigned short,typename>class TStorage>
    std::ostream & operator << (std::ostream & os,const array<N,T,TStorage>& a) 
    {
      tfel::math::internals::ArraySerialize<N>::exe(os,a);
      return os;
    }
      
  } //end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_ARRAY_IXX */

