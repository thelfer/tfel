/*!
 * \file   include/TFEL/Math/T2toT2/t2tot2.ixx
 * \brief  This file implements the methods of the class t2tot2.
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_T2TOT2_IXX_
#define _LIB_TFEL_MATH_T2TOT2_IXX_ 

#include <cmath>
#include <iterator>
#include <algorithm>
#include<type_traits>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"
#include"TFEL/Math/T2toT2/TensorProductLeftDerivativeExpr.hxx"
#include"TFEL/Math/T2toT2/TensorProductRightDerivativeExpr.hxx"

namespace tfel{

  namespace math {

    // Assignement operator
    template<typename Child>
    template<typename T2toT2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
      T2toT2Traits<Child>::dime==T2toT2Traits<T2toT2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename T2toT2Traits<T2toT2Type>::NumType,
				       typename T2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tot2_base<Child>::operator=(const T2toT2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::copy(src,child);
      return child;
    }

    template<typename Child>
    template<typename T2toT2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
      T2toT2Traits<Child>::dime==T2toT2Traits<T2toT2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename T2toT2Traits<T2toT2Type>::NumType,
				       typename T2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tot2_base<Child>::operator+=(const T2toT2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::plusEqual(child,src);
      return child;
    }

    // Assignement operator
    template<typename Child>
    template<typename T2toT2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
      T2toT2Traits<Child>::dime==T2toT2Traits<T2toT2Type>::dime &&
      tfel::typetraits::IsAssignableTo<typename T2toT2Traits<T2toT2Type>::NumType,
				       typename T2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tot2_base<Child>::operator-=(const T2toT2Type& src)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::minusEqual(child,src);
      return child;
    }

    // *= operator
    template<typename Child>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<typename T2toT2Traits<Child>::NumType,
						 T2,OpMult>::type,
			     typename T2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tot2_base<Child>::operator*=(const T2 s)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::multByScalar(child,s);
      return child;
    }

    // /= operator
    template<typename Child>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<typename T2toT2Traits<Child>::NumType,
						 T2,OpDiv>::type,
			     typename T2toT2Traits<Child>::NumType>::cond,
      Child&>::type
    t2tot2_base<Child>::operator/=(const T2 s)
    {
      auto& child = static_cast<Child&>(*this);
      matrix_utilities<TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value,
		       TensorDimeToSize<T2toT2Traits<Child>::dime>::value>::divByScalar(child,s);
      return child;
    }

    template<unsigned short N, typename T>
    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,T>::cond,
      T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> > >::type
    t2tot2<N,T>::tpld(const TensorType& B)
    {
      return T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> >(B);
    } // end of t2tot2<N,T>

    template<unsigned short N, typename T>
    template<typename TensorType,
	     typename T2toT2Type>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
      TensorTraits<TensorType>::dime==N&&
      T2toT2Traits<T2toT2Type>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename TensorTraits<TensorType>::NumType,
								    typename T2toT2Traits<T2toT2Type>::NumType,
								    OpMult>::Result,T>::cond,
      T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> > >::type
    t2tot2<N,T>::tpld(const TensorType& B,
		      const T2toT2Type& C)
    {
      return T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> >(B,C);
    }

    template<unsigned short N, typename T>
    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,T>::cond,
      T2toT2Expr<t2tot2<N,T>,TensorProductRightDerivativeExpr<N> > >::type
    t2tot2<N,T>::tprd(const TensorType& A)
    {
      return T2toT2Expr<t2tot2<N,T>,TensorProductRightDerivativeExpr<N> >(A);
    }

    template<unsigned short N, typename T>
    template<typename TensorType,
	     typename T2toT2Type>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
      TensorTraits<TensorType>::dime==N&&
      T2toT2Traits<T2toT2Type>::dime==N&&
      tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename TensorTraits<TensorType>::NumType,
								    typename T2toT2Traits<T2toT2Type>::NumType,
								    OpMult>::Result,T>::cond,
      T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> > >::type
    t2tot2<N,T>::tprd(const TensorType& A,
		      const T2toT2Type& C)
    {
      return T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> >(A,C);
    }

    template<unsigned short N, typename T>
    constexpr t2tot2<N,T>::t2tot2()
    {}

    template<unsigned short N, typename T>
    template<typename T2,
	     typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,bool>::type>
    constexpr t2tot2<N,T>::t2tot2(const T2& init)
      : fsarray<TensorDimeToSize<N>::value*TensorDimeToSize<N>::value,T>(init)
    {}

    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    t2tot2<N,T>::t2tot2(const T2toT2Expr<t2tot2<N,T2>,Expr>& src){
      matrix_utilities<TensorDimeToSize<N>::value,
		       TensorDimeToSize<N>::value,
		       TensorDimeToSize<N>::value>::copy(src,*this);
    }

    template<unsigned short N,typename T>
    constexpr t2tot2<N,T>::t2tot2(const t2tot2<N,T>& src)
      : T2toT2Concept<t2tot2<N,T>>(src),
        fsarray<TensorDimeToSize<N>::value*TensorDimeToSize<N>::value,T>(src)
    {}

    template<unsigned short N,typename T>
    t2tot2<N,T>&
    t2tot2<N,T>::operator=(const t2tot2<N,T>& src)
    {
      fsarray<TensorDimeToSize<N>::value*TensorDimeToSize<N>::value,T>::operator=(src);
      return *this;
    }

    template<unsigned short N, typename T>
    T& 
    t2tot2<N,T>::operator()(const unsigned short i,const unsigned short j){
      return this->v[TensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    constexpr const T& 
    t2tot2<N,T>::operator()(const unsigned short i,const unsigned short j) const{
      return this->v[TensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 void t2tot2<N,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<TensorDimeToSize<N>::value*
			 TensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<unsigned short N, typename T>
    typename t2tot2<N,T>::RunTimeProperties
    t2tot2<N,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    } // end of t2tot2<N,T>::getRunTimeProperties

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_T2TOT2_IXX */

