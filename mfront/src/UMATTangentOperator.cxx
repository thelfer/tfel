/*! 
 * \file  UMATTangentOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 févr. 2013
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/t2tost2.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"MFront/UMAT/UMATTangentOperator.hxx"


namespace umat
{

  void
  UMATTangentOperator::normalize(tfel::math::t2tost2<1u,UMATReal>&)
  {
    using namespace std;
    string msg("UMATTangentOperator::normalize : "
	       "unsupported feature");
    throw(runtime_error(msg));
  } // end of UMATTangentOperator::normalize
  
  void
  UMATTangentOperator::normalize(tfel::math::t2tost2<2u,UMATReal>&)
  {
    using namespace std;
    string msg("UMATTangentOperator::normalize : "
	       "unsupported feature");
    throw(runtime_error(msg));
  } // end of UMATTangentOperator::normalize
  
  void
  UMATTangentOperator::normalize(tfel::math::t2tost2<3u,UMATReal>&)
  {
    using namespace std;
    string msg("UMATTangentOperator::normalize : "
	       "unsupported feature");
    throw(runtime_error(msg));
  } // end of struct UMATTangentOperator

  void
  UMATTangentOperator::transpose(tfel::math::t2tost2<1u,UMATReal>&)
  {}

  void
  UMATTangentOperator::transpose(tfel::math::t2tost2<2u,UMATReal>&)
  {}

  void
  UMATTangentOperator::transpose(tfel::math::t2tost2<3u,UMATReal>&)
  {}

  void
  UMATTangentOperator::normalize(tfel::math::st2tost2<1u,UMATReal>&)
  {} // end of UMATTangentOperator::normalize

  void
  UMATTangentOperator::normalize(tfel::math::st2tost2<2u,UMATReal>& Dt)
  {
    using namespace std;
    static const UMATReal inv_sqrt2 = 1./std::sqrt(UMATReal(2));
    Dt(0,3) *= inv_sqrt2;
    Dt(1,3) *= inv_sqrt2;
    Dt(2,3) *= inv_sqrt2;
    Dt(3,0) *= inv_sqrt2;
    Dt(3,1) *= inv_sqrt2;
    Dt(3,2) *= inv_sqrt2;
    Dt(3,3) *= 0.5;
  } // end of UMATTangentOperator::normalize
  
  /*!
   * \brief normalize the tangent operator in 3D
   */
  void
  UMATTangentOperator::normalize(tfel::math::st2tost2<3u,UMATReal>& Dt)
  {
    using namespace std;
    static const UMATReal inv_sqrt2 = 1./std::sqrt(UMATReal(2));
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
  } // end of struct UMATTangentOperator

  void
  UMATTangentOperator::transpose(tfel::math::st2tost2<1u,UMATReal>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(1,2),Dt(2,1));
  }

  void
  UMATTangentOperator::transpose(tfel::math::st2tost2<2u,UMATReal>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(0,3),Dt(3,0));
    std::swap(Dt(1,2),Dt(2,1));
    std::swap(Dt(1,3),Dt(3,1));
    std::swap(Dt(2,3),Dt(3,2));
  }

  void
  UMATTangentOperator::transpose(tfel::math::st2tost2<3u,UMATReal>& Dt)
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

  void
  UMATTangentOperator::normalize(tfel::math::tmatrix<1u,1u,UMATReal>&)
  {} // end of UMATTangentOperator::normalize

  void
  UMATTangentOperator::normalize(tfel::math::tmatrix<2u,2u,UMATReal>&)
  {} // end of UMATTangentOperator::normalize
  
  void
  UMATTangentOperator::normalize(tfel::math::tmatrix<3u,3u,UMATReal>&)
  {} // end of struct UMATTangentOperator

  void
  UMATTangentOperator::transpose(tfel::math::tmatrix<1u,1u,UMATReal>&)
  {}

  void
  UMATTangentOperator::transpose(tfel::math::tmatrix<2u,2u,UMATReal>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
  }

  void
  UMATTangentOperator::transpose(tfel::math::tmatrix<3u,3u,UMATReal>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(0,3),Dt(3,0));
  }
  
} // end of namespace umat
