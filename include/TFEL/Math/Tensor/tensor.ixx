/*!
 * \file   tensor.ixx
 * \brief  This file implements the methods of the class tensor.
 * \author Helfer Thomas
 * \date   02 jun 2006
 */

#ifndef _LIB_TFEL_TENSOR_IXX_
#define _LIB_TFEL_TENSOR_IXX_ 

#include <cmath>
#include <iterator>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Tensor/TensorChangeBasis.hxx"

namespace tfel{

  namespace math {

#ifndef DOXYGENSPECIFIC

    template<unsigned short N, typename T>
    tensor<N,T>::tensor(const T init)
    {
      tfel::fsalgo::fill<TensorDimeToSize<N>::value>::exe(this->v,init);
    }

    template<unsigned short N, typename T>
    T& 
    tensor<N,T>::operator()(const unsigned short i){
      assert(i<TensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T>
    const T& 
    tensor<N,T>::operator()(const unsigned short i) const{
      assert(i<TensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T>
    T& 
    tensor<N,T>::operator[](const unsigned short i){
      assert(i<TensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T>
    typename tensor<N,T>::RunTimeProperties
    tensor<N,T>::getRunTimeProperties(void) const{
      return RunTimeProperties();
    }

    template<unsigned short N, typename T>
    const T& 
    tensor<N,T>::operator[](const unsigned short i) const{
      assert(i<TensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tensor<N,T>&
    >::type 
    tensor<N,T>::operator=(const TensorExpr<tensor<N,T2>, Expr>& src){
      vectorToTab<TensorDimeToSize<N>::value>::exe(src,this->v);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tensor<N,T>&
    >::type 
    tensor<N,T>::operator+=(const tensor<N,T2>& src)
    {
      VectorUtilities<TensorDimeToSize<N>::value>::PlusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tensor<N,T>&
    >::type
    tensor<N,T>::operator+=(const TensorExpr<tensor<N,T2>,Expr>& src)
    {
      VectorUtilities<TensorDimeToSize<N>::value>::PlusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tensor<N,T>&
    >::type
    tensor<N,T>::operator-=(const tensor<N,T2>& src)
    {
      VectorUtilities<TensorDimeToSize<N>::value>::MinusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tensor<N,T>&
    >::type
    tensor<N,T>::operator-=(const TensorExpr<tensor<N,T2>,Expr>& src)
    {
      VectorUtilities<TensorDimeToSize<N>::value>::MinusEqual(*this,src);
      return *this;
    }

    // *= operator
    template<unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      tensor<N,T>&
    >::type
    tensor<N,T>::operator*=(const T2 s)
    {
      VectorUtilities<TensorDimeToSize<N>::value>::scale(*this,s);
      return *this;
    }

    // /= operator
    template<unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      tensor<N,T>&
    >::type
    tensor<N,T>::operator/=(const T2 s)
    {
      VectorUtilities<TensorDimeToSize<N>::value>::scale(*this,(static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u))/s);
      return *this;
    }

    // Import from values
    template<unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    tensor<N,T>::import(const T2 * const src)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<TensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(src,reinterpret_cast<base*>(this->v));
    }

    // Write to Tab
    template<unsigned short N, typename T>
    void tensor<N,T>::write(typename tfel::typetraits::BaseType<T>::type* const t) const
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<TensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(this->v),t);
    }

    // ChangeBasis
    template<unsigned short N,typename T>
    void 
    tensor<N,T>::changeBasis(const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m)
    {
      TensorChangeBasis<N,T>::exe(this->v,m);
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tensor<N,T>&
    >::type
    tensor<N,T>::operator=(const tensor<N,T2>& src)
    {
      vectorToTab<TensorDimeToSize<N>::value>::exe(src,this->v);
      return *this;
    }

    // Return Id
    template<unsigned short N,typename T>
    const tensor<N,T>&
    tensor<N,T>::Id(void)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      static const base zero = static_cast<base>(0);
      static const base one  = static_cast<base>(1);
      static const base IdCoef[]  = {one,one,one,
				     zero,zero,zero,
				     zero,zero,zero};
      static const tensor<N,T> id(IdCoef);
      return id;
    } // end of tensor<N,T>::Id

    template<unsigned short N, typename T>
    template<typename InputIterator>
    void 
    tensor<N,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<TensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<unsigned short N, typename T,
	     typename OutputIterator>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      void>::type
    exportToBaseTypeArray(const tensor<N,T>& t,
			  OutputIterator p)
    {    
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(&t[0]),p);
    }

#endif

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TENSOR_IXX */

