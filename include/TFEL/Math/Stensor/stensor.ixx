/*!
 * \file   stensor.ixx
 * \brief  This file implements the methods of the class stensor.
 * \author Helfer Thomas
 * \date   02 jun 2006
 */

#ifndef _LIB_TFEL_STENSOR_IXX_
#define _LIB_TFEL_STENSOR_IXX_ 

#include <cmath>
#include <iterator>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/StorageTraits.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"

#include"TFEL/Math/Stensor/Internals/StensorComputeEigenValues.hxx"
#include"TFEL/Math/Stensor/Internals/StensorComputeEigenVectors.hxx"
#include"TFEL/Math/Stensor/Internals/StensorImport.hxx"
#include"TFEL/Math/Stensor/Internals/StensorExport.hxx"
#include"TFEL/Math/Stensor/Internals/StensorChangeBasis.hxx"

namespace tfel{

  namespace math {

    namespace internals{
      template<unsigned short N>
      class StensorComputeEigenVectors_;
    }

#ifndef DOXYGENSPECIFIC

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE stensor<N,T,Storage>::stensor(const T init)
    {
      tfel::fsalgo::fill<StensorDimeToSize<N>::value>::exe(this->v,init);
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    T& 
    stensor<N,T,Storage>::operator()(const unsigned short i){
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const T& 
    stensor<N,T,Storage>::operator()(const unsigned short i) const{
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    T& 
    stensor<N,T,Storage>::operator[](const unsigned short i){
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    unsigned short
    stensor<N,T,Storage>::size(void) const{
      return StensorDimeToSize<N>::value;
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const T& 
    stensor<N,T,Storage>::operator[](const unsigned short i) const{
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      stensor<N,T,Storage>&
    >::type 
    stensor<N,T,Storage>::operator=(const StensorExpr<stensor<N,T2,Storage2>, Expr>& src){
      vectorToTab<StensorDimeToSize<N>::value>::exe(src,this->v);
      return *this;
    }

    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<template<unsigned short,typename> class Storage2>
    stensor<N,T,Storage>&
    stensor<N,T,Storage>::operator=(const stensor<N,T,Storage2>& src){
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(src.v,this->v);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2,template<unsigned short,typename> class Storage2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      stensor<N,T,Storage>&
    >::type 
    stensor<N,T,Storage>::operator+=(const stensor<N,T2,Storage2>& src)
    {
      VectorUtilities<StensorDimeToSize<N>::value>::PlusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      stensor<N,T,Storage>&
    >::type
    stensor<N,T,Storage>::operator+=(const StensorExpr<stensor<N,T2,Storage2>,Expr>& src)
    {
      VectorUtilities<StensorDimeToSize<N>::value>::PlusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2,template<unsigned short,typename> class Storage2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      stensor<N,T,Storage>&
    >::type
    stensor<N,T,Storage>::operator-=(const stensor<N,T2,Storage2>& src)
    {
      VectorUtilities<StensorDimeToSize<N>::value>::MinusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      stensor<N,T,Storage>&
    >::type
    stensor<N,T,Storage>::operator-=(const StensorExpr<stensor<N,T2,Storage2>,Expr>& src)
    {
      VectorUtilities<StensorDimeToSize<N>::value>::MinusEqual(*this,src);
      return *this;
    }

    // *= operator
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      stensor<N,T,Storage>&
    >::type
    stensor<N,T,Storage>::operator*=(const T2 s)
    {
      VectorUtilities<StensorDimeToSize<N>::value>::scale(*this,s);
      return *this;
    }

    // /= operator
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      stensor<N,T,Storage>&
    >::type
    stensor<N,T,Storage>::operator/=(const T2 s)
    {
      VectorUtilities<StensorDimeToSize<N>::value>::scale(*this,(static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u))/s);
      return *this;
    }

    // Import from Voigt
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::importVoigt(const T2 * const src)
    {
      tfel::math::internals::ImportFromVoigt<N>::exe(reinterpret_cast<T2*>(this->v),src);
    }
    
    // Import from Tab
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::importTab(const T2* const src)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::math::internals::ImportFromTab<N> Import;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Import::exe(reinterpret_cast<T2*>(this->v),src);
    }

    // Import from values
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::import(const T2 * const src)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(src,reinterpret_cast<base*>(this->v));
    }

    // Export to Tab
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::exportTab(T2* const src) const
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::math::internals::ExportToTab<N> Export;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Export::exe(reinterpret_cast<const T2*>(this->v),src);
    }

    // Write to Tab
    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2 void stensor<N,T,Storage>::write(typename tfel::typetraits::BaseType<T>::type* const t) const
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(this->v),t);
    }

    // computeEigenValues
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void stensor<N,T,Storage>::computeEigenValues(T& vp1,T& vp2,T& vp3) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      tfel::math::internals::StensorComputeEigenValues_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type*>(this->v),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp1),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp2),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp3));
    }

    // computeEigenValues
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void stensor<N,T,Storage>::computeEigenValues(tvector<3u,T>& vp) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      tfel::math::internals::StensorComputeEigenValues_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type* const>(this->v),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(0)),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(1)),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(2)));
    }


    // computeEigenVectors
    template<unsigned short N,typename T, 
	     template<unsigned short,typename> class Storage>
    bool stensor<N,T,Storage>::computeEigenVectors(tvector<3u,T>& vp,tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& vec) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      return tfel::math::internals::StensorComputeEigenVectors_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type*>(this->v),reinterpret_cast<tvector<3u,typename tfel::typetraits::BaseType<T>::type>&>(vp),vec);
    }

    // ChangeBasis
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void 
    stensor<N,T,Storage>::changeBasis(const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m)
    {
      return tfel::math::internals::StensorChangeBasis<N>::exe(this->v,m);
    }

    // Assignement operator
    template<unsigned short N,typename T,
	     template<unsigned short,typename> class Storage>
    template<typename T2,template<unsigned short,typename> class Storage2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      stensor<N,T,Storage>&
    >::type
    stensor<N,T,Storage>::operator=(const stensor<N,T2,Storage2>& src)
    {
      vectorToTab<StensorDimeToSize<N>::value>::exe(src,this->v);
      return *this;
    }

    // Return Id
    template<unsigned short N,typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const stensor<N,T,Storage>&
    stensor<N,T,Storage>::Id(void)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      static const base zero = static_cast<base>(0);
      static const base one  = static_cast<base>(1);
      static const base IdCoef[]  = {one,one,one,zero,zero,zero};
      static const stensor<N,T,Storage> id(IdCoef);
      return id;
    } // end of stensor<N,T,Storage>::Id

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2 
    typename stensor<N,T,Storage>::iterator 
    stensor<N,T,Storage>::begin(void)
    {
      return this->v;
    }

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2 
    typename stensor<N,T,Storage>::const_iterator 
    stensor<N,T,Storage>::begin(void) const
    {
      return this->v;
    }

    template<unsigned short N, typename T, 
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2 typename stensor<N,T,Storage>::iterator stensor<N,T,Storage>::end(void)
    {
      return this->v+StensorDimeToSize<N>::value;
    }

    template<unsigned short N, typename T, 
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2 
    typename stensor<N,T,Storage>::const_iterator
    stensor<N,T,Storage>::end(void) const
    {
      return this->v+StensorDimeToSize<N>::value;
    }

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2 
    typename stensor<N,T,Storage>::reverse_iterator
    stensor<N,T,Storage>::rbegin(void)
    {
      return reverse_iterator(this->v+StensorDimeToSize<N>::value);
    }

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2 
    typename stensor<N,T,Storage>::const_reverse_iterator 
    stensor<N,T,Storage>::rbegin(void) const
    {
      return const_reverse_iterator(this->v+StensorDimeToSize<N>::value);
    }

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2
    typename stensor<N,T,Storage>::reverse_iterator
    stensor<N,T,Storage>::rend(void)
    {
      return reverse_iterator(this->v);
    }

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE2
    typename stensor<N,T,Storage>::const_reverse_iterator
    stensor<N,T,Storage>::rend(void) const
    {
      return const_reverse_iterator(this->v);
    }

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 
    void 
    stensor<N,T,Storage>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<typename T,
	     template<unsigned short,typename> class Storage>
    T tresca(const stensor<1u,T,Storage>& s)
    {
      using namespace std;
      const T sd1 = abs(s[0]-s[1]);
      const T sd2 = abs(s[0]-s[2]);
      const T sd3 = abs(s[2]-s[1]);
      const T tmp  = sd1 > sd2 ? sd1 : sd2;
      const T tmp2 = sd3 > tmp ? sd3 : tmp;
      return tmp2;
    } // end of tresca
    
    template<unsigned short N,
	     typename T,
	     template<unsigned short,typename> class Storage>
    T tresca(const stensor<N,T,Storage>& s)
    {
      using namespace std;
      T s1;
      T s2;
      T s3;
      s.computeEigenValues(s1,s2,s3);
      const T sd1 = abs(s1-s2);
      const T sd2 = abs(s1-s3);
      const T sd3 = abs(s3-s2);
      const T tmp  = sd1 > sd2  ? sd1 : sd2;
      const T tmp2 = sd3 > tmp  ? sd3 : tmp;
      return tmp2;
    } // end of tresca
    
#endif

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSOR_IXX */

