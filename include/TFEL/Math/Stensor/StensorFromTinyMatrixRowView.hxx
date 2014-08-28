/*!
 * \file   include/TFEL/Math/Stensor/StensorFromTinyMatrixRowView.hxx
 * \brief  
 * Stensor From Tensor Matrix Row View
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_STENSORFROMTINYMATRIXROWVIEW_HXX_
#define _LIB_TFEL_MATH_STENSORFROMTINYMATRIXROWVIEW_HXX_ 

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
    struct StensorFromTinyMatrixRowViewExpr
    {}; // end of struct StensorFromTinyMatrixRowViewExpr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct StensorExpr<stensor<N,T>,StensorFromTinyMatrixRowViewExpr<N,Mn,Mm,In,Im,T> >
      : public StensorConcept<StensorExpr<stensor<N,T>,StensorFromTinyMatrixRowViewExpr<N,Mn,Mm,In,Im,T> > >,
	public stensor_base<StensorExpr<stensor<N,T>,StensorFromTinyMatrixRowViewExpr<N,Mn,Mm,In,Im,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      StensorExpr(tmatrix<Mn,Mm,T>& m_)
	: m(m_)
      {} // end of StensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->m(In,Im+i);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->m(In,Im+i);
      } // end of operator()

      const T&
      operator[](const unsigned short i) const
      {
	return this->m(In,Im+i);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->m(In,Im+i);
      } // end of operator[]

      //! assignement operator
      using stensor_base<StensorExpr>::operator=;

    protected:

      tmatrix<Mn,Mm,T>& m;

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
    struct StensorFromTinyMatrixRowView
    {
      typedef StensorExpr<stensor<N,T>,StensorFromTinyMatrixRowViewExpr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct StensorFromTinyMatrixRowView
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_STENSORFROMTINYMATRIXROWVIEW_HXX */

