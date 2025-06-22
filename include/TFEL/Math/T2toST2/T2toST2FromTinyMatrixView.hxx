/*!
 * \file   include/TFEL/Math/T2toST2/T2toST2FromTinyMatrixView.hxx
 * \brief  T2toST2FromTMatrixView
 *
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2FROMTINYMATRIXVIEW_HXX
#define LIB_TFEL_MATH_T2TOST2FROMTINYMATRIXVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/tmatrix.hxx"

// namespace tfel
// {

//   namespace math
//   {

//     /*!
//      * T2toST2 From Tiny Matrix
//      */
//     template<unsigned short N,
// 	     unsigned short Mn,
// 	     unsigned short Mm,
// 	     unsigned short In,
// 	     unsigned short Im,
// 	     typename T>
//     struct T2toST2FromTinyMatrixViewExpr
//     {}; // end of struct T2toST2FromTinyMatrixViewExpr

//     template<unsigned short N,
// 	     unsigned short Mn,
// 	     unsigned short Mm,
// 	     unsigned short In,
// 	     unsigned short Im,
// 	     typename T>
//     struct Expr<t2tost2<N,T>,
// 			T2toST2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T> >
// 	: public T2toST2Concept<Expr<t2tost2<N,T>,
// 					      T2toST2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T>
// > >, 	  public t2tost2_base<Expr<t2tost2<N,T>,
// 					    T2toST2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T>
// >
// >
//     {

//       typedef EmptyRunTimeProperties RunTimeProperties;

//       RunTimeProperties
//       getRunTimeProperties() const
//       {
// 	return RunTimeProperties();
//       }

//       Expr(tmatrix<Mn,Mm,T>& m_)
// 	: m(m_)
//       {} // end of Expr

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
//       using t2tost2_base<Expr>::operator=;

//     protected:

//       tmatrix<Mn,Mm,T>& m;

//     private:

//       /*!
//        * Simple checks
//        */
//       TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
//       TFEL_STATIC_ASSERT((In<Mn));
//       TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mm-Im));

//     }; // end of struct Expr

//     template<unsigned short N,
// 	     unsigned short Mn,
// 	     unsigned short Mm,
// 	     unsigned short In,
// 	     unsigned short Im,
// 	     typename T = double>
//     struct T2toST2FromTinyMatrixView
//     {
//       typedef
//       Expr<t2tost2<N,T>,T2toST2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T> >
//       type;
//     }; // end of struct T2toST2FromTinyMatrixView

//   } // end of namespace math

// } // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOST2FROMTINYMATRIXVIEW_HXX */
