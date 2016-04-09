/*!
 * \file   AbaqusTangentOperator.ixx
 * \brief    
 * \author THOMAS HELFER
 * \date   04 avril 2016
 */

#ifndef LIB_ABAQUSTANGENTOPERATOR_IXX_
#define LIB_ABAQUSTANGENTOPERATOR_IXX_

#include"TFEL/Math/General/ConstExprMathFunctions.hxx"

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
#ifndef _MSC_VER
    constexpr const real cste = 1/(tfel::math::constexpr_fct::sqrt(real(2)));
#else
    static const real cste = 1/(std::sqrt(real(2)));
#endif
    Dt(0,3) *= cste;
    Dt(1,3) *= cste;
    Dt(2,3) *= cste;
    Dt(3,0) *= cste;
    Dt(3,1) *= cste;
    Dt(3,2) *= cste;
    Dt(3,3) /= 2;
  } // end of AbaqusTangentOperator<real>::normalize
  
  template<typename real>
  void
  AbaqusTangentOperator<real>::normalize(tfel::math::ST2toST2View<3u,real>& Dt)
  {
#ifndef _MSC_VER
    constexpr const real cste = 1/(tfel::math::constexpr_fct::sqrt(real(2)));
#else
    static const real cste = 1/(std::sqrt(real(2)));
#endif
    Dt(0,3) *= cste;
    Dt(1,3) *= cste;
    Dt(2,3) *= cste;
    Dt(0,4) *= cste;
    Dt(1,4) *= cste;
    Dt(2,4) *= cste;
    Dt(0,5) *= cste;
    Dt(1,5) *= cste;
    Dt(2,5) *= cste;
    Dt(3,0) *= cste;
    Dt(3,1) *= cste;
    Dt(3,2) *= cste;
    Dt(4,0) *= cste;
    Dt(4,1) *= cste;
    Dt(4,2) *= cste;
    Dt(5,0) *= cste;
    Dt(5,1) *= cste;
    Dt(5,2) *= cste;
    Dt(3,3) /= 2;
    Dt(3,4) /= 2;
    Dt(3,5) /= 2;
    Dt(4,3) /= 2;
    Dt(4,4) /= 2;
    Dt(4,5) /= 2;
    Dt(5,3) /= 2;
    Dt(5,4) /= 2;
    Dt(5,5) /= 2;
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
