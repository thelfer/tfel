/*!
 * \file   include/TFEL/Math/xtensor.hxx
 * \brief
 * \author Antoine Martin
 * \date   18 Sept 2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_XTENSOR_HXX
#define LIB_TFEL_MATH_XTENSOR_HXX

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Array/GenericFixedSizeArray.hxx"
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Tensor/TensorConceptOperations.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Forward/tensor.hxx"

namespace tfel::math {


    
  /*!
   * \brief 
   */
  template<std::size_t P,std::size_t N,typename real>
  struct xtensor {
    std::array<xtensor<P-1,N,real>,N> arr;
    xtensor(std::array<<xtensor<P-1,N,real>,N>,N>& t) : arr(t) {}
    const real& operator()(std::array<std::size_t,P>& ii) const {
        return arri(ii(0))(std::ranges::views::drop(ii, 1));
    }
    real& operator()(std::array<std::size_t,P>& ii) {
        return arri(ii(0))(std::ranges::views::drop(ii, 1));
    }
  }

  /*!
   * \brief 
   */
  template<1,N,typename real>
  struct xtensor {
    std::array<real,N> arr;
    xtensor(std::array<real,N>& t) : arr(t) {}
    const real& operator()(std::size_t i) const {
        return (this->arr)(i);
    }
    real& operator()(std::size_t i) {
        return (this->arr)(i);
    }
  }

  /*!
   * \brief 
   */
  template<0,N,typename real>
  struct xtensor {
    real arr;
    xtensor(real& t) : arr(t) {}
    const real& operator() const {
        return this->arr;
    }
    void operator()(real& t) {
        this->arr=t;
    }
  }

  /*!
   * \brief contraction of a xtensor on indices i, i+1
   */
  template<std::size_t P,std::size_t N,typename real>
  xtensor<P-2,N,real> contract(xtensor<P,N,real>& T,i) {
    xtensor<P-2,N,real> Tc;
    if (i>0){
        for (std::size_t j=0;j<N;j++){
            Tc(j)=contract(T(j),i-1)
        }
    }
    if ((i==0) and (P>2)){
        for (std::size_t j=0;j<N;j++){
            for (int k=0;k<N;k++){
                Tc(j)+=T(k)(k)(j);
            }
        }
    }
    if (P==2){
        r=real(0);
        for (int k=0;k<N;k++){
            r+=T(k)(k);
        }
        Tc(r);
    }
    return Tc;
    }

   /*!
   * \brief dyadic product of 2 xtensors
   */
  template<std::size_t P1,std::size_t P2,std::size_t N,typename real>
  xtensor<P1+P2,N,real> otimes(xtensor<P1,N,real>& T1,xtensor<P2,N,real>& T2) {
    std::size_t P=P1+P2;
    xtensor<P,N,real> Tc;
    if (P1>1){
        for (std::size_t i=0;i<N;i++){
            Tc(i)=otimes<P1-1,P2,N,real>(T1(i),T2);
        }
    }
    if (P1==1){
        for (std::size_t i=0;i<N;i++){
            Tc(i)=T1(i)*T2;
        }
    }
    return Tc;
    }


  /*!
   * \brief M-contraction of 2 xtensor
   */
  template<std::size_t P1,std::size_t P2,std::size_t M,std::size_t N,typename real>
  xtensor<P1+P2-M,N,real> dot(xtensor<P1,N,real>& T1,xtensor<P2,N,real>& T2) {
    xtensor<P1+P2-M,N,real> Tc;
    xtensor<P1+P2,N,real> T0=otimes<P1,P2,N,real>(T1,T2);
    std::size_t p=0;
    while (p<M){
        xtensor<P1+P2-2*p-1,N,real> T0=contract(T0,P1-1-p);
        p++;
    }

    }


  }// end of namespace tfel::math



#endif /* LIB_TFEL_MATH_XTENSOR_HXX */
