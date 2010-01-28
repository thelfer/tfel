/*!
 * \file   ArrayRowMajorStorage.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 aoû 2006
 */

#ifndef _LIB_TFEL_ROWMAJORSTORAGE_IXX_
#define _LIB_TFEL_ROWMAJORSTORAGE_IXX_ 

#include<string>
#include<cassert>

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/IsSubClassOf.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/Array/ArrayConcept.hxx"
#include"TFEL/Math/Array/ArrayExpr.hxx"
#include"TFEL/Math/Array/ArrayRefCountedStorage.hxx"
#include"TFEL/Math/Array/ArrayRunTimeProperties.hxx"

namespace tfel{
  
  namespace math {

    template<typename T>
    struct ArrayRowMajorStorage<1u,T>
      : public ArrayRunTimeProperties<1u>,
	public ArrayRefCountedStorage<T>
    {

      static const std::string getName(void){
	return "ArrayRowMajorStorage<1u,T>";
      }

      typedef ArrayRunTimeProperties<1u> RunTimeProperties;

      ArrayRowMajorStorage()
      {}

      ArrayRowMajorStorage(const RunTimeProperties& src)
	: ArrayRunTimeProperties<1u>(src),
	  ArrayRefCountedStorage<T>(src.getNbCols())
      {}

      ArrayRowMajorStorage(const ArrayRowMajorStorage& src)
	: ArrayRunTimeProperties<1u>(src),
	  ArrayRefCountedStorage<T>(src)
      {}

      ArrayRowMajorStorage& operator=(const ArrayRowMajorStorage& src)
      {
	ArrayRunTimeProperties<1u>::operator=(src);
	ArrayRefCountedStorage<T>::operator=(src);
	return *this;
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<1u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::typetraits::IsAssignableTo<typename ArrayTraits<T_type>::NumType,T>::cond,
	void>::type
      operator=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] = expr(i);
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpPlus>::type>::cond,
	void>::type
      operator+=(ArrayRowMajorStorage<1u,T2>& a)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] += a.v[i];
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpMinus>::type>::cond,
	void>::type
      operator-=(ArrayRowMajorStorage<1u,T2>& a)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] -= a.v[i];
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpMult>::type>::cond,
	void>::type
      operator*=(ArrayRowMajorStorage<1u,T2>& a)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] *= a.v[i];
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpDiv>::type>::cond,
	void>::type
      operator/=(ArrayRowMajorStorage<1u,T2>& a)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] /= a.v[i];
	}
      }
      
      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<1u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpPlus>::type
      >::cond,void>::type
      operator+=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] += expr(i);
	}
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<1u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpMinus>::type
      >::cond,void>::type
      operator-=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] -= expr(i);
	}
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<1u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpMult>::type
      >::cond,void>::type
      operator*=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] *= expr(i);
	}
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<1u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpDiv>::type
      >::cond,void>::type
      operator/=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] /= expr(i);
	}
      }

      template<typename Scalar>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<Scalar>::cond &&
      tfel::meta::IsSameType<
	T,typename ResultType<T,Scalar,OpMult>::type
      >::cond,void>::type
      operator*=(const Scalar a)
      {
	unsigned int i;
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] *= a;
	}
      }

      template<typename Scalar>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<Scalar>::cond &&
      tfel::meta::IsSameType<
	T,typename ResultType<T,Scalar,OpDiv>::type
      >::cond,void>::type
      operator/=(const Scalar a)
      {
	unsigned int i;
	for(i=0;i<this->nb_cols;++i){
	  this->v[i] /= a;
	}
      }      

      T& operator()(const unsigned int i)
      {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->nb_cols);
#endif
	return *(this->v+i);
      }

      const T& operator()(const unsigned int i) const
      {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->nb_cols);
#endif
	return *(this->v+i);
      }

      const RunTimeProperties& getRunTimeProperties(void) const
      {
	return *this;
      }

      typedef typename ArrayRefCountedStorage<T>::value_type             value_type;
      typedef typename ArrayRefCountedStorage<T>::pointer                pointer;
      typedef typename ArrayRefCountedStorage<T>::const_pointer          const_pointer;
      typedef typename ArrayRefCountedStorage<T>::iterator               iterator;
      typedef typename ArrayRefCountedStorage<T>::const_iterator         const_iterator;
      typedef typename ArrayRefCountedStorage<T>::const_reverse_iterator const_reverse_iterator;
      typedef typename ArrayRefCountedStorage<T>::reverse_iterator       reverse_iterator;
      typedef typename ArrayRefCountedStorage<T>::reference              reference;
      typedef typename ArrayRefCountedStorage<T>::const_reference        const_reference;
      typedef typename ArrayRefCountedStorage<T>::size_type              size_type;
      typedef typename ArrayRefCountedStorage<T>::difference_type        difference_type;
    };

    template<typename T>
    struct ArrayRowMajorStorage<2u,T>
      : public ArrayRunTimeProperties<2u>,
	public ArrayRefCountedStorage<T>
    {

      static const std::string getName(void){
	return "ArrayRowMajorStorage<2u,T>";
      }

      typedef ArrayRunTimeProperties<2u> RunTimeProperties;

      ArrayRowMajorStorage()
      {}

      ArrayRowMajorStorage(const RunTimeProperties& src)
	: ArrayRunTimeProperties<2u>(src),
	  ArrayRefCountedStorage<T>((src.getNbCols())*(src.getNbRows()))
      {}

      ArrayRowMajorStorage(const ArrayRowMajorStorage& src)
	: ArrayRunTimeProperties<2u>(src),
	  ArrayRefCountedStorage<T>(src)
      {}

      ArrayRowMajorStorage& operator=(const ArrayRowMajorStorage& src)
      {
	ArrayRunTimeProperties<2u>::operator=(src);
	ArrayRefCountedStorage<T>::operator=(src);
	return *this;
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<2u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::typetraits::IsAssignableTo<typename ArrayTraits<T_type>::NumType,T>::cond,
	void
      >::type
      operator=(const ArrayExpr<T_type,Expr> &expr)
      {
	unsigned int i,j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    this->v[i*(this->nb_cols)+j] = expr(i,j);
	  }
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpPlus>::type>::cond,
	void>::type
      operator+=(ArrayRowMajorStorage<2u,T2>& a)
      {
	T*  p;
	T2* p2;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	p2=a.v;
	for(p=this->v;p<this->v+this->size;++p,++p2){
	  *p += *p2;
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpMinus>::type>::cond,
	void>::type
      operator-=(ArrayRowMajorStorage<2u,T2>& a)
      {
	T*  p;
	T2* p2;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	p2=a.v;
	for(p=this->v;p<this->v+this->size;++p,++p2){
	  *p -= *p2;
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpMult>::type>::cond,
	void>::type
      operator*=(ArrayRowMajorStorage<2u,T2>& a)
      {
	T*  p;
	T2* p2;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	p2=a.v;
	for(p=this->v;p<this->v+this->size;++p,++p2){
	  *p *= *p2;
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpDiv>::type>::cond,
	void>::type
      operator/=(ArrayRowMajorStorage<2u,T2>& a)
      {
	T*  p;
	T2* p2;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	p2=a.v;
	for(p=this->v;p<this->v+this->size;++p,++p2){
	  *p /= *p2;
	}
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<2u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpPlus>::type
      >::cond,void>::type
      operator+=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i,j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    this->v[i*(this->nb_cols)+j] += expr(i,j);
	  }
	}
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<2u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpMinus>::type
      >::cond,void>::type
      operator-=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i,j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    this->v[i*(this->nb_cols)+j] -= expr(i,j);
	  }
	}
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<2u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpMult>::type
      >::cond,void>::type
      operator*=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i,j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    this->v[i*(this->nb_cols)+j] *= expr(i,j);
	  }
	}
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<2u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpDiv>::type
      >::cond,void>::type
      operator/=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i,j;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    this->v[i*(this->nb_cols)+j] /= expr(i,j);
	  }
	}
      }

      template<typename Scalar>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<Scalar>::cond &&
      tfel::meta::IsSameType<
	T,typename ResultType<T,Scalar,OpMult>::type
      >::cond,void>::type
      operator*=(const Scalar a)
      {
	unsigned int i,j;
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    this->v[i*(this->nb_cols)+j] *= a;
	  }
	}
      }

      template<typename Scalar>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<Scalar>::cond &&
      tfel::meta::IsSameType<
	T,typename ResultType<T,Scalar,OpDiv>::type
      >::cond,void>::type
      operator/=(const Scalar a)
      {
	unsigned int i,j;
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    this->v[i*(this->nb_cols)+j] /= a;
	  }
	}
      }      
      
      T& operator()(const unsigned int i,const unsigned int j)
      {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->nb_rows);
	assert(j<this->nb_cols);
#endif
	return *(this->v+i*(this->nb_cols)+j);
      }

      const T& operator()(const unsigned int i,const unsigned int j) const
      {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->nb_rows);
	assert(j<this->nb_cols);
#endif
	return *(this->v+i*(this->nb_cols)+j);
      }

      const RunTimeProperties& getRunTimeProperties(void) const
      {
	return *this;
      }

      typedef typename ArrayRefCountedStorage<T>::value_type             value_type;
      typedef typename ArrayRefCountedStorage<T>::pointer                pointer;
      typedef typename ArrayRefCountedStorage<T>::const_pointer          const_pointer;
      typedef typename ArrayRefCountedStorage<T>::iterator               iterator;
      typedef typename ArrayRefCountedStorage<T>::const_iterator         const_iterator;
      typedef typename ArrayRefCountedStorage<T>::const_reverse_iterator const_reverse_iterator;
      typedef typename ArrayRefCountedStorage<T>::reverse_iterator       reverse_iterator;
      typedef typename ArrayRefCountedStorage<T>::reference              reference;
      typedef typename ArrayRefCountedStorage<T>::const_reference        const_reference;
      typedef typename ArrayRefCountedStorage<T>::size_type              size_type;
      typedef typename ArrayRefCountedStorage<T>::difference_type        difference_type;
    };

    template<typename T>
    struct ArrayRowMajorStorage<3u,T>
      : public ArrayRunTimeProperties<3u>,
	public ArrayRefCountedStorage<T>
    {

      static const std::string getName(void){
	return "ArrayRowMajorStorage<3u,T>";
      }

      typedef ArrayRunTimeProperties<3u> RunTimeProperties;

      ArrayRowMajorStorage()
      {}

      ArrayRowMajorStorage(const RunTimeProperties& src)
	: ArrayRunTimeProperties<3u>(src),
	  ArrayRefCountedStorage<T>((src.getNbCols())*(src.getNbRows())*(src.getNbSlices()))
      {}

      ArrayRowMajorStorage(const ArrayRowMajorStorage& src)
	: ArrayRunTimeProperties<3u>(src),
	  ArrayRefCountedStorage<T>(src)
      {}

      ArrayRowMajorStorage& operator=(const ArrayRowMajorStorage& src)
      {
	ArrayRunTimeProperties<3u>::operator=(src);
	ArrayRefCountedStorage<T>::operator=(src);
	return *this;
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<3u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::typetraits::IsAssignableTo<typename ArrayTraits<T_type>::NumType,T>::cond,
	void
      >::type
      operator=(const ArrayExpr<T_type,Expr> &expr)
      {
	unsigned int i,j,k;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    for(k=0;k<this->nb_slices;++k){
	      this->v[(i*(this->nb_cols)+j)*(this->nb_slices)+k] = expr(i,j,k);
	    }
	  }
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpPlus>::type>::cond,
	void>::type
      operator+=(ArrayRowMajorStorage<3u,T2>& a)
      {
	T*  p;
	T2* p2;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	p2=a.v;
	for(p=this->v;p<this->v+this->size;++p,++p2){
	  *p+=*p2;
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpMinus>::type>::cond,
	void>::type
      operator-=(ArrayRowMajorStorage<3u,T2>& a)
      {
	T*  p;
	T2* p2;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	p2=a.v;
	for(p=this->v;p<this->v+this->size;++p,++p2){
	  *p-=*p2;
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpMult>::type>::cond,
	void>::type
      operator*=(ArrayRowMajorStorage<3u,T2>& a)
      {
	T*  p;
	T2* p2;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	p2=a.v;
	for(p=this->v;p<this->v+this->size;++p,++p2){
	  *p *= *p2;
	}
      }

      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<T,typename ResultType<T,T2,OpDiv>::type>::cond,
	void>::type
      operator/=(ArrayRowMajorStorage<3u,T2>& a)
      {
	T*  p;
	T2* p2;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,a.getRunTimeProperties());
#endif
	p2=a.v;
	for(p=this->v;p<this->v+this->size;++p,++p2){
	  *p /= *p2;
	}
      }

      template<typename T_type,typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<3u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpPlus>::type
      >::cond,void>::type
      operator+=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i,j,k;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    for(k=0;k<this->nb_slices;++k){
	      this->v[(i*(this->nb_cols)+j)*(this->nb_slices)+k] += expr(i,j,k);
	    }
	  }
	}
      }

      template<typename T_type,typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<3u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpMinus>::type
      >::cond,void>::type
      operator-=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i,j,k;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    for(k=0;k<this->nb_slices;++k){
	      this->v[(i*(this->nb_cols)+j)*(this->nb_slices)+k] -= expr(i,j,k);
	    }
	  }
	}
      }

      template<typename T_type,typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<3u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpMult>::type
      >::cond,void>::type
      operator*=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i,j,k;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    for(k=0;k<this->nb_slices;++k){
	      this->v[(i*(this->nb_cols)+j)*(this->nb_slices)+k] *= expr(i,j,k);
	    }
	  }
	}
      }

      template<typename T_type, typename Expr>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::IsSubClassOf<T_type,ArrayRowMajorStorage<3u,typename ArrayTraits<T_type>::NumType> >::cond &&
	tfel::meta::IsSameType<
	T,typename ResultType<T,typename ArrayTraits<T_type>::NumType,OpDiv>::type
      >::cond,void>::type
      operator/=(const ArrayExpr<T_type,Expr>& expr)
      {
	unsigned int i,j,k;
#ifndef NO_RUNTIME_CHECK_BOUNDS
	RunTimeCheck<RunTimeProperties>::exe(*this,expr.getRunTimeProperties());
#endif
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    for(k=0;k<this->nb_slices;++k){
	      this->v[(i*(this->nb_cols)+j)*(this->nb_slices)+k] /= expr(i,j,k);
	    }
	  }
	}
      }

      template<typename Scalar>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<Scalar>::cond &&
      tfel::meta::IsSameType<
	T,typename ResultType<T,Scalar,OpMult>::type
      >::cond,void>::type
      operator*=(const Scalar a)
      {
	unsigned int i,j,k;
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    for(k=0;k<this->nb_slices;++k){
	      this->v[(i*(this->nb_cols)+j)*(this->nb_slices)+k] *= a;
	    }
	  }
	}
      }

      template<typename Scalar>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<Scalar>::cond &&
      tfel::meta::IsSameType<
	T,typename ResultType<T,Scalar,OpDiv>::type
      >::cond,void>::type
      operator/=(const Scalar a)
      {
	unsigned int i,j,k;
	for(i=0;i<this->nb_rows;++i){
	  for(j=0;j<this->nb_cols;++j){
	    for(k=0;k<this->nb_slices;++k){
	      this->v[(i*(this->nb_cols)+j)*(this->nb_slices)+k] /= a;
	    }
	  }
	}
      }      
            
      T& operator()(const unsigned int i,const unsigned int j,const unsigned int k)
      {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->nb_rows);
	assert(j<this->nb_cols);
	assert(k<this->nb_slices);
#endif
	return *(this->v+(i*(this->nb_cols)+j)*(this->nb_slices)+k);
      }

      const T& operator()(const unsigned int i,const unsigned int j,const unsigned int k) const
      {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->nb_rows);
	assert(j<this->nb_cols);
	assert(k<this->nb_slices);
#endif
	return *(this->v+(i*(this->nb_cols)+j)*(this->nb_slices)+k);
      }

      const RunTimeProperties& getRunTimeProperties(void) const
      {
	return *this;
      }

      typedef typename ArrayRefCountedStorage<T>::value_type             value_type;
      typedef typename ArrayRefCountedStorage<T>::pointer                pointer;
      typedef typename ArrayRefCountedStorage<T>::const_pointer          const_pointer;
      typedef typename ArrayRefCountedStorage<T>::iterator               iterator;
      typedef typename ArrayRefCountedStorage<T>::const_iterator         const_iterator;
      typedef typename ArrayRefCountedStorage<T>::const_reverse_iterator const_reverse_iterator;
      typedef typename ArrayRefCountedStorage<T>::reverse_iterator       reverse_iterator;
      typedef typename ArrayRefCountedStorage<T>::reference              reference;
      typedef typename ArrayRefCountedStorage<T>::const_reference        const_reference;
      typedef typename ArrayRefCountedStorage<T>::size_type              size_type;
      typedef typename ArrayRefCountedStorage<T>::difference_type        difference_type;
    };

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_ROWMAJORSTORAGE_IXX */

