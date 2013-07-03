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

#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/DotProduct.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"

namespace tfel{
  
  namespace math{

#ifndef DOXYGENSPECIFIC

    template<typename Child,unsigned short N,typename T>    template<typename T2,typename Expr>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      Child&
    >::type 
    tvector_base<Child,N,T>::operator=(const VectorExpr<tvector<N,T2>, Expr>& src){
      vectorToTab<N>::exe(src,static_cast<Child&>(*this));
      return static_cast<Child&>(*this);
    }

    template<typename Child,unsigned short N,typename T>    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      Child&
    >::type 
    tvector_base<Child,N,T>::operator=(const tvector<N,T2>& src){
      vectorToTab<N>::exe(src,static_cast<Child&>(*this));
      return static_cast<Child&>(*this);
    }

    template<typename Child,unsigned short N,typename T>    template<typename T2,typename Expr>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      Child&
    >::type 
    tvector_base<Child,N,T>::operator+=(const VectorExpr<tvector<N,T2>, Expr>& src){
      VectorUtilities<N>::PlusEqual(static_cast<Child&>(*this),src);
      return static_cast<Child&>(*this);
    }

    template<typename Child,unsigned short N,typename T>    template<typename T2,typename Expr>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      Child&
    >::type 
    tvector_base<Child,N,T>::operator-=(const VectorExpr<tvector<N,T2>, Expr>& src){
      VectorUtilities<N>::MinusEqual(static_cast<Child&>(*this),src);
      return static_cast<Child&>(*this);
    }
    
    template<typename Child,unsigned short N,typename T>    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      Child&
    >::type 
    tvector_base<Child,N,T>::operator+=(const tvector<N,T2>& src){
      VectorUtilities<N>::PlusEqual(static_cast<Child&>(*this),src);
      return static_cast<Child&>(*this);
    }

    template<typename Child,unsigned short N,typename T>    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      Child&
    >::type 
    tvector_base<Child,N,T>::operator-=(const tvector<N,T2>& src){
      VectorUtilities<N>::MinusEqual(static_cast<Child&>(*this),src);
      return static_cast<Child&>(*this);
    }

    // *= operator
    template<typename Child,unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      IsTVectorScalarOperationValid<T,T2,OpMult>::cond,
      Child&
    >::type
    tvector_base<Child,N,T>::operator*=(const T2 s)
    {
      VectorUtilities<N>::scale(static_cast<Child&>(*this),s);
      return static_cast<Child&>(*this);
    }

    // /= operator
    template<typename Child,unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      IsTVectorScalarOperationValid<T,T2,OpDiv>::cond,
      Child&
    >::type
    tvector_base<Child,N,T>::operator/=(const T2 s)
    {
      VectorUtilities<N>::scale(static_cast<Child&>(*this),
				(static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u))/s);
      return static_cast<Child&>(*this);
    }

    template<unsigned short N, typename T>
    tvector<N,T>::tvector()
    {}

    template<unsigned short N, typename T>
    tvector<N,T>::tvector(const T init)
    {
      tfel::fsalgo::fill<N>::exe(this->v,init);
    }

    template<unsigned short N, typename T>
    tvector<N,T>::tvector(const T *const init)
    {
      tfel::fsalgo::copy<N>::exe(init,this->v);
    }
    
    template<unsigned short N, typename T>
    template<typename T2,typename Expr>
    tvector<N,T>::tvector(const VectorExpr<tvector<N,T2>, Expr>& src)
    {
      TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<T2,T>::cond));
      vectorToTab<N>::exe(src,this->v);
    } // end of tvector<N,T>::tvector(const VectorExpr<tvector<N,T2>, Expr>&)

    template<unsigned short N, typename T>
    const T& 
    tvector<N,T>::operator()(const unsigned short i) const
    {
      assert(i<N);
      return v[i];
    }

    template<unsigned short N, typename T>
    T& 
    tvector<N,T>::operator()(const unsigned short i)
    {
      assert(i<N);
      return v[i];
    }

    template<unsigned short N, typename T>
    const T& 
    tvector<N,T>::operator[](const unsigned short i) const
    {
      assert(i<N);
      return v[i];
    }

    template<unsigned short N, typename T>
    T& 
    tvector<N,T>::operator[](const unsigned short i)
    {
      assert(i<N);
      return v[i];
    }
    
    template<unsigned short N, typename T>
    typename tvector<N,T>::RunTimeProperties
    tvector<N,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    }
    
    template<unsigned short N, typename T>
    typename tvector<N,T>::iterator 
    tvector<N,T>::begin(void)
    {
      return this->v;
    }

    template<unsigned short N, typename T>
    typename tvector<N,T>::const_iterator 
    tvector<N,T>::begin(void) const
    {
      return this->v;
    }

    template<unsigned short N, typename T>
    typename tvector<N,T>::iterator 
    tvector<N,T>::end(void)
    {
      return this->v+N;
    }

    template<unsigned short N, typename T>
    typename tvector<N,T>::const_iterator 
    tvector<N,T>::end(void) const
    {
      return this->v+N;
    }

    template<unsigned short N, typename T>
    typename tvector<N,T>::reverse_iterator 
    tvector<N,T>::rbegin(void)
    {
      return reverse_iterator(this->v+N);
    }

    template<unsigned short N, typename T>
    typename tvector<N,T>::const_reverse_iterator 
    tvector<N,T>::rbegin(void) const
    {
      return const_reverse_iterator(this->v+N);
    }

    template<unsigned short N, typename T>
    typename tvector<N,T>::reverse_iterator 
    tvector<N,T>::rend(void)
    {
      return reverse_iterator(this->v);
    }

    template<unsigned short N, typename T>
    typename tvector<N,T>::const_reverse_iterator 
    tvector<N,T>::rend(void) const
    {
      return const_reverse_iterator(this->v);
    }

    template<unsigned short N, typename T>
    template<typename InputIterator>
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
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      typename tfel::typetraits::RealPartType<T>::type
    >::type
    norm(const tvector<N,T>& vec)
    {
      typedef typename ResultType<T,T,OpMult>::type squareT;
      return sqrt(real(dotProduct<N>::exe(vec.begin(),vec.begin(),squareT(0u))));
    }

    template<unsigned short N,typename T>
    typename tfel::typetraits::AbsType<T>::type
    abs(const tvector<N,T>& v)
    {
      using namespace tfel::fsalgo;
      AbsSum<T> a;
      for_each<N>::exe(v.begin(),a);
      return a.result;
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

    template<typename T>
    tvector<1u,T>
    makeTVector1D(const T v)
    {
      return tvector<1u,T>(v);
    } // end of makeTVector1D

    template<typename T>
    tvector<2u,T>
    makeTVector2D(const T v1,
		  const T v2)
    {
      tvector<2u,T> r;
      r[0] = v1;
      r[1] = v2;
      return r;
    } // end of makeTVector2D
    
    template<typename T>
    tvector<3u,T>
    makeTVector3D(const T v1,
		  const T v2,
		  const T v3)
    {
      tvector<3u,T> r;
      r[0] = v1;
      r[1] = v2;
      r[2] = v3;
      return r;
    } // end of makeTVector3D

    template<typename T>
    tvector<3u,T>
    cross_product(const tvector<2u,T>& v1,
		  const tvector<2u,T>& v2)
    {
      tvector<3u,T> v3;
      v3[0] = T(0);
      v3[1] = T(0);
      v3[2] = v1[0]*v2[1]-v1[1]*v2[0];
      return v3;
    } // end of cross_product

    template<typename T>
    tvector<3u,T>
    cross_product(const tvector<3u,T>& v1,
		  const tvector<3u,T>& v2)
    {
      tvector<3u,T> v3;
      v3[0] = v1[1]*v2[2]-v1[2]*v2[1];
      v3[1] = v1[2]*v2[0]-v1[0]*v2[2];
      v3[2] = v1[0]*v2[1]-v1[1]*v2[0];
      return v3;
    } // end of cross_product

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TINY_VECTOR_IXX */

