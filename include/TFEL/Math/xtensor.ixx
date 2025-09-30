/*!
 * \file   include/TFEL/Math/xtensor.ixx
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

#ifndef LIB_TFEL_MATH_XTENSOR_IXX
#define LIB_TFEL_MATH_XTENSOR_IXX


#include <ranges>
#include <iterator>
#include <iostream>
#include "TFEL/Math/Array/View.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::math {

  /*!
   * P-order tensor
   */

  template<std::size_t P,std::size_t N,typename real>
  xtensor<P-1,N,real>& xtensor<P,N,real>::operator()(const std::size_t& i) {
    return (this->arr)[i];
  }

  template<std::size_t P,std::size_t N,typename real>
  template<typename... typeind>
  auto& xtensor<P,N,real>::operator()(const std::size_t& i0,typeind... ind) {
    return (this->arr)[i0](ind...);
  }

  template<std::size_t P,std::size_t N,typename real>
  xtensor<P-1,N,real> xtensor<P,N,real>::createZero(const std::size_t&){
    return xtensor<P-1,N,real>::zero();
  }
  
  template<std::size_t P,std::size_t N,typename real>
  template<std::size_t... ind>
  std::array<xtensor<P-1,N,real>,N> xtensor<P,N,real>::createZeroArray(const std::index_sequence<ind...>&){
    return {createZero(ind)...};
  }

  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> xtensor<P,N,real>::zero(){
    return createZeroArray(std::make_index_sequence<N>{});
  }

  template<std::size_t P,std::size_t N,typename real>
  template<std::size_t P2>
  xtensor<P+P2,N,real> xtensor<P,N,real>::otimes(xtensor<P2,N,real>& T2) {
    auto Tc=xtensor<P+P2,N,real>::zero();
    for (std::size_t i=0;i<N;i++){
        Tc(i)=((this->arr)[i]).otimes(T2);
    }
    return Tc;
  }

   /*!
   * \brief contraction of a xtensor<P> on indices i, i+1 when P>2
   */
  template<std::size_t P,std::size_t N,typename real>
  xtensor<P-2,N,real> xtensor<P,N,real>::contract(const std::size_t& i) requires (P>2) {
    assert(P>i+1);
    if (i>0){
      auto Tc=xtensor<P-2,N,real>::zero();
      for (std::size_t j=0;j<N;j++){
        Tc(j)=((this->arr)[j]).contract(i-1);
      }
      return Tc;
    }
    else {
      auto Tc=xtensor<P-2,N,real>::zero();
      for (std::size_t k=0;k<N;k++){
        Tc=Tc+(this->arr)[k](k);
      }
      return Tc;
    }
  }

    /*!
   * \brief contraction of a xtensor<P> if P==2
   */
  template<std::size_t P,std::size_t N,typename real>
  real xtensor<P,N,real>::contract(const std::size_t& i) requires (P==2) {
    assert(i==0);
    auto Tc=real(0.);
    for (std::size_t k=0;k<N;k++){
      Tc+=(this->arr)[k](k);
    }
    return Tc;
  }

  template<std::size_t P,std::size_t N,typename real>
  template<std::size_t M>
  auto xtensor<P,N,real>::contract_on(const std::array<std::size_t,M>& ind) requires (M>1) {
    assert(P>=2*M);
    xtensor<P,N,real> T(this->arr);
    xtensor<P-2,N,real> Tc=T.contract(ind[0]);
    std::array<std::size_t,M-1> ind_;
    std::ranges::copy(std::views::drop(ind, 1), ind_.begin());
    return Tc.template contract_on<M-1>(ind_);
  }


  template<std::size_t P,std::size_t N,typename real>
  template<std::size_t M>
  auto xtensor<P,N,real>::contract_on(const std::array<std::size_t,M>& ind) requires (M==1) {
    return this->contract(ind[0]);
  }

  template<std::size_t P,std::size_t N,typename real>
  template<std::size_t P2>
  xtensor<P+P2-2,N,real> xtensor<P,N,real>::dot(xtensor<P2,N,real>& T2) requires (P2>0) {
    auto Tc=this->otimes<P2>(T2);
    return Tc.contract(P-1);
  }

  template<std::size_t P,std::size_t N,typename real>
  template<std::size_t P2,std::size_t M>
  auto xtensor<P,N,real>::contract_with(xtensor<P2,N,real>& T2) {
    assert((P>=M) and (P2>=M));
    xtensor<P+P2,N,real> Tc=this->otimes<P2>(T2);
    std::array<std::size_t,M> ind;
    for (std::size_t i=0;i<M;i++){
      ind[i]=P-1-i;
    }
    return Tc.template contract_on<M>(ind);
  }
  
  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> xtensor<P,N,real>::drop() requires (P==2) {
    xtensor<2,N,real> T(this->arr);
    for (std::size_t i=0;i<N;i++)
    for (std::size_t j=0;j<N;j++){
      T(i,j)=(this->arr)[j](i);
    }
    return T;
  }

  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> xtensor<P,N,real>::drop() requires (P>2) {
    auto T = xtensor<P,N,real>::zero();
    for (std::size_t i=0;i<N;i++){
      T(i) = ((this->arr)[i]).template drop();
    }
    for (std::size_t i=0;i<N;i++)
    for (std::size_t j=0;j<N;j++){
      T(i,j)=T(j,i);
    }
    return T;
  }

  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> xtensor<P,N,real>::transpose() requires (P==2) {
    return this->drop();
  }

  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> xtensor<P,N,real>::transpose() requires (P>2) {
    auto T = this->drop();
    auto Td = xtensor<P,N,real>::zero();
    for (std::size_t i=0;i<N;i++){
      Td(i)=(T(i)).template transpose();
    }
    return Td;
  }

  template<std::size_t P,std::size_t N,typename real>
  st2tost2<3,real> xtensor<P,N,real>::toStensor4() requires ((P==2) and (N==6)){
    st2tost2<3,real> st4;
    for (std::size_t i=0;i<N;i++)
    for (std::size_t j=0;j<N;j++){
      st4(i,j)=(this->arr)[i](j);
    }
    return st4;
  }

   /*!
   * first-order tensor
   */

  template<std::size_t N,typename real>
  real& xtensor<1,N,real>::operator()(const std::size_t& i) {
    return (this->arr)[i];
  }

  template<std::size_t N,typename real>
  real xtensor<1,N,real>::createZero(const std::size_t&){
    return real(0.);
  }

  template<std::size_t N,typename real>
  template<std::size_t... ind>
  std::array<real,N> xtensor<1,N,real>::createZeroArray(const std::index_sequence<ind...>&){
    return {createZero(ind)...};
  }

  template<std::size_t N,typename real>
  xtensor<1,N,real> xtensor<1,N,real>::zero(){
    return createZeroArray(std::make_index_sequence<N>{});
  }

  template<std::size_t N,typename real>
  template<std::size_t P2>
  xtensor<1+P2,N,real> xtensor<1,N,real>::otimes(xtensor<P2,N,real>& T2) {
    auto Tc=xtensor<1+P2,N,real>::zero();
    for (std::size_t i=0;i<N;i++){
      real t0=(this->arr)[i];
      Tc(i)=t0*T2;
    }
    return Tc;
  }

  template<std::size_t N,typename real>
  template<std::size_t P2>
  auto xtensor<1,N,real>::dot(xtensor<P2,N,real>& T2) {
    auto Tc=this->otimes<P2>(T2);
    return Tc.contract(0);
  }

  template<std::size_t N,typename real>
  stensor<3,real> xtensor<1,N,real>::toStensor() requires (N==6){
    stensor<3,real> st;
    for (std::size_t i=0;i<N;i++){
      st(i)=(this->arr)[i];
    }
    return st;
  }

  
  template<std::size_t N,typename real>
  xtensor<1,N,real> operator*(real lamb,xtensor<1,N,real>& T) {
    auto Tc=xtensor<1,N,real>::zero();
    for (std::size_t i=0;i<N;i++){
      Tc(i)=lamb*T(i);
    }
    return Tc;
  }

  
  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> operator*(real lamb,xtensor<P,N,real>& T) {
    auto Tc=xtensor<P,N,real>::zero();
    for (std::size_t i=0;i<N;i++){
        Tc(i)=lamb*T(i);
    }
    return Tc;
    }
 
  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> operator+(xtensor<P,N,real>& T1,xtensor<P,N,real>& T2) {
    auto Tc=xtensor<P,N,real>::zero();
    for (std::size_t i=0;i<N;i++){
        Tc(i)=(T1(i))+(T2(i));
    }
    return Tc;
  }

  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> operator-(xtensor<P,N,real>& T1,xtensor<P,N,real>& T2) {
    auto Tc=xtensor<P,N,real>::zero();
    for (std::size_t i=0;i<N;i++){
        Tc(i)=(T1(i))-(T2(i));
    }
    return Tc;
  }

  template<typename real>
  xtensor<2,6,real> toXtensor(const st2tost2<3,real>& st4){
    auto xt=xtensor<2,6,real>::zero();
    for (std::size_t i=0;i<6;i++)
    for (std::size_t j=0;j<6;j++){
      xt(i,j)=st4(i,j);
    }
    return xt;
  }

  template<typename real>
  xtensor<1,6,real> toXtensor(const stensor<3,real>& st){
    auto xt=xtensor<1,6,real>::zero();
    for (std::size_t i=0;i<6;i++){
      xt(i)=st(i);
    }
    return xt;
  }

  template<std::size_t M,std::size_t N,typename real>
  void fill_matrix(tmatrix<M,N,real>& A, const st2tost2<3u,real>& st2, const std::size_t& i, const std::size_t& j){
    for (std::size_t ii=0;ii<6;ii++)
    for (std::size_t jj=0;jj<6;jj++){
      A(i*6+ii,j*6+jj)=st2(ii,jj);
    }
  }

  template<std::size_t M,std::size_t N,typename real>
  void fill_vector(tmatrix<M,N,real>& A, const stensor<3u,real>& st, const std::size_t& i, const std::size_t& j){
    for (std::size_t ii=0;ii<6;ii++){
      A(i*6+ii,j*6)=st(ii);
    }
  }

  template<std::size_t M,std::size_t N,typename real>
  st2tost2<3u,real> pull_matrix(const tmatrix<M,N,real>& A, const std::size_t& i, const std::size_t& j){
    st2tost2<3u,real> st2;
    for (std::size_t ii=0;ii<6;ii++)
    for (std::size_t jj=0;jj<6;jj++){
      st2(ii,jj)=A(i*6+ii,j*6+jj);
    }
    return st2;
  }

  template<std::size_t M,std::size_t N,typename real>
  stensor<3u,real> pull_vector(const tmatrix<M,N,real>& A, const std::size_t& i, const std::size_t& j){
    stensor<3u,real> st;
    for (std::size_t ii=0;ii<6;ii++){
      st(ii)=A(i*6+ii,j*6);
    }
    return st;
  }


  }// end of namespace tfel::math



#endif /* LIB_TFEL_MATH_XTENSOR_IXX */
