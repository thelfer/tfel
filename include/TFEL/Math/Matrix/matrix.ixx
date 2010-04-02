/*!
 * \file   matrix.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   05 oct 2006
 */

#ifndef _LIB_TFEL_MATRIX_IXX_
#define _LIB_TFEL_MATRIX_IXX_ 

#include"cassert"
#include"TFEL/Math/Function/Power.hxx"


namespace tfel{

  namespace math{

    template<typename ValueType>
    matrix<ValueType>::matrix(const matrix<ValueType>& src)
      : tfel::math::vector<ValueType>(src),
	MatrixProperties<ValueType>(src)
    {}

    template<typename ValueType>
    matrix<ValueType>::matrix(const typename matrix<ValueType>::size_type n,
			      const typename matrix<ValueType>::size_type m)
      : tfel::math::vector<ValueType>(n*m),
	MatrixProperties<ValueType>(n,m)
    {}

    template<typename ValueType>
    matrix<ValueType>::matrix(const typename matrix<ValueType>::size_type n,
			      const typename matrix<ValueType>::size_type m,
			      const ValueType& v)
      : tfel::math::vector<ValueType>(n*m,v),
	MatrixProperties<ValueType>(n,m)
    {}

    template<typename ValueType>
    matrix<ValueType>&
    matrix<ValueType>::operator=(const matrix<ValueType>& src)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   src.getRunTimeProperties());
#endif
      tfel::math::vector<ValueType>::operator=(src);
      return *this;
    }

    template<typename ValueType>
    matrix<ValueType>&
    matrix<ValueType>::operator+=(const matrix<ValueType>& src)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   src.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) += src(i,j);
	}
      }
      return *this;
    }

    template<typename ValueType>
    matrix<ValueType>&
    matrix<ValueType>::operator-=(const matrix<ValueType>& src)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   src.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) -= src(i,j);
	}
      }
      return *this;
    }

    template<typename ValueType>
    ValueType& 
    matrix<ValueType>::operator()(const typename matrix<ValueType>::size_type i,
				  const typename matrix<ValueType>::size_type j)
    {
      return tfel::math::vector<ValueType>::operator[](i*(this->nb_cols)+j);
    }
    
    template<typename ValueType>
    const ValueType& 
    matrix<ValueType>::operator()(const typename matrix<ValueType>::size_type i,
				  const typename matrix<ValueType>::size_type j) const
    {
      return tfel::math::vector<ValueType>::operator[](i*(this->nb_cols)+j);
    }

    template<typename ValueType>
    template<typename ValueType2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
      matrix<ValueType>&
    >::type
    matrix<ValueType>::operator=(const MatrixExpr<matrix<ValueType2>,Expr>& expr)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   expr.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) = expr(i,j);
	}
      }
      return *this;
    }

    template<typename ValueType>
    template<typename ValueType2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
      matrix<ValueType>&
    >::type
    matrix<ValueType>::operator+=(const MatrixExpr<matrix<ValueType2>,Expr>& expr)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   expr.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) += expr(i,j);
	}
      }
      return *this;
    }

    template<typename ValueType>
    template<typename ValueType2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
      matrix<ValueType>&
    >::type
    matrix<ValueType>::operator-=(const MatrixExpr<matrix<ValueType2>,Expr>& expr)
    {
      size_type i;
      size_type j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   expr.getRunTimeProperties());
#endif
      for(i=0;i<this->nb_rows;++i){
	for(j=0;j<this->nb_cols;++j){
	  this->operator()(i,j) -= expr(i,j);
	}
      }
      return *this;
    }

    template<typename ValueType>
    void matrix<ValueType>::swap(matrix<ValueType>& a)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),
					   a.getRunTimeProperties());
#endif
      tfel::math::vector<ValueType>::swap(a);
    }

    template<typename ValueType>
    TFEL_MATH_INLINE
    typename matrix<ValueType>::size_type
    matrix<ValueType>::getNbRows() const
    {
      return this->nb_rows;
    }
    
    template<typename ValueType>
    TFEL_MATH_INLINE
    typename matrix<ValueType>::size_type
    matrix<ValueType>::getNbCols() const
    {
      return this->nb_cols;
    }

    template<typename T>
    TFEL_MATH_INLINE
    const typename matrix<T>::RunTimeProperties&
    matrix<T>::getRunTimeProperties(void) const
    {
      return *this;
    } // end of matrix<T>::getRunTimeProperties(void) const

    template<typename T>
    MatrixProperties<T>::MatrixProperties(const typename MatrixProperties<T>::IndexType n,
					  const typename MatrixProperties<T>::IndexType m)
      :	nb_rows(n),
	nb_cols(m)
    {} // end of MatrixProperties<T>::MatrixProperties
    
    template<typename T>
    MatrixProperties<T>::MatrixProperties(const MatrixProperties& src)
      : nb_rows(src.nb_rows),
	nb_cols(src.nb_cols)
    {} // end of MatrixProperties<T>::MatrixProperties

    template<typename T>
    MatrixProperties<T>&
    MatrixProperties<T>::operator=(const MatrixProperties<T>& s)
    {
      this->nb_rows = s.nb_rows;
      this->nb_cols = s.nb_cols;
      return *this;
    }  // end of MatrixProperties<T>::operator=

    template<typename T>
    bool
    MatrixProperties<T>::operator == (const MatrixProperties& s) const
    {
      return (this->nb_rows==s.nb_rows)&&(this->nb_cols==s.nb_cols);
    } // end of MatrixProperties<T>::operator == 

    template<typename T>
    bool
    MatrixProperties<T>::operator != (const MatrixProperties& s) const
    {
      return (this->nb_rows!=s.nb_rows)||(this->nb_cols!=s.nb_cols);
    } // end of MatrixProperties<T>::operator == 

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATRIX_IXX */
