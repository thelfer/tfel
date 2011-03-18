/*!
 * \file   st2tost2.ixx
 * \brief  This file implements the methods of the class st2tost2.
 * \author Helfer Thomas
 * \date   02 jun 2006
 */

#ifndef _LIB_TFEL_ST2TOST2_IXX_
#define _LIB_TFEL_ST2TOST2_IXX_ 

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <iterator>
#include <algorithm>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"

#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/StorageTraits.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"

namespace tfel{

  namespace math {

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE st2tost2<N,T>::st2tost2(const T init)
    {
      tfel::fsalgo::fill<St2tost2Size>::exe(this->v,init);
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE st2tost2<N,T>::st2tost2(const st2tost2<N,T>& src)
    {
      matrix_utilities<StensorSize,StensorSize,StensorSize>::copy(src,*this);
    }

    template<unsigned short N, typename T>
    const st2tost2<N,T>&
    st2tost2<N,T>::Id(void){
      static const st2tost2<N,T> sId(st2tost2<N,T>::ST2TOST2_IDENTITY);
      return sId;
    } // end of st2tost2<N,T>::Id

    template<unsigned short N, typename T>
    const st2tost2<N,T>&
    st2tost2<N,T>::J(void){
      static const st2tost2<N,T> sJ(st2tost2<N,T>::ST2TOST2_J);
      return sJ;
    } // end of st2tost2<N,T>::J

    template<unsigned short N, typename T>
    const st2tost2<N,T>&
    st2tost2<N,T>::IxI(void){
      static const st2tost2<N,T> sIxI(st2tost2<N,T>::ST2TOST2_IxI);
      return sIxI;
    } // end of st2tost2<N,T>::IxI

    template<unsigned short N, typename T>
    const st2tost2<N,T>&
    st2tost2<N,T>::K(void){
      static const st2tost2<N,T> sK(st2tost2<N,T>::ST2TOST2_K);
      return sK;
    } // end of st2tost2<N,T>::K

    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE 
    st2tost2<N,T>::st2tost2(const ST2toST2Expr<st2tost2<N,T2>,Expr>& src){
      matrix_utilities<StensorSize,StensorSize,StensorSize>::copy(src,*this);
    }

    template<unsigned short N,typename T>
    st2tost2<N,T>::st2tost2(const st2tost2<N,T>::ParticularSt2toSt2 id){
      using namespace std;
      unsigned short i,j;
      std::fill(this->v,this->v+St2tost2Size,T(0));
      if(id==ST2TOST2_IDENTITY){
	for(i=0;i!=StensorSize;++i){
	  this->operator()(i,i)=T(1);
	}
      } else if (id==ST2TOST2_J){
	for(i=0;i!=3;++i){
	  for(j=0;j!=3;++j){
	    this->operator()(i,j)=1/T(3);
	  }
	}
      } else if (id==ST2TOST2_IxI){
	for(i=0;i!=3;++i){
	  for(j=0;j!=3;++j){
	    this->operator()(i,j)=T(1);
	  }
	}
      } else if (id==ST2TOST2_K){
	for(i=0;i!=3;++i){
	  for(j=0;j!=3;++j){
	    if(i==j){
	      this->operator()(i,j)=2/T(3);
	    } else {
	      this->operator()(i,j)=-1/T(3);
	    }
	  }
	}
	for(i=3;i!=StensorSize;++i){
	  this->operator()(i,i)=T(1);
	}
      }
    } // end of st2tost2(const st2tost2<N,T>::ParticularSt2toSt2)

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE T& 
    st2tost2<N,T>::operator()(const unsigned short i,const unsigned short j){
      assert(i<StensorSize);
      assert(j<StensorSize);
      return this->v[StensorSize*i+j];
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE const T& 
    st2tost2<N,T>::operator()(const unsigned short i,const unsigned short j) const{
      assert(i<StensorSize);
      assert(j<StensorSize);
      return this->v[StensorSize*i+j];
    }

    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      st2tost2<N,T>&
    >::type 
    st2tost2<N,T>::operator=(const ST2toST2Expr<st2tost2<N,T2>, Expr>& src){
      matrix_utilities<StensorSize,StensorSize,StensorSize>::copy(src,*this);
      return *this;
    }

    template<unsigned short N,typename T>
    st2tost2<N,T>&
    st2tost2<N,T>::operator=(const st2tost2<N,T>& src){
      tfel::fsalgo::copy<St2tost2Size>::exe(src.v,this->v);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      st2tost2<N,T>&
    >::type 
    st2tost2<N,T>::operator+=(const st2tost2<N,T2>& src)
    {
      matrix_utilities<StensorSize,StensorSize,StensorSize>::plusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      st2tost2<N,T>&
    >::type
    st2tost2<N,T>::operator+=(const ST2toST2Expr<st2tost2<N,T2>,Expr>& src)
    {
      matrix_utilities<StensorSize,StensorSize,StensorSize>::plusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      st2tost2<N,T>&
    >::type
    st2tost2<N,T>::operator-=(const st2tost2<N,T2>& src)
    {
      matrix_utilities<StensorSize,StensorSize,StensorSize>::minusEqual(*this,src);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      st2tost2<N,T>&
    >::type
    st2tost2<N,T>::operator-=(const ST2toST2Expr<st2tost2<N,T2>,Expr>& src)
    {
      matrix_utilities<StensorSize,StensorSize,StensorSize>::minusEqual(*this,src);
      return *this;
    }

    // *= operator
    template<unsigned short N,typename T>
    template<typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      st2tost2<N,T>&
    >::type
    st2tost2<N,T>::operator*=(const T2 s)
    {
      matrix_utilities<StensorSize,StensorSize,StensorSize>::multByScalar(*this,s);
      return *this;
    }

    // /= operator
    template<unsigned short N,typename T>
    template<typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      st2tost2<N,T>&
    >::type
    st2tost2<N,T>::operator/=(const T2 s)
    {
      matrix_utilities<StensorSize,StensorSize,StensorSize>::divByScalar(*this,s);
      return *this;
    }

    // Assignement operator
    template<unsigned short N,typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      st2tost2<N,T>&
    >::type
    st2tost2<N,T>::operator=(const st2tost2<N,T2>& src)
    {
      tfel::fsalgo::copy<St2tost2Size>::exe(src,*this);
      return *this;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2 
    typename st2tost2<N,T>::iterator 
    st2tost2<N,T>::begin(void)
    {
      return this->v;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2 
    typename st2tost2<N,T>::const_iterator 
    st2tost2<N,T>::begin(void) const
    {
      return this->v;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2 typename 
    st2tost2<N,T>::iterator 
    st2tost2<N,T>::end(void)
    {
      return this->v+St2tost2Size;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2 
    typename st2tost2<N,T>::const_iterator 
    st2tost2<N,T>::end(void) const
    {
      return this->v+St2tost2Size;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2 
    typename st2tost2<N,T>::reverse_iterator 
    st2tost2<N,T>::rbegin(void)
    {
      return reverse_iterator(this->v+St2tost2Size);
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2 
    typename st2tost2<N,T>::const_reverse_iterator 
    st2tost2<N,T>::rbegin(void) const
    {
      return const_reverse_iterator(this->v+St2tost2Size);
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename st2tost2<N,T>::reverse_iterator 
    st2tost2<N,T>::rend(void)
    {
      return reverse_iterator(this->v);
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2 
    typename st2tost2<N,T>::const_reverse_iterator 
    st2tost2<N,T>::rend(void) const
    {
      return const_reverse_iterator(this->v);
    }

    template<unsigned short N, typename T>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 void st2tost2<N,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<St2tost2Size>::exe(src,*this);
    }

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ST2TOST2_IXX */

