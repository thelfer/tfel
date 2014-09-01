/*!
 * \file   include/TFEL/Math/Tensor/TensorFromTinyMatrixRowView2.hxx
 * \brief  
 * Tensor From Tensor Matrix Row View
 * \author Helfer Thomas
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_TENSORFROMTINYMATRIXROWVIEW2_HXX_
#define _LIB_TFEL_MATH_TENSORFROMTINYMATRIXROWVIEW2_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/Tensor/TensorExpr.hxx"
#include"TFEL/Math/tensor.hxx"
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
    struct TensorFromTinyMatrixRowView2Expr
    {}; // end of struct TensorFromTinyMatrixRowView2Expr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct TensorExpr<tensor<N,T>,TensorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> >
      : public TensorConcept<TensorExpr<tensor<N,T>,TensorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> > >,
	public tensor_base<TensorExpr<tensor<N,T>,TensorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      TensorExpr(tmatrix<Mn,Mm,T>& m_,
		  const unsigned short i_,
		  const unsigned short j_)
	: m(m_),
	  oi(i_),
	  oj(j_)
      {} // end of TensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->m(In+this->oi,Im+(this->oj)*TensorDimeToSize<N>::value+i);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->m(In+this->oi,Im+(this->oj)*TensorDimeToSize<N>::value+i);
      } // end of operator()

      const T&
      operator[](const unsigned short i) const
      {
	return this->m(In+this->oi,Im+(this->oj)*TensorDimeToSize<N>::value+i);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->m(In+this->oi,Im+(this->oj)*TensorDimeToSize<N>::value+i);
      } // end of operator[]

      //! assignement operator
      using tensor_base<TensorExpr>::operator=;

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
      TFEL_STATIC_ASSERT((TensorDimeToSize<N>::value<=Mm-Im));

    }; // end of struct TensorExpr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T = double>
    struct TensorFromTinyMatrixRowView2
    {
      typedef TensorExpr<tensor<N,T>,TensorFromTinyMatrixRowView2Expr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct TensorFromTinyMatrixRowView2
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TENSORFROMTINYMATRIXROWVIEW2_HXX */

