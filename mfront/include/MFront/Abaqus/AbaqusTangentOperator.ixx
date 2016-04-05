/*!
 * \file   AbaqusTangentOperator.ixx
 * \brief    
 * \author THOMAS HELFER
 * \date   04 avril 2016
 */

#ifndef LIB_ABAQUSTANGENTOPERATOR_IXX_
#define LIB_ABAQUSTANGENTOPERATOR_IXX_

namespace abaqus
{

  template<typename real>
  void
  AbaqusTangentOperator<real>::normalize(tfel::math::ST2toST2View<1u,real>&)
  {} // end of AbaqusTangentOperator<real>::normalize

  template<typename real>
  void
  AbaqusTangentOperator<real>::normalize(tfel::math::ST2toST2View<2u,real>& Dt)
  {
    using namespace std;
    static const real inv_sqrt2 = real(1)/std::sqrt(real(2));
    Dt(0,3) *= inv_sqrt2;
    Dt(1,3) *= inv_sqrt2;
    Dt(2,3) *= inv_sqrt2;
    Dt(3,0) *= inv_sqrt2;
    Dt(3,1) *= inv_sqrt2;
    Dt(3,2) *= inv_sqrt2;
    Dt(3,3) *= 0.5;
  } // end of AbaqusTangentOperator<real>::normalize
  
  template<typename real>
  void
  AbaqusTangentOperator<real>::normalize(tfel::math::ST2toST2View<3u,real>& Dt)
  {
    using namespace std;
    static const real inv_sqrt2 = real(1)/std::sqrt(real(2));
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

  template<typename real>
  void
  AbaqusTangentOperator<real>::transpose(tfel::math::ST2toST2View<1u,real>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(1,2),Dt(2,1));
  }

  template<typename real>
  void
  AbaqusTangentOperator<real>::transpose(tfel::math::ST2toST2View<2u,real>& Dt)
  {
    std::swap(Dt(0,1),Dt(1,0));
    std::swap(Dt(0,2),Dt(2,0));
    std::swap(Dt(0,3),Dt(3,0));
    std::swap(Dt(1,2),Dt(2,1));
    std::swap(Dt(1,3),Dt(3,1));
    std::swap(Dt(2,3),Dt(3,2));
  }

  template<typename real>
  void
  AbaqusTangentOperator<real>::transpose(tfel::math::ST2toST2View<3u,real>& Dt)
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

#endif /* LIB_ABAQUSTANGENTOPERATOR_IXX_ */
