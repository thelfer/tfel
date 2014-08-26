/*!
 * \file   T2toT2FromTinyMatrixView.hxx
 * \brief  T2toT2FromTMatrixView
 * 
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_MATH_T2TOT2FROMTINYMATRIXVIEW_HXX_
#define _LIB_TFEL_MATH_T2TOT2FROMTINYMATRIXVIEW_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Matrix/MatrixUtilities.hxx"
#include"TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include"TFEL/Math/T2toT2/T2toT2Expr.hxx"
#include"TFEL/Math/t2tot2.hxx"
#include"TFEL/Math/tmatrix.hxx"

// namespace tfel
// {
  
//   namespace math
//   {

//     /*!
//      * T2toT2 From Tiny Matrix
//      */
//     template<unsigned short N,
// 	     unsigned short Mn,
// 	     unsigned short Mm,
// 	     unsigned short In,
// 	     unsigned short Im,
// 	     typename T>
//     struct T2toT2FromTinyMatrixViewExpr
//     {}; // end of struct T2toT2FromTinyMatrixViewExpr

//     template<unsigned short N,
// 	     unsigned short Mn,
// 	     unsigned short Mm,
// 	     unsigned short In,
// 	     unsigned short Im,
// 	     typename T>
//     struct T2toT2Expr<t2tot2<N,T>,
// 			T2toT2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T> >
// 	: public T2toT2Concept<T2toT2Expr<t2tot2<N,T>,
// 					      T2toT2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T> > >,
// 	  public t2tot2_base<T2toT2Expr<t2tot2<N,T>,
// 					    T2toT2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T> > >
//     {

//       typedef EmptyRunTimeProperties RunTimeProperties;

//       RunTimeProperties
//       getRunTimeProperties() const
//       {
// 	return RunTimeProperties();
//       }

//       T2toT2Expr(tmatrix<Mn,Mm,T>& m_)
// 	: m(m_)
//       {} // end of T2toT2Expr

//       const T&
//       operator()(const unsigned short i,
// 		 const unsigned short j) const
//       {
// 	assert(i<StensorDimeToSize<N>::value);
// 	assert(j<StensorDimeToSize<N>::value);
// 	return this->m(In+i,Im+j);
//       } // end of operator() const

//       T&
//       operator()(const unsigned short i,
// 		 const unsigned short j)
//       {
// 	assert(i<StensorDimeToSize<N>::value);
// 	assert(j<StensorDimeToSize<N>::value);
// 	return this->m(In+i,Im+j);
//       } // end of operator()

//       //! assignement operator
//       using t2tot2_base<T2toT2Expr>::operator=;
      
//     protected:

//       tmatrix<Mn,Mm,T>& m;
      
//     private:
      
//       /*!
//        * Simple checks
//        */
//       TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
//       TFEL_STATIC_ASSERT((In<Mn));
//       TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mm-Im));
      
//     }; // end of struct T2toT2Expr
    
    
//     template<unsigned short N,
// 	     unsigned short Mn,
// 	     unsigned short Mm,
// 	     unsigned short In,
// 	     unsigned short Im,
// 	     typename T = double>
//     struct T2toT2FromTinyMatrixView
//     {
//       typedef T2toT2Expr<t2tot2<N,T>,T2toT2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T> > type;
//     }; // end of struct T2toT2FromTinyMatrixView
    
//   } // end of namespace math
  
// } // end of namespace tfel

#endif /* _LIB_TFEL_MATH_T2TOT2FROMTINYMATRIXVIEW_HXX */

