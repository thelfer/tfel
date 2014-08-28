/*!
 * \file   include/TFEL/Math/Vector/TVectorFromTinyMatrixRowView2.hxx
 * \brief  
 * Tvector From Tensor Matrix Row View
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_TVECTORFROMTINYMATRIXROWVIEW2_HXX_
#define _LIB_TFEL_MATH_TVECTORFROMTINYMATRIXROWVIEW2_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Tvector/TvectorConcept.hxx"
#include"TFEL/Math/Tvector/TvectorExpr.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel
{
  
  namespace math
  {

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T>
    struct TVectorFromTinyMatrixRowView2Expr
    {}; // end of struct TVectorFromTinyMatrixRowView2Expr

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T>
    struct TvectorExpr<tvector<N,T>,TVectorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> >
      : public TvectorConcept<TvectorExpr<tvector<N,T>,TVectorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> > >,
	public tvector_base<TvectorExpr<tvector<N,T>,TVectorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> >,N,T>
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      TvectorExpr(tmatrix<Mn,Mm,T>& m_,
		  const unsigned short i_,
		  const unsigned short j_)
	: m(m_),
	  oi(i_),
	  oj(j_)
      {} // end of TvectorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->m(In+this->oi,Im+(this->oj)*N+i);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->m(In+this->oi,Im+(this->oj)*N+i);
      } // end of operator()

      const T&
      operator[](const unsigned short i) const
      {
	return this->m(In+this->oi,Im+(this->oj)*N+i);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->m(In+this->oi,Im+(this->oj)*N+i);
      } // end of operator[]

      //! assignement operator
      using tvector_base<TvectorExpr,N,T>::operator=;

    protected:

      tmatrix<Mn,Mm,T>& m;
      const unsigned short oi;
      const unsigned short oj;

    private:

      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      TFEL_STATIC_ASSERT((In<Mn));
      TFEL_STATIC_ASSERT((N<=Mm-Im));

    }; // end of struct TvectorExpr

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T = double>
    struct TVectorFromTinyMatrixRowView2
    {
      typedef TvectorExpr<tvector<N,T>,TVectorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct TVectorFromTinyMatrixRowView2
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TVECTORFROMTINYMATRIXROWVIEW2_HXX */

