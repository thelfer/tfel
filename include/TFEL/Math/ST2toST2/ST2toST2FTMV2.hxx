/*!
 * \file   ST2toST2FTMV2.hxx
 * \brief  ST2toST2FromTMatrixView
 * 
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_ST2toST2FTMV2_HXX_
#define _LIB_TFEL_MATH_ST2toST2FTMV2_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Expr.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel
{
  
  namespace math
  {

    /*!
     * ST2toST2 From Tiny Matrix
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct ST2toST2FTMV2Expr
    {}; // end of struct ST2toST2FTMV2Expr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct ST2toST2Expr<st2tost2<N,T>,
			ST2toST2FTMV2Expr<N,Mn,Mm,In,Im,T> >
	: public ST2toST2Concept<ST2toST2Expr<st2tost2<N,T>,
					      ST2toST2FTMV2Expr<N,Mn,Mm,In,Im,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      ST2toST2Expr(tmatrix<Mn,Mm,T>& m_,
		   const unsigned short i_,
		   const unsigned short j_)
	: m(m_),
	  oi(i_),
	  oj(j_)
      {} // end of ST2toST2Expr

      const T&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	assert(i<StensorDimeToSize<N>::value);
	assert(j<StensorDimeToSize<N>::value);
	return this->m(In+this->oi*StensorDimeToSize<N>::value+i,
		       Im+this->oj*StensorDimeToSize<N>::value+j);
      } // end of operator() const

      T&
      operator()(const unsigned short i,
		 const unsigned short j)
      {
	assert(i<StensorDimeToSize<N>::value);
	assert(j<StensorDimeToSize<N>::value);
	return this->m(In+this->oi*StensorDimeToSize<N>::value+i,
		       Im+this->oj*StensorDimeToSize<N>::value+j);
      } // end of operator()

      /*!
       * Assignement operator
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	ST2toST2Expr&>::type
      operator=(const st2tost2<N,T2>& s){
	matrix_utilities<StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value>::copy(s,*this);
	return *this;
      }
      
      /*!
       * Assignement operator
       */
    template<typename T2,typename Expr>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      ST2toST2Expr&
      >::type
      operator=(const ST2toST2Expr<st2tost2<N,T2>,Expr>& s)
      {
	matrix_utilities<StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value,
                         StensorDimeToSize<N>::value>::copy(s,*this);
        return *this;
      }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	ST2toST2Expr&
	>::type
      operator+=(const st2tost2<N,T2>& s){
	matrix_utilities<StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value>::plusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
    template<typename T2,typename Expr>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<T2,T>::cond,
      ST2toST2Expr&
      >::type
      operator+=(const ST2toST2Expr<st2tost2<N,T2>,Expr>& s){
      matrix_utilities<StensorDimeToSize<N>::value,
	               StensorDimeToSize<N>::value,
	               StensorDimeToSize<N>::value>::plusEqual(*this,s);
      return *this;
    }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	ST2toST2Expr&
	>::type
      operator-=(const st2tost2<N,T2>& s){
	matrix_utilities<StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value>::minusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	ST2toST2Expr&
	>::type
      operator-=(const ST2toST2Expr<st2tost2<N,T2>,Expr>& s){
	matrix_utilities<StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value>::minusEqual(*this,s);
	return *this;
      }

      /*!
       * operator*=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
	tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	ST2toST2Expr&
	>::type
      operator*=(const T2 a){
	matrix_utilities<StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value>::multByScalar(*this,a);
        return *this;
      }

      /*!
       * operator/=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
	tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	ST2toST2Expr&
	>::type
      operator/=(const T2 a){
	matrix_utilities<StensorDimeToSize<N>::value,
	                 StensorDimeToSize<N>::value,
                         StensorDimeToSize<N>::value>::multByScalar(*this,1/a);
        return *this;
      }

    protected:

      tmatrix<Mn,Mm,T>& m;
      unsigned short oi;
      unsigned short oj;

    private:
      
      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      TFEL_STATIC_ASSERT((In<Mn));
      TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mm-Im));
      
    }; // end of struct ST2toST2Expr
    
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T = double>
    struct ST2toST2FTMV2
    {
      typedef ST2toST2Expr<st2tost2<N,T>,ST2toST2FTMV2Expr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct ST2toST2FTMV2
    
    // Serialisation operator
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    std::ostream&
    operator << (std::ostream & os,
		 const ST2toST2Expr<st2tost2<N,T>,ST2toST2FTMV2Expr<N,Mn,Mm,In,Im,T> >& s)
    {
      unsigned short i;
      unsigned short j;
      os << "[";
      for(i=0;i<StensorDimeToSize<N>::value;++i){
	if(i!=0){
	  os << " [";
	} else {
	  os << "[";
	}
	for(j=0;j<StensorDimeToSize<N>::value;++j){
	  os << s(i,j);
	  if(j!=StensorDimeToSize<N>::value-1){
	    os << ",";
	  }
	}
	if(i!=StensorDimeToSize<N>::value-1){
	  os << "]\n";
	} else {
	  os << "]";
	}
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_ST2toST2FTMV2_HXX */

