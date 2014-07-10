/*!
 * \file   StensorFromTinyMatrixColumnView.hxx
 * \brief  
 * Stensor From Matric Column View
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW_HXX_
#define _LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW_HXX_ 

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

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T>
    struct StensorFromTinyMatrixColumnViewExpr
    {}; // end of struct StensorFromTinyMatrixColumnViewExpr

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T>
    struct StensorExpr<stensor<N,T>,StensorFromTinyMatrixColumnViewExpr<N,Mn,Mm,In,Im,T> >
      : public StensorConcept<StensorExpr<stensor<N,T>,StensorFromTinyMatrixColumnViewExpr<N,Mn,Mm,In,Im,T> > >,
	public stensor_base<StensorExpr<stensor<N,T>,StensorFromTinyMatrixColumnViewExpr<N,Mn,Mm,In,Im,T> > >
    {

      //! a simple alias
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
	return this->m(In+i,Im);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->m(In+i,Im);
      } // end of operator()

      const T&
      operator[](const unsigned short i) const
      {
	return this->m(In+i,Im);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->m(In+i,Im);
      } // end of operator[]

      //! assignement operator
      using stensor_base<StensorExpr>::operator =;

    protected:

      //! the underlying matrix
      tmatrix<Mn,Mm,T>& m;

    private:

      //! a simple check
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      //! a simple check
      TFEL_STATIC_ASSERT((Im<Mm));
      //! a simple check
      TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct StensorExpr

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T = double>
    struct StensorFromTinyMatrixColumnView
    {
      typedef StensorExpr<stensor<N,T>,StensorFromTinyMatrixColumnViewExpr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct StensorFromTinyMatrixColumnView
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW_HXX */

