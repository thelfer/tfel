/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2/T2toST2FromTinyMatrixView2.hxx
 * \brief  T2toST2FromTMatrixView
 * 
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_MATH_T2TOST2FROMTINYMATRIXVIEW2_HXX_
#define _LIB_TFEL_MATH_T2TOST2FROMTINYMATRIXVIEW2_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"
#include"TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include"TFEL/Math/T2toST2/T2toST2Expr.hxx"
#include"TFEL/Math/t2tost2.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel
{
  
  namespace math
  {

    // /*!
    //  * T2toST2 From Tiny Matrix
    //  */
    // template<unsigned short N,
    // 	     unsigned short Mn,
    // 	     unsigned short Mm,
    // 	     unsigned short In,
    // 	     unsigned short Im,
    // 	     typename T>
    // struct T2toST2FromTinyMatrixView2Expr
    // {}; // end of struct T2toST2FromTinyMatrixView2Expr

    // template<unsigned short N,
    // 	     unsigned short Mn,
    // 	     unsigned short Mm,
    // 	     unsigned short In,
    // 	     unsigned short Im,
    // 	     typename T>
    // struct T2toST2Expr<t2tost2<N,T>,
    // 			T2toST2FromTinyMatrixView2Expr<N,Mn,Mm,In,Im,T> >
    // 	: public T2toST2Concept<T2toST2Expr<t2tost2<N,T>,
    // 					      T2toST2FromTinyMatrixView2Expr<N,Mn,Mm,In,Im,T> > >,
    // 	  public t2tost2_base<T2toST2Expr<t2tost2<N,T>,
    // 					    T2toST2FromTinyMatrixView2Expr<N,Mn,Mm,In,Im,T> > >

    // {

    //   typedef EmptyRunTimeProperties RunTimeProperties;

    //   RunTimeProperties
    //   getRunTimeProperties() const
    //   {
    // 	return RunTimeProperties();
    //   }

    //   T2toST2Expr(tmatrix<Mn,Mm,T>& m_,
    // 		   const unsigned short i_,
    // 		   const unsigned short j_)
    // 	: m(m_),
    // 	  oi(i_),
    // 	  oj(j_)
    //   {} // end of T2toST2Expr

    //   const T&
    //   operator()(const unsigned short i,
    // 		 const unsigned short j) const
    //   {
    // 	assert(i<StensorDimeToSize<N>::value);
    // 	assert(j<StensorDimeToSize<N>::value);
    // 	return this->m(In+this->oi*StensorDimeToSize<N>::value+i,
    // 		       Im+this->oj*StensorDimeToSize<N>::value+j);
    //   } // end of operator() const

    //   T&
    //   operator()(const unsigned short i,
    // 		 const unsigned short j)
    //   {
    // 	assert(i<StensorDimeToSize<N>::value);
    // 	assert(j<StensorDimeToSize<N>::value);
    // 	return this->m(In+this->oi*StensorDimeToSize<N>::value+i,
    // 		       Im+this->oj*StensorDimeToSize<N>::value+j);
    //   } // end of operator()

    //   //! assignement operator
    //   using t2tost2_base<T2toST2Expr>::operator=;

    // protected:

    //   tmatrix<Mn,Mm,T>& m;
    //   unsigned short oi;
    //   unsigned short oj;

    // private:
      
    //   /*!
    //    * Simple checks
    //    */
    //   TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
    //   TFEL_STATIC_ASSERT((In<Mn));
    //   TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mm-Im));
      
    // }; // end of struct T2toST2Expr
    
    // template<unsigned short N,
    // 	     unsigned short Mn,
    // 	     unsigned short Mm,
    // 	     unsigned short In,
    // 	     unsigned short Im,
    // 	     typename T = double>
    // struct T2toST2FromTinyMatrixView2
    // {
    //   typedef T2toST2Expr<t2tost2<N,T>,T2toST2FromTinyMatrixView2Expr<N,Mn,Mm,In,Im,T> > type;
    // }; // end of struct T2toST2FromTinyMatrixView2
    
  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_T2TOST2FROMTINYMATRIXVIEW2_HXX */

