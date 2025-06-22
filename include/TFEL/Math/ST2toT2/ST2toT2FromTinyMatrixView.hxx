/*!
 * \file   include/TFEL/Math/ST2toT2/ST2toT2FromTinyMatrixView.hxx
 * \brief  ST2toT2FromTMatrixView
 *
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2FROMTINYMATRIXVIEW_HXX_
#define LIB_TFEL_MATH_ST2TOT2FROMTINYMATRIXVIEW_HXX_

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2Concept.hxx"
#include "TFEL/Math/ST2toT2.hxx"
#include "TFEL/Math/tmatrix.hxx"

// namespace tfel
// {

//   namespace math
//   {

//     /*!
//      * ST2toT2 From Tiny Matrix
//      */
//     template<unsigned short N,
// 	     unsigned short Mn,
// 	     unsigned short Mm,
// 	     unsigned short In,
// 	     unsigned short Im,
// 	     typename T>
//     struct ST2toT2FromTinyMatrixViewExpr
//     {}; // end of struct ST2toT2FromTinyMatrixViewExpr

//     template<unsigned short N,
// 	     unsigned short Mn,
// 	     unsigned short Mm,
// 	     unsigned short In,
// 	     unsigned short Im,
// 	     typename T>
//     struct Expr<ST2toT2<N,T>,
// 			ST2toT2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T> >
// 	: public ST2toT2Concept<Expr<ST2toT2<N,T>,
// 					      ST2toT2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T>
// > >, 	  public ST2toT2_base<Expr<ST2toT2<N,T>,
// 					    ST2toT2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T>
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
//       using ST2toT2_base<Expr>::operator=;

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
//     struct ST2toT2FromTinyMatrixView
//     {
//       typedef
//       Expr<ST2toT2<N,T>,ST2toT2FromTinyMatrixViewExpr<N,Mn,Mm,In,Im,T> >
//       type;
//     }; // end of struct ST2toT2FromTinyMatrixView

//   } // end of namespace math

// } // end of namespace tfel

#endif /* LIB_TFEL_MATH_ST2TOT2FROMTINYMATRIXVIEW_HXX_ */
