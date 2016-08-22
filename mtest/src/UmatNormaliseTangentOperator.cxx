/*! 
 * \file  mfront/mtest/UmatNormaliseTangentOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cstdlib>
#include<cmath>
#include"MTest/UmatNormaliseTangentOperator.hxx"

namespace mtest{

  void
  UmatNormaliseTangentOperator::exe(real* const Kt,
				    const tfel::math::matrix<real>& D,
				    const unsigned short d)
  {
    using namespace std;
    using namespace tfel::math;
    using size_type = tfel::math::matrix<real>::size_type;
    static const real sqrt2 = sqrt(real(2));
    // normalise
    if(d==1u){
      // transpose
      for(size_type i=0;i!=3u;++i){
	for(size_type j=0;j!=3u;++j){
	  *(Kt+i*4+j) = D(j,i);
	}
      }
    } else if(d==2u){
      auto access = [&Kt](const size_type i,
			  const size_type j) -> real&{
	return *(Kt+i*4+j);
      };	
      // transpose
      for(size_type i=0;i!=4u;++i){
	for(size_type j=0;j!=4u;++j){
	  access(i,j) = D(j,i);
	}
      }
      access(0,3) *= sqrt2;
      access(1,3) *= sqrt2;
      access(2,3) *= sqrt2;
      access(3,0) *= sqrt2;
      access(3,1) *= sqrt2;
      access(3,2) *= sqrt2;
      access(3,3) *= 2;
    } else if (d==3u){
      auto access = [&Kt](const size_type i,
			  const size_type j) -> real&{
	return *(Kt+i*6+j);
      };	
      // transpose
      for(size_type i=0;i!=6u;++i){
	for(size_type j=0;j!=6u;++j){
	  access(i,j) = D(j,i);
	}
      }
      access(0,3) *= sqrt2;
      access(1,3) *= sqrt2;
      access(2,3) *= sqrt2;
      access(0,4) *= sqrt2;
      access(1,4) *= sqrt2;
      access(2,4) *= sqrt2;
      access(0,5) *= sqrt2;
      access(1,5) *= sqrt2;
      access(2,5) *= sqrt2;
      access(3,0) *= sqrt2;
      access(3,1) *= sqrt2;
      access(3,2) *= sqrt2;
      access(4,0) *= sqrt2;
      access(4,1) *= sqrt2;
      access(4,2) *= sqrt2;
      access(5,0) *= sqrt2;
      access(5,1) *= sqrt2;
      access(5,2) *= sqrt2;
      access(3,3) *= 2;
      access(3,4) *= 2;
      access(3,5) *= 2;
      access(4,3) *= 2;
      access(4,4) *= 2;
      access(4,5) *= 2;
      access(5,3) *= 2;
      access(5,4) *= 2;
      access(5,5) *= 2;
    }
  } // end of UmatNormaliseTangentOperator::exe

} // end of namespace mtest

