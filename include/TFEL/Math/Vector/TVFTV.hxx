/*!
 * \file   TVFTV.hxx
 * \brief  TinyVectorFromTVectorView
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_TVFTV_HXX_
#define _LIB_TFEL_MATH_TVFTV_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Vector/VectorExpr.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/tvector.hxx"

namespace tfel
{
  
  namespace math
  {

    /*!
     *  Tiny vector from Tiny Vector view expression
     * \param[in] N  : number of object holed
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the beginning in the underlying vector
     * \param[in] T  : underlying type
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T>
    struct TVFTVExpr
    {
      /*!
       * numerical type holded by the expression
       */
      typedef T NumType;
    }; // end of struct TVFTVExpr

    /*!
     * \param[in] N : size of the tvector holed
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the beginning in the underlying vector
     * \param[in] T  : underlying type
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T>
    struct VectorExpr<tvector<N,T>,TVFTVExpr<N,Mn,In,T> >
      : public VectorConcept<VectorExpr<tvector<N,T>,TVFTVExpr<N,Mn,In,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      VectorExpr(tvector<Mn,T>& v_)
	: v(v_)
      {} // end of VectorExpr

      /*
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RunTimeProperties();
      }

      const T&
      operator[](const unsigned short i) const
      {
	return this->v[In+i];
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->v[In+i];
      } // end of operator[]

      const T&
      operator()(const unsigned short i) const
      {
	return this->v[In+i];
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->v[In+i];
      } // end of operator()

      /*!
       * Assignement operator
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&>::type
      operator=(const tvector<N,T2 >& s){
	VectorUtilities<N>::copy(s,*this);
	return *this;
      }
      
      /*!
       * Assignement operator
       */
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator=(const VectorExpr<tvector<N,T2 >,Expr>& s)
      {
	VectorUtilities<N>::copy(s,*this);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator+=(const tvector<N,T2 >& s){
	VectorUtilities<N>::PlusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator+=(const VectorExpr<tvector<N,T2 >,Expr>& s){
	VectorUtilities<N>::PlusEqual(*this,s);
	return *this;
      }

      // Assignement operator
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator-=(const tvector<N,T2 >& s){
	VectorUtilities<N>::MinusEqual(*this,s);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	VectorExpr&
      >::type
      operator-=(const VectorExpr<tvector<N,T2 >,Expr>& s){
	VectorUtilities<N>::MinusEqual(*this,s);
	return *this;
      }

      /*!
       * operator*=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	VectorExpr&
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
	VectorExpr&
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
      TFEL_STATIC_ASSERT((In<Mn));
      TFEL_STATIC_ASSERT((N<=Mn-In));

    }; // end of struct VectorExpr


    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T = double>
    struct TVFTV
    {
      typedef VectorExpr<tvector<N,T>,TVFTVExpr<N,Mn,In,T> > type;
    }; // end of struct TVFTV

    // Serialisation operator
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T>
    std::ostream&
    operator << (std::ostream & os,
		 const VectorExpr<tvector<N,T>,TVFTVExpr<N,Mn,In,T> >& s)
    {
      os << "[ ";
      for(unsigned short i=0;i<N;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math

  namespace typetraits{

    template<unsigned short N,
  	     unsigned short Mn,
  	     unsigned short In,
  	     typename T>
    struct IsTemporary<tfel::math::VectorExpr<tfel::math::tvector<N,T>,
					      tfel::math::TVFTVExpr<N,Mn,In,T> > >
    {
      static const bool cond = false;
    };

  }// end of namespace typetraits

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TVFTV_HXX */

