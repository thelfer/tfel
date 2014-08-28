/*! 
 * \file  include/TFEL/Math/Bindings/atlas.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_ATLAS_H_
#define _LIB_TFEL_MATH_ATLAS_H_ 

#include<stdexcept>

extern "C" {
#include<clapack.h>
}

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"

namespace tfel
{

  namespace math
  {

    namespace atlas{
      
      void gesv(tfel::math::matrix<double>& A,
		tfel::math::vector<double>& b)
      {
	using namespace std;
	using namespace tfel::math;
	using tfel::math::vector;
	vector<int> pv(b.size(),0);
	if(clapack_dgesv(CblasRowMajor,b.size(),1,&A(0,0),b.size(),&pv[0],&b(0),b.size())!=0){
	  throw(runtime_error("gesv failed"));
	}
      }

    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_ATLAS_H */

