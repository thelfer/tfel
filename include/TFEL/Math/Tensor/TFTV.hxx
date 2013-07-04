/*!
 * \file   TFTV.hxx
 * \brief  TensorFromTVectorView
 * 
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_TFTV_HXX_
#define _LIB_TFEL_MATH_TFTV_HXX_ 

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

    /*!
     *  Tensor From Tiny Vector
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T>
    struct TFTVExpr
    {}; // end of struct TFTVExpr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T>
    struct TensorExpr<tensor<N,T>,TFTVExpr<N,Mn,In,T> >
      : public TensorConcept<TensorExpr<tensor<N,T>,TFTVExpr<N,Mn,In,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      TensorExpr(tvector<Mn,T>& v_)
	: v(v_)
      {} // end of TensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->v(In+i);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->v(In+i);
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
	VectorUtilities<TensorDimeToSize<N>::value>::PlusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&
      >::type
      operator+=(const TensorExpr<tensor<N,T2>,Expr>& s){
	VectorUtilities<TensorDimeToSize<N>::value>::PlusEqual(*this,s);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&
      >::type
      operator-=(const tensor<N,T2>& s){
	VectorUtilities<TensorDimeToSize<N>::value>::MinusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	TensorExpr&
      >::type
      operator-=(const TensorExpr<tensor<N,T2>,Expr>& s){
	VectorUtilities<TensorDimeToSize<N>::value>::MinusEqual(*this,s);
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

      tvector<Mn,T>& v;

    private:

      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      TFEL_STATIC_ASSERT((In<Mn));
      TFEL_STATIC_ASSERT((TensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct TensorExpr


    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T = double>
    struct TFTV
    {
      typedef TensorExpr<tensor<N,T>,TFTVExpr<N,Mn,In,T> > type;
    }; // end of struct TFTV

    // Serialisation operator
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T>
    std::ostream&
    operator << (std::ostream & os,
		 const TensorExpr<tensor<N,T>,TFTVExpr<N,Mn,In,T> >& s)
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

#endif /* _LIB_TFEL_MATH_TFTV_HXX */

