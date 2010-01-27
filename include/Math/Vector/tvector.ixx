/*!
 * \file   tvector.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   28 Jun 2006
 */

#ifndef _LIB_TFEL_TINY_VECTOR_IXX_
#define _LIB_TFEL_TINY_VECTOR_IXX_ 

#include <cassert>
#include <iterator>

#include "FSAlgorithm/FSAlgorithm.hxx"
#include "Math/General/Complex.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/General/DotProduct.hxx"
#include "Math/functions.hxx"
#include "Math/Vector/VectorUtilities.hxx"

namespace tfel{
  
  namespace math{

#ifndef DOXYGENSPECIFIC

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE tvector<N,T>::tvector()
    {}

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE tvector<N,T>::tvector(const T init)
    {
      tfel::fsalgo::fill<N>::exe(this->v,init);
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE tvector<N,T>::tvector(const T *const init)
    {
      tfel::fsalgo::copy<N>::exe(init,this->v);
    }
    
    template<unsigned short N, typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE tvector<N,T>::tvector(const VectorExpr<tvector<N,T2>, Expr>& src)
    {
      TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<T2,T>::cond));
      vectorToTab<N>::exe(src,this->v);
    } // end of tvector<N,T>::tvector(const VectorExpr<tvector<N,T2>, Expr>&)

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE const T& 
    tvector<N,T>::operator()(const unsigned short i) const
    {
      assert(i<N);
      return v[i];
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE T& 
    tvector<N,T>::operator()(const unsigned short i)
    {
      assert(i<N);
      return v[i];
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE const T& 
    tvector<N,T>::operator[](const unsigned short i) const
    {
      assert(i<N);
      return v[i];
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE T& 
    tvector<N,T>::operator[](const unsigned short i)
    {
      assert(i<N);
      return v[i];
    }

    template<unsigned short N, typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tvector<N,T>&
    >::type 
    tvector<N,T>::operator=(const VectorExpr<tvector<N,T2>, Expr>& src){
      vectorToTab<N>::exe(src,this->v);
      return *this;
    }

    template<unsigned short N, typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tvector<N,T>&
    >::type 
    tvector<N,T>::operator+=(const VectorExpr<tvector<N,T2>, Expr>& src){
      VectorUtilities<N>::PlusEqual(*this,src);
      return *this;
    }

    template<unsigned short N, typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tvector<N,T>&
    >::type 
    tvector<N,T>::operator-=(const VectorExpr<tvector<N,T2>, Expr>& src){
      VectorUtilities<N>::MinusEqual(*this,src);
      return *this;
    }
    
    template<unsigned short N, typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename 
    tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tvector<N,T>&
    >::type 
    tvector<N,T>::operator=(const tvector<N,T2>& src){
      vectorToTab<N>::exe(src,this->v);
      return *this;
    }

    template<unsigned short N, typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename 
    tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tvector<N,T>&
    >::type 
    tvector<N,T>::operator+=(const tvector<N,T2>& src){
      VectorUtilities<N>::PlusEqual(*this,src);
      return *this;
    }

    template<unsigned short N, typename T>
    template<typename T2>
    TFEL_MATH_INLINE typename 
    tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      tvector<N,T>&
    >::type 
    tvector<N,T>::operator-=(const tvector<N,T2>& src){
      VectorUtilities<N>::MinusEqual(*this,src);
      return *this;
    }

    // *= operator
    template<unsigned short N,typename T>
    template<typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      tvector<N,T>&
    >::type
    tvector<N,T>::operator*=(const T2 s)
    {
      VectorUtilities<N>::scale(*this,s);
      return *this;
    }

    // /= operator
    template<unsigned short N,typename T>
    template<typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
      tvector<N,T>&
    >::type
    tvector<N,T>::operator/=(const T2 s)
    {
      VectorUtilities<N>::scale(*this,(static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u))/s);
      return *this;
    }
    

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE
    const typename tvector<N,T>::RunTimeProperties
    tvector<N,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    }
    
    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tvector<N,T>::iterator 
    tvector<N,T>::begin(void)
    {
      return this->v;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tvector<N,T>::const_iterator 
    tvector<N,T>::begin(void) const
    {
      return this->v;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tvector<N,T>::iterator 
    tvector<N,T>::end(void)
    {
      return this->v+N;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tvector<N,T>::const_iterator 
    tvector<N,T>::end(void) const
    {
      return this->v+N;
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tvector<N,T>::reverse_iterator 
    tvector<N,T>::rbegin(void)
    {
      return reverse_iterator(this->v+N);
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tvector<N,T>::const_reverse_iterator 
    tvector<N,T>::rbegin(void) const
    {
      return const_reverse_iterator(this->v+N);
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tvector<N,T>::reverse_iterator 
    tvector<N,T>::rend(void)
    {
      return reverse_iterator(this->v);
    }

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tvector<N,T>::const_reverse_iterator 
    tvector<N,T>::rend(void) const
    {
      return const_reverse_iterator(this->v);
    }

    template<unsigned short N, typename T>
    template<typename InputIterator>
    TFEL_MATH_INLINE2
    void
    tvector<N,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<N>::exe(src,this->v);
    }

    template<unsigned short N, typename T>
    unsigned short
    tvector<N,T>::size(void) const
    {
      return N;
    }

    // Norm2
    template<unsigned short N,typename T>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      typename tfel::typetraits::RealPartType<T>::type
    >::type
    norm(const tvector<N,T>& vec)
    {
      typedef typename ResultType<T,T,OpMult>::type squareT;
      return sqrt(real(dotProduct<N>::exe(vec.begin(),vec.begin(),squareT(0u))));
    }

    // inner_product
    template<unsigned short N,typename T,typename T2>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond&&
    tfel::typetraits::IsScalar<T2>::cond,
      typename ResultType<T,T2,OpMult>::type
    >::type
    operator | (const tvector<N,T>& vec, const tvector<N,T2>& vec2)
    {
      typedef typename ResultType<T,T2,OpMult>::type MultRes;
      return dotProduct<N>::exe(vec.begin(),vec2.begin(),MultRes(0u));
    }

    template<unsigned short N,typename T>
    std::ostream &
    operator << (std::ostream & os, const tvector<N,T>& s)
    {
      os << "( ";
      tfel::fsalgo::copy<N>::exe(s.begin(),std::ostream_iterator<T>(os," "));
      os << ")";

      return os;
    }

#endif

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TINY_VECTOR_IXX */

