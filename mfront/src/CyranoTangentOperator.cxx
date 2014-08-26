/*! 
 * \file  CyranoTangentOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/st2tost2.hxx"
#include"MFront/Cyrano/CyranoTangentOperator.hxx"

namespace cyrano
{

  void
  CyranoTangentOperator::transpose(tfel::math::st2tost2<1u,CyranoReal>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(1,2),Dt(2,1));
  }
  
} // end of namespace cyrano
