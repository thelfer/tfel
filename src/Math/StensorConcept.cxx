/*!
 * \file   StensorConcept.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 janv. 2016
 */

#include<stdexcept>
#include"TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math{

    unsigned short
    getStensorSize(const unsigned short d){
      if(d==1u){
	return 3u;
      }
      if(d==2u){
	return 4u;
      }
      if(d==3u){
	return 6u;
      }
      throw(std::runtime_error("getStensorSize: "
			       "invalid dimension"));
    }

  } // end of namespace math

} // end of namespace tfel
