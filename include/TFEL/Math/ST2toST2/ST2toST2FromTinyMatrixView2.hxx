/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2FromTinyMatrixView2.hxx
 * \brief  ST2toST2FromTMatrixView
 * 
 * \author Helfer Thomas
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_ST2toST2FromTinyMatrixView2_HXX_
#define _LIB_TFEL_MATH_ST2toST2FromTinyMatrixView2_HXX_ 

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
    struct ST2toST2FromTinyMatrixView2Expr
    {}; // end of struct ST2toST2FromTinyMatrixView2Expr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct ST2toST2Expr<st2tost2<N,T>,
			ST2toST2FromTinyMatrixView2Expr<N,Mn,Mm,In,Im,T> >
	: public ST2toST2Concept<ST2toST2Expr<st2tost2<N,T>,
					      ST2toST2FromTinyMatrixView2Expr<N,Mn,Mm,In,Im,T> > >,
	  public st2tost2_base<ST2toST2Expr<st2tost2<N,T>,
					    ST2toST2FromTinyMatrixView2Expr<N,Mn,Mm,In,Im,T> > >

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

      //! assignement operator
      using st2tost2_base<ST2toST2Expr>::operator=;

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
    struct ST2toST2FromTinyMatrixView2
    {
      typedef ST2toST2Expr<st2tost2<N,T>,ST2toST2FromTinyMatrixView2Expr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct ST2toST2FromTinyMatrixView2
    
  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_ST2toST2FromTinyMatrixView2_HXX */

