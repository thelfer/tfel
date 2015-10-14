/*! 
 * \file  mfront/src/AbaqusTangentOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 févr. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/t2tost2.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"MFront/Abaqus/AbaqusTangentOperator.hxx"

namespace abaqus
{

  void
  AbaqusTangentOperator::normalize(tfel::math::ST2toST2View<1u,AbaqusReal>&)
  {} // end of AbaqusTangentOperator::normalize

  void
  AbaqusTangentOperator::normalize(tfel::math::ST2toST2View<2u,AbaqusReal>& Dt)
  {
    using namespace std;
    static const AbaqusReal inv_sqrt2 = AbaqusReal(1)/std::sqrt(AbaqusReal(2));
    Dt(0,3) *= inv_sqrt2;
    Dt(1,3) *= inv_sqrt2;
    Dt(2,3) *= inv_sqrt2;
    Dt(3,0) *= inv_sqrt2;
    Dt(3,1) *= inv_sqrt2;
    Dt(3,2) *= inv_sqrt2;
    Dt(3,3) *= 0.5;
  } // end of AbaqusTangentOperator::normalize
  
  void
  AbaqusTangentOperator::normalize(tfel::math::ST2toST2View<3u,AbaqusReal>& Dt)
  {
    using namespace std;
    static const AbaqusReal inv_sqrt2 = AbaqusReal(1)/std::sqrt(AbaqusReal(2));
    Dt(0,3) *= inv_sqrt2;
    Dt(1,3) *= inv_sqrt2;
    Dt(2,3) *= inv_sqrt2;
    Dt(0,4) *= inv_sqrt2;
    Dt(1,4) *= inv_sqrt2;
    Dt(2,4) *= inv_sqrt2;
    Dt(0,5) *= inv_sqrt2;
    Dt(1,5) *= inv_sqrt2;
    Dt(2,5) *= inv_sqrt2;
    Dt(3,0) *= inv_sqrt2;
    Dt(3,1) *= inv_sqrt2;
    Dt(3,2) *= inv_sqrt2;
    Dt(4,0) *= inv_sqrt2;
    Dt(4,1) *= inv_sqrt2;
    Dt(4,2) *= inv_sqrt2;
    Dt(5,0) *= inv_sqrt2;
    Dt(5,1) *= inv_sqrt2;
    Dt(5,2) *= inv_sqrt2;
    Dt(3,3) *= 0.5;
    Dt(3,4) *= 0.5;
    Dt(3,5) *= 0.5;
    Dt(4,3) *= 0.5;
    Dt(4,4) *= 0.5;
    Dt(4,5) *= 0.5;
    Dt(5,3) *= 0.5;
    Dt(5,4) *= 0.5;
    Dt(5,5) *= 0.5;
  } // end of struct AbaqusTangentOperator

  void
  AbaqusTangentOperator::transpose(tfel::math::ST2toST2View<1u,AbaqusReal>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(1,2),Dt(2,1));
  }

  void
  AbaqusTangentOperator::transpose(tfel::math::ST2toST2View<2u,AbaqusReal>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(0,3),Dt(3,0));
    std::swap(Dt(1,2),Dt(2,1));
    std::swap(Dt(1,3),Dt(3,1));
    std::swap(Dt(2,3),Dt(3,2));
  }

  void
  AbaqusTangentOperator::transpose(tfel::math::ST2toST2View<3u,AbaqusReal>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(0,3),Dt(3,0));
    std::swap(Dt(0,4),Dt(4,0));
    std::swap(Dt(0,5),Dt(5,0));
    std::swap(Dt(1,2),Dt(2,1));
    std::swap(Dt(1,3),Dt(3,1));
    std::swap(Dt(1,4),Dt(4,1));
    std::swap(Dt(1,5),Dt(5,1));
    std::swap(Dt(2,3),Dt(3,2));
    std::swap(Dt(2,4),Dt(4,2));
    std::swap(Dt(2,5),Dt(5,2));
    std::swap(Dt(3,4),Dt(4,3));
    std::swap(Dt(3,5),Dt(5,3));
    std::swap(Dt(4,5),Dt(5,4));
  }
  
} // end of namespace abaqus
