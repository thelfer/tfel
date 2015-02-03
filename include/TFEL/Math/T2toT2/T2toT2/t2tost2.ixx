/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2/t2tost2.ixx
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

#ifndef _LIB_TFEL_MATH_T2TOST2_IXX_
#define _LIB_TFEL_MATH_T2TOST2_IXX_ 

#include<cmath>
#include<iterator>
#include<algorithm>
#include<type_traits>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"

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

    template<unsigned short N, typename T>
    t2tost2<N,T>::t2tost2(const T init)
    {
      tfel::fsalgo::fill<StensorDimeToSize<N>::value*
			 TensorDimeToSize<N>::value>::exe(this->v,init);
    }

    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    t2tost2<N,T>::t2tost2(const T2toST2Expr<t2tost2<N,T2>,Expr>& src){
      matrix_utilities<StensorDimeToSize<N>::value,
		       TensorDimeToSize<N>::value,
		       TensorDimeToSize<N>::value>::copy(src,*this);
    }

    template<unsigned short N, typename T>
    T& 
    t2tost2<N,T>::operator()(const unsigned short i,const unsigned short j){
      return this->v[TensorDimeToSize<N>::value*i+j];
    }

    template<unsigned short N, typename T>
    const T& 
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

#endif /* _LIB_TFEL_MATH_T2TOST2_IXX */

