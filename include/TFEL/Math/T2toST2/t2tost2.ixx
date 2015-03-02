/*!
 * \file   include/TFEL/Math/T2toST2/t2tost2.ixx
 * \brief  This file implements the methods of the class t2tost2.
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_T2TOST2_IXX_
#define LIB_TFEL_MATH_T2TOST2_IXX_ 

#include<cmath>
#include<iterator>
#include<algorithm>
#include<type_traits>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"
#include"TFEL/Math/T2toST2/LeftCauchyGreenTensorDerivativeExpr.hxx"
#include"TFEL/Math/T2toST2/RightCauchyGreenTensorDerivativeExpr.hxx"

namespace tfel{

  namespace math {

    // Assignement operator
    template<typename Child>
    template<typename T2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond &&
      T2toST2Traits<Child>::dime==T2toST2Traits<T2toST2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename T2toST2Traits<T2toST2Type>::NumType,
				       typename T2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tost2_base<Child>::operator=(const T2toST2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::copy(src,child);
      return child;
    }

    template<typename Child>
    template<typename T2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond &&
      T2toST2Traits<Child>::dime==T2toST2Traits<T2toST2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename T2toST2Traits<T2toST2Type>::NumType,
				       typename T2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tost2_base<Child>::operator+=(const T2toST2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::plusEqual(child,src);
      return child;
    }

    // Assignement operator
    template<typename Child>
    template<typename T2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond &&
      T2toST2Traits<Child>::dime==T2toST2Traits<T2toST2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename T2toST2Traits<T2toST2Type>::NumType,
				       typename T2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tost2_base<Child>::operator-=(const T2toST2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::minusEqual(child,src);
      return child;
    }

    // *= operator
    template<typename Child>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<typename T2toST2Traits<Child>::NumType,
						 T2,OpMult>::type,
			     typename T2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tost2_base<Child>::operator*=(const T2 s)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::multByScalar(child,s);
      return child;
    }

    // /= operator
    template<typename Child>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<typename T2toST2Traits<Child>::NumType,
						 T2,OpDiv>::type,
			     typename T2toST2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tost2_base<Child>::operator/=(const T2 s)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<StensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toST2Traits<Child>::dime>::value>::divByScalar(child,s);
      return child;
    }

    template<unsigned short N,typename T>
    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,T>::cond,
      Expr<t2tost2<N,T>,RightCauchyGreenTensorDerivativeExpr<N> > >::type
    t2tost2<N,T>::dCdF(const TensorType& F)
    {
      return Expr<t2tost2<N,T>,RightCauchyGreenTensorDerivativeExpr<N> >(F);
    } // end of t2tost2::dCdF

    template<unsigned short N,typename T>
    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,T>::cond,
      Expr<t2tost2<N,T>,LeftCauchyGreenTensorDerivativeExpr<N> > >::type
    t2tost2<N,T>::dBdF(const TensorType& F)
    {
      return Expr<t2tost2<N,T>,LeftCauchyGreenTensorDerivativeExpr<N> >(F);
    } // end of t2tost2::dBdF

    template<unsigned short N, typename T>
    constexpr t2tost2<N,T>::t2tost2()
    {} // end of t2tost2<N,T>::t2tost2

    template<unsigned short N, typename T>
    template<typename T2,
	     typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,bool>::type>
    constexpr t2tost2<N,T>::t2tost2(const T2& init)
      : fsarray<StensorDimeToSize<N>::value*TensorDimeToSize<N>::value,T>(init)
    {} // end of t2tost2<N,T>::t2tost2

    template<unsigned short N,typename T>
    constexpr t2tost2<N,T>::t2tost2(const t2tost2<N,T>& src)
      : T2toST2Concept<t2tost2<N,T>>(src),
        fsarray<StensorDimeToSize<N>::value*TensorDimeToSize<N>::value,T>(src)
    {}
    
    template<unsigned short N,typename T>
    template<typename T2,typename Op>
    t2tost2<N,T>::t2tost2(const Expr<t2tost2<N,T2>,Op>& src){
      matrix_utilities<StensorDimeToSize<N>::value,
		       TensorDimeToSize<N>::value,
		       TensorDimeToSize<N>::value>::copy(src,*this);
    }

    template<unsigned short N,typename T>
    t2tost2<N,T>&
    t2tost2<N,T>::operator=(const t2tost2<N,T>& src)
    {
      fsarray<StensorDimeToSize<N>::value*TensorDimeToSize<N>::value,T>::operator=(src);
      return *this;
    }

    template<unsigned short N, typename T>
    T& 
    t2tost2<N,T>::operator()(const unsigned short i,const unsigned short j){
      return this->v[TensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    constexpr const T& 
    t2tost2<N,T>::operator()(const unsigned short i,const unsigned short j) const{
      return this->v[TensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 void t2tost2<N,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<StensorDimeToSize<N>::value*TensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<unsigned short N, typename T>
    typename t2tost2<N,T>::RunTimeProperties
    t2tost2<N,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    } // end of t2tost2<N,T>::getRunTimeProperties

  } //end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOST2_IXX_ */

