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
   * \brief P-order tensor
   */
  template<std::size_t P,std::size_t N,typename real>
  struct xtensor {
    std::array<xtensor<P-1,N,real>,N> arr;
    xtensor() : arr(std::array<xtensor<P-1,N,real>,N>{}) {}
    xtensor(const std::array<xtensor<P-1,N,real>,N>& t) : arr(t) {}

    xtensor<P-1,N,real>& operator()(const std::size_t&);
    template<typename... typeind>
    auto& operator()(const std::size_t&,typeind...);

    static xtensor<P-1,N,real> createZero(const std::size_t&);
    template<std::size_t... ind>
    static std::array<xtensor<P-1,N,real>,N> createZeroArray(const std::index_sequence<ind...>&);
    static xtensor<P,N,real> zero();

    template<std::size_t P2>
    xtensor<P+P2,N,real> otimes(xtensor<P2,N,real>&);

    xtensor<P-2,N,real> contract(const std::size_t&) requires (P>2);
    real contract(const std::size_t&) requires (P==2);

    template<std::size_t M>
    auto contract_on(const std::array<std::size_t,M>&) requires (M>1);
    template<std::size_t M>
    auto contract_on(const std::array<std::size_t,M>&) requires (M==1);

    template<std::size_t P2>
    xtensor<P+P2-2,N,real> dot(xtensor<P2,N,real>&) requires (P2>0);

    template<std::size_t P2,std::size_t M>
    auto contract_with(xtensor<P2,N,real>&);

    xtensor<P,N,real> drop() requires (P==2);
    xtensor<P,N,real> drop() requires (P>2);

    xtensor<P,N,real> transpose() requires (P==2);
    xtensor<P,N,real> transpose() requires (P>2);

    st2tost2<3,real> toStensor4() requires ((P==2) and (N==6));

    ~xtensor(){};
  };


   /*!
   * \brief first-order tensor
   */
  template<std::size_t N,typename real>
  struct xtensor<1,N,real> {
    std::array<real,N> arr;
    xtensor() : arr(std::array<real,N>{}) {}
    xtensor(const std::array<real,N>& t) : arr(t) {}

    real& operator()(const std::size_t&);

    static real createZero(const std::size_t&);
    template<std::size_t... ind>
    static std::array<real,N> createZeroArray(const std::index_sequence<ind...>&);
    static xtensor<1,N,real> zero();

    template<std::size_t P2>
    xtensor<1+P2,N,real> otimes(xtensor<P2,N,real>&);

    template<std::size_t P2>
    auto dot(xtensor<P2,N,real>&);

    stensor<3,real> toStensor() requires (N==6);

    ~xtensor(){};
  };


    /*!
   * \brief product of a real and a xtensor<1>
   */
  template<std::size_t N,typename real>
  xtensor<1,N,real> operator*(real,xtensor<1,N,real>&);

    /*!
   * \brief product of a real and a xtensor<P>
   */
  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> operator*(real,xtensor<P,N,real>&);

    /*!
   * \brief sum of 2 xtensor<P>
   */
  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> operator+(xtensor<P,N,real>& T1,xtensor<P,N,real>& T2);


    /*!
   * \brief substraction of 2 xtensor<P>
   */
  template<std::size_t P,std::size_t N,typename real>
  xtensor<P,N,real> operator-(xtensor<P,N,real>& T1,xtensor<P,N,real>& T2);

    /*!
   * \brief conversion of a st2tost2<3> to a xtensor<2,6>
   */
  template<typename real>
  xtensor<2,6,real> toXtensor(const st2tost2<3,real>&);
  
   /*!
   * \brief conversion of a stensor<3> to a xtensor<1,6>
   */
  template<typename real>
  xtensor<1,6,real> toXtensor(const stensor<3,real>&);


  }// end of namespace tfel::math

#include "TFEL/Math/xtensor.ixx"

#endif /* LIB_TFEL_MATH_XTENSOR_HXX */
