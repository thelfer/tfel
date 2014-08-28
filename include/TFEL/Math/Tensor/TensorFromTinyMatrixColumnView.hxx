/*!
 * \file   include/TFEL/Math/Tensor/TensorFromTinyMatrixColumnView.hxx
 * \brief  
 * Tensor From Matric Column View
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_TENSORFROMTINYMATRIXCOLUMNVIEW_HXX_
#define _LIB_TFEL_MATH_TENSORFROMTINYMATRIXCOLUMNVIEW_HXX_ 

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

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T>
    struct TensorFromTinyMatrixColumnViewExpr
    {}; // end of struct TensorFromTinyMatrixColumnViewExpr

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T>
    struct TensorExpr<tensor<N,T>,TensorFromTinyMatrixColumnViewExpr<N,Mn,Mm,In,Im,T> >
      : public TensorConcept<TensorExpr<tensor<N,T>,TensorFromTinyMatrixColumnViewExpr<N,Mn,Mm,In,Im,T> > >,
	public tensor_base<TensorExpr<tensor<N,T>,TensorFromTinyMatrixColumnViewExpr<N,Mn,Mm,In,Im,T> > >
    {

      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      TensorExpr(tmatrix<Mn,Mm,T>& m_)
	: m(m_)
      {} // end of TensorExpr

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
      using tensor_base<TensorExpr>::operator =;

    protected:

      //! the underlying matrix
      tmatrix<Mn,Mm,T>& m;

    private:

      //! a simple check
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      //! a simple check
      TFEL_STATIC_ASSERT((Im<Mm));
      //! a simple check
      TFEL_STATIC_ASSERT((TensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct TensorExpr

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T = double>
    struct TensorFromTinyMatrixColumnView
    {
      typedef TensorExpr<tensor<N,T>,TensorFromTinyMatrixColumnViewExpr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct TensorFromTinyMatrixColumnView
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TENSORFROMTINYMATRIXCOLUMNVIEW_HXX */

