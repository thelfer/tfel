/*!
 * \file   TFTMCV.hxx
 * \brief  
 * Tensor From Matric Column View
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_TFTMCV_HXX_
#define _LIB_TFEL_MATH_TFTMCV_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/Tensor/TensorExpr.hxx"
#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel
{
  
  namespace math
  {

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct TFTMCVExpr
    {}; // end of struct TFTMCVExpr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct TensorExpr<tensor<N,T>,TFTMCVExpr<N,Mn,Mm,In,Im,T> >
      : public TensorConcept<TensorExpr<tensor<N,T>,TFTMCVExpr<N,Mn,Mm,In,Im,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      TensorExpr(tmatrix<Mn,Mm,T>& m_)
	: m(m_)
      {} // end of TensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->m(In+i,Im);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->m(In+i,Im);
      } // end of operator()

      /*!
       * Assignement operator
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&>::type
      operator=(const tensor<N,T2>& s){
	VectorUtilities<TensorDimeToSize<N>::value>::copy(s,*this);
	return *this;
      }
      
      /*!
       * Assignement operator
       */
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&
      >::type
      operator=(const TensorExpr<tensor<N,T2>,Expr>& s)
      {
	VectorUtilities<TensorDimeToSize<N>::value>::copy(s,*this);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&
      >::type
      operator+=(const tensor<N,T2>& s){
	VectorUtilities<TensorDimeToSize<N>::value>::PlusEqual(s,*this);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&
      >::type
      operator+=(const TensorExpr<tensor<N,T2>,Expr>& s){
	VectorUtilities<TensorDimeToSize<N>::value>::PlusEqual(s,*this);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&
      >::type
      operator-=(const tensor<N,T2>& s){
	VectorUtilities<TensorDimeToSize<N>::value>::MinusEqual(s,*this);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&
      >::type
      operator-=(const TensorExpr<tensor<N,T2>,Expr>& s){
	VectorUtilities<TensorDimeToSize<N>::value>::MinusEqual(s,*this);
	return *this;
      }

      /*!
       * operator*=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	TensorExpr&
      >::type
      operator*=(const T2 a){
	VectorUtilities<N>::scale(*this,a);
	return *this;
      }

      /*!
       * operator/=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	TensorExpr&
      >::type
      operator/=(const T2 a){
	VectorUtilities<N>::scale(*this,1/a);
	return *this;
      }

    protected:

      tmatrix<Mn,Mm,T>& m;

    private:

      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      TFEL_STATIC_ASSERT((Im<Mm));
      TFEL_STATIC_ASSERT((TensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct TensorExpr


    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T = double>
    struct TFTMCV
    {
      typedef TensorExpr<tensor<N,T>,TFTMCVExpr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct TFTMCV

    // Serialisation operator
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    std::ostream&
    operator << (std::ostream & os,const TensorExpr<tensor<N,T>,TFTMCVExpr<N,Mn,Mm,In,Im,T> >& s)
    {
      os << "[ ";
      for(unsigned short i=0;i<TensorDimeToSize<N>::value;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TFTMCV_HXX */

