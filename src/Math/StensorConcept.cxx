/*!
 * \file   StensorConcept.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 janv. 2016
 */

#include"TFEL/Raise.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math{

    unsigned short getStensorSize(const unsigned short d){
      if(d==1u){
	return 3u;
      }
      if(d==2u){
	return 4u;
      }
      if(d==3u){
	return 6u;
      }
      raise("getStensorSize: invalid dimension");
    }

  } // end of namespace math

} // end of namespace tfel
