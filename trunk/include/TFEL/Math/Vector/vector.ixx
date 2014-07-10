/*!
 * \file   vector.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   05 oct 2006
 */

#ifndef _LIB_TFEL_VECTOR_IXX_
#define _LIB_TFEL_VECTOR_IXX_ 

#include<cassert>

namespace tfel{

  namespace math{

    template<typename T>
    vector<T>::vector()
      : std::vector<T>()
    {}

    template<typename T>
    vector<T>::vector(const vector<T>& src)
      : std::vector<T>(src)
    {}

    template<typename T>
    vector<T>::vector(const typename vector<T>::size_type s)
      : std::vector<T>(s)
    {}

    template<typename T>
    vector<T>::vector(const typename vector<T>::size_type s, const T& v)
      : std::vector<T>(s,v)
    {}

    template<typename T>
    template<typename InputIterator>
    vector<T>::vector(const InputIterator begin_, const InputIterator end_)
      : std::vector<T>(begin_,end_)
    {}

    template<typename T>
    vector<T>&
    vector<T>::operator=(const vector<T>& src)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(),src.size());
#endif
      std::vector<T>::operator=(src);
      return *this;
    }

    template<typename T>
    vector<T>&
    vector<T>::operator+=(const vector<T>& src)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(),src.size());
#endif
      for(i=0;i<this->size();++i){
	std::vector<T>::operator[](i) += src(i);
      }
      return *this;
    }

    template<typename T>
    vector<T>&
    vector<T>::operator-=(const vector<T>& src)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(),src.size());
#endif
      for(i=0;i<this->size();++i){
	std::vector<T>::operator[](i) -= src(i);
      }
      return *this;
    }

    template<typename T>
    TFEL_MATH_INLINE2 const typename vector<T>::RunTimeProperties 
    vector<T>::getRunTimeProperties(void) const
    {
      return std::vector<T>::size();
    }

    template<typename T>
    TFEL_MATH_INLINE T& 
    vector<T>::operator()(const typename vector<T>::size_type i)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->size());
#endif
      return std::vector<T>::operator[](i);
    }

    template<typename T>
    TFEL_MATH_INLINE const T& 
    vector<T>::operator()(const typename vector<T>::size_type i) const
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->size());
#endif
      return std::vector<T>::operator[](i);
    }

    template<typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      vector<T>&
    >::type
    vector<T>::operator=(const VectorExpr<vector<T2>,Expr>& expr)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),expr.getRunTimeProperties());
#endif
      for(i=0;i<this->size();++i){
	std::vector<T>::operator[](i) = expr(i);
      }
      return *this;
    }

    template<typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      vector<T>&
    >::type
    vector<T>::operator+=(const VectorExpr<vector<T2>,Expr>& expr)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),expr.getRunTimeProperties());
#endif
      for(i=0;i<this->size();++i){
	std::vector<T>::operator[](i) += expr(i);
      }
      return *this;
    }

    template<typename T>
    template<typename T2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      vector<T>&
    >::type
    vector<T>::operator-=(const VectorExpr<vector<T2>,Expr>& expr)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),expr.getRunTimeProperties());
#endif
      for(i=0;i<this->size();++i){
	std::vector<T>::operator[](i) -= expr(i);
      }
      return *this;
    }

    template<typename T>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 void vector<T>::copy(const InputIterator b,const InputIterator e)
    {
      std::copy(b,e,this->v);
    }

    template<typename T>
    void vector<T>::swap(vector<T>& a)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(),a.size());
#endif
      std::vector<T>::swap(a);
    }

    template<typename T>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      typename tfel::typetraits::RealPartType<T>::type
    >::type
    norm(const vector<T>& vec)
    {
      T n(0);
      typename vector<T>::size_type i;
      for(i=0;i!=vec.size();++i){
	const T v = vec(i);
	n += v*v;
      }
      return sqrt(real(n));
    }

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Vector/vectorResultType.hxx"

#endif /* _LIB_TFEL_VECTOR_IXX */

