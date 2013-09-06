/*!
 * \file   StensorFromTinyMatrixRowView2.hxx
 * \brief  
 * Stensor From Tensor Matrix Row View
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_STENSORFROMTINYMATRIXROWVIEW2_HXX_
#define _LIB_TFEL_MATH_STENSORFROMTINYMATRIXROWVIEW2_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Stensor/StensorExpr.hxx"
#include"TFEL/Math/stensor.hxx"
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
    struct StensorFromTinyMatrixRowView2Expr
    {}; // end of struct StensorFromTinyMatrixRowView2Expr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct StensorExpr<stensor<N,T>,StensorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> >
      : public StensorConcept<StensorExpr<stensor<N,T>,StensorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> > >,
	public stensor_base<StensorExpr<stensor<N,T>,StensorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      StensorExpr(tmatrix<Mn,Mm,T>& m_,
		  const unsigned short i_,
		  const unsigned short j_)
	: m(m_),
	  oi(i_),
	  oj(j_)
      {} // end of StensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->m(In+this->oi,Im+(this->oj)*StensorDimeToSize<N>::value+i);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->m(In+this->oi,Im+(this->oj)*StensorDimeToSize<N>::value+i);
      } // end of operator()

      const T&
      operator[](const unsigned short i) const
      {
	return this->m(In+this->oi,Im+(this->oj)*StensorDimeToSize<N>::value+i);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->m(In+this->oi,Im+(this->oj)*StensorDimeToSize<N>::value+i);
      } // end of operator[]

      //! assignement operator
      using stensor_base<StensorExpr>::operator=;

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
      TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mm-Im));

    }; // end of struct StensorExpr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T = double>
    struct StensorFromTinyMatrixRowView2
    {
      typedef StensorExpr<stensor<N,T>,StensorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct StensorFromTinyMatrixRowView2
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_STENSORFROMTINYMATRIXROWVIEW2_HXX */

