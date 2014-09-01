/*!
 * \file   include/TFEL/Math/Array/Internals/ArraySerialize.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   22 Aug 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_ARRAYSERIALIZE_H_
#define _LIB_TFEL_ARRAYSERIALIZE_H_ 

#include<ostream>

#include"TFEL/Math/array.hxx"

namespace tfel{
  
  namespace math {

    namespace internals{
      
      template<unsigned short N>
      struct ArraySerialize;
      
      template<>
      struct ArraySerialize<1u>
      {
	template<typename T,template<unsigned short,typename>class TStorage>
	static void exe(std::ostream& os,const array<1u,T,TStorage>& a)
	{
	  unsigned int i;
	  os << "[ ";
	  for(i=0;i<a.getSize();++i){
	    os << a(i) << " ";
	  }
	  os << "]";
	}
      };
      
      template<>
      struct ArraySerialize<2u>
      {
	template<typename T,template<unsigned short,typename>class TStorage>
	static void exe(std::ostream& os,const array<2u,T,TStorage>& a)
	{
	  unsigned int i,j;
	  os << "[";
	  for(i=0;i<a.getNbRows();++i){
	    os << "[ ";
	    for(j=0;j<a.getNbCols();++j){
	      os << a(i,j) << " ";
	    }
	    os << "]";
	  }
	  os << "]";
	}
      };

      template<>
      struct ArraySerialize<3u>
      {
	template<typename T,template<unsigned short,typename>class TStorage>
	static void exe(std::ostream& os,const array<3u,T,TStorage>& a)
	{
	  unsigned int i,j,k;
	  os << "[";
	  for(i=0;i<a.getNbRows();++i){
	    os << "[";
	    for(j=0;j<a.getNbCols();++j){
	      os << "[ ";
	      for(k=0;k<a.getNbSlices();++k){
		os << a(i,j,k) << " ";
	      }
	      os << "]";
	    }
	    os << "]";
	  }
	  os << "]";
	}
      };

    }// end of namespace internals

  } //end of namespace math
  
} // end of namespace tfel


#endif /* _LIB_TFEL_ARRAYSERIALIZE_H */

