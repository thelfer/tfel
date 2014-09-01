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

#endif /* _LIB_TFEL_ARRAY_IXX * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

