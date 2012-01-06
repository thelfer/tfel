/*!
 * \file   tmatrix.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   28 Jun 2006
 */

#ifndef _LIB_TFEL_TINY_MATRIX_IXX_
#define _LIB_TFEL_TINY_MATRIX_IXX_ 

#include <cassert>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"

#include"TFEL/Math/General/AbsCompare.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"

namespace tfel{
  
  namespace math{

#ifndef DOXYGENSPECIFIC

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE tmatrix<N,M,T>::tmatrix()
    {}

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE tmatrix<N,M,T>::tmatrix(const T init)
    {
      tfel::fsalgo::fill<N*M>::exe(this->m,init);
    }


    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE tmatrix<N,M,T>::tmatrix(const T * const init)
    {
      tfel::fsalgo::copy<N*M>::exe(init,this->m);
    }
    
    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE const T& 
    tmatrix<N,M,T>::operator()(const unsigned short i,const unsigned short j) const
    {
      assert(i<N);
      assert(j<M);
      return this->m[i*M+j];
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE T& 
    tmatrix<N,M,T>::operator()(const unsigned short i,const unsigned short j)
    {
      assert(i<N);
      assert(j<M);
      return this->m[i*M+j];
    }

    template<unsigned short N,unsigned short M, typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tmatrix<N,M,T>&
    >::type 
    tmatrix<N,M,T>::operator=(const MatrixExpr<tmatrix<N,M,T2>, Expr>& src){
      matrix_utilities<N,M,M>::copy(src,*this);
      return *this;
    }

    template<unsigned short N,unsigned short M, typename T>
    template<typename T2>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tmatrix<N,M,T>&
    >::type
    tmatrix<N,M,T>::operator=(const tmatrix<N,M,T2>& src)
    {
      matrix_utilities<N,M,M>::copy(src,*this);
      return *this;
    }

    template<unsigned short N,unsigned short M, typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tmatrix<N,M,T>&
    >::type 
    tmatrix<N,M,T>::operator+=(const MatrixExpr<tmatrix<N,M,T2>, Expr>& src){
      matrix_utilities<N,M,M>::plusEqual(*this,src);
      return *this;
    }

    template<unsigned short N,unsigned short M, typename T>
    template<typename T2>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tmatrix<N,M,T>&
    >::type
    tmatrix<N,M,T>::operator+=(const tmatrix<N,M,T2>& src)
    {
      matrix_utilities<N,M,M>::plusEqual(*this,src);
      return *this;
    }

    template<unsigned short N,unsigned short M, typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tmatrix<N,M,T>&
    >::type 
    tmatrix<N,M,T>::operator-=(const MatrixExpr<tmatrix<N,M,T2>, Expr>& src){
      matrix_utilities<N,M,M>::minusEqual(*this,src);
      return *this;
    }

    template<unsigned short N,unsigned short M, typename T>
    template<typename T2>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tmatrix<N,M,T>&
    >::type
    tmatrix<N,M,T>::operator-=(const tmatrix<N,M,T2>& src)
    {
      matrix_utilities<N,M,M>::minusEqual(*this,src);
      return *this;
    }

    // *= operator
    template<unsigned short N,
	     unsigned short M,
	     typename T>
    template<typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      tmatrix<N,M,T>&
    >::type
    tmatrix<N,M,T>::operator*=(const T2 s)
    {
      matrix_utilities<N,M,M>::multByScalar(*this,s);
      return *this;
    }

    // /= operator
    template<unsigned short N,
	     unsigned short M,
	     typename T>
    template<typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      tmatrix<N,M,T>&
    >::type
    tmatrix<N,M,T>::operator/=(const T2 s)
    {
      matrix_utilities<N,M,M>::multByScalar(*this,(static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u))/s);
      return *this;
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 unsigned short tmatrix<N,M,T>::getNbCols(void) const
    {
      return M;
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 unsigned short tmatrix<N,M,T>::getNbRows(void) const
    {
      return N;
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 T tmatrix<N,M,T>::max(void) const
    {
      return *tfel::fsalgo::max_element<N*M>::exe(this->m);
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 T tmatrix<N,M,T>::abs_max(void) const
    {
      return std::abs(*tfel::fsalgo::max_element<N*M>::exe(this->m,absCompare<T>()));
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 void 
    tmatrix<N,M,T>::swap_rows(const unsigned short i,const unsigned short j){
      assert(i<N);
      assert(j<N);
      tfel::fsalgo::swap_ranges<M>::exe(this->m+M*i,this->m+M*j);
    }
    

    template<unsigned short N,unsigned short M, typename T>
    tmatrix<N,N,T> 
    tmatrix<N,M,T>::Id(void)
    {
      TFEL_STATIC_ASSERT(N==M);

      tmatrix<N,N,T> m2(static_cast<T>(0.));
      unsigned short i;
      for(i=0;i<N;++i){
	m2(i,i)=static_cast<T>(1.);
      }
      return m2;
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 typename tmatrix<N,M,T>::iterator tmatrix<N,M,T>::begin(void)
    {
      return this->m;
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 typename tmatrix<N,M,T>::const_iterator tmatrix<N,M,T>::begin(void) const
    {
      return this->m;
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 typename tmatrix<N,M,T>::iterator tmatrix<N,M,T>::end(void)
    {
      return this->m+N*M;
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 typename tmatrix<N,M,T>::const_iterator tmatrix<N,M,T>::end(void) const
    {
      return this->m+N*M;
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 typename tmatrix<N,M,T>::reverse_iterator tmatrix<N,M,T>::rbegin(void)
    {
      return reverse_iterator(this->m+N*M);
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 typename tmatrix<N,M,T>::const_reverse_iterator tmatrix<N,M,T>::rbegin(void) const
    {
      return const_reverse_iterator(this->m+N*M);
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 typename tmatrix<N,M,T>::reverse_iterator tmatrix<N,M,T>::rend(void)
    {
      return reverse_iterator(this->m);
    }

    template<unsigned short N,unsigned short M, typename T>
    TFEL_MATH_INLINE2 typename tmatrix<N,M,T>::const_reverse_iterator tmatrix<N,M,T>::rend(void) const
    {
      return const_reverse_iterator(this->m);
    }

    template<unsigned short N,unsigned short M, typename T>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 void tmatrix<N,M,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<N*M>::exe(src,this->m);
    }

    //template<unsigned short N,unsigned short M,typename T>
    template<unsigned short N,unsigned short M,typename T>
    TFEL_MATH_INLINE2
    tmatrix<M,N,T> transpose(const tmatrix<N,M,T> &m)
    {
      tmatrix<M,N,T> tm;
      unsigned short i,j;
      for(i=0;i<N;++i){
	for(j=0;j<M;++j){
	  tm(j,i)=m(i,j);
	}
      }
      return tm;
    }

    template<unsigned short N,unsigned short M,typename T>
    std::ostream &
    operator << (std::ostream & os, const tmatrix<N,M,T>& m)
    {
      unsigned short i,j;
      os << "[";
      for(i=0;i<N;++i){
	os << "[";
	for(j=0;j<M;++j){
	  os << m(i,j);
	  if(j!=M-1){
	    os << ",";
	  }
	}
	os << "]" ;
      }
      os << "]";
      return os;
    }

#endif

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TINY_MATRIX_IXX */

