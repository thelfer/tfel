/*!
 * \file   include/TFEL/Math/Tensor/TensorFromTinyVectorView.hxx
 * \brief  TensorFromTVectorView
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

#ifndef _LIB_TFEL_MATH_TENSORFROMTINYVECTORVIEW_HXX_
#define _LIB_TFEL_MATH_TENSORFROMTINYVECTORVIEW_HXX_ 

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

    /*!
     *  Tensor From Tiny Vector
     * \param N  : space dimension
     * \param Mn : underlying tiny vector size
     * \param In : starting index in the underlying tiny vector
     * \param T  : value type
     */
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T>
    struct TensorFromTinyVectorViewExpr
    {}; // end of struct TensorFromTinyMatrixColumnViewExpr

    /*!
     *  Tensor From Tiny Vector expression
     * \param N  : space dimension
     * \param Mn : underlying tiny vector size
     * \param In : starting index in the underlying tiny vector
     * \param T  : value type
     */
    template<unsigned short N, unsigned short Mn,
	     unsigned short In,typename T>
    struct TensorExpr<tensor<N,T>,TensorFromTinyVectorViewExpr<N,Mn,In,T> >
      : public TensorConcept<TensorExpr<tensor<N,T>,TensorFromTinyVectorViewExpr<N,Mn,In,T> > >,
	public tensor_base<TensorExpr<tensor<N,T>,TensorFromTinyVectorViewExpr<N,Mn,In,T> > >
    {
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename tensor<N,T>::value_type      value_type;      
      typedef typename tensor<N,T>::pointer	   pointer;	    
      typedef typename tensor<N,T>::const_pointer   const_pointer; 
      typedef typename tensor<N,T>::reference	   reference;	    
      typedef typename tensor<N,T>::const_reference const_reference;
      typedef typename tensor<N,T>::size_type 	   size_type;	    
      typedef typename tensor<N,T>::difference_type difference_type;

      typedef tvector<Mn,T>           first_arg;
      typedef tfel::meta::InvalidType second_arg;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      TensorExpr(tvector<Mn,T>& v_)
	: v(v_)
      {} // end of TensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->v(In+i);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->v(In+i);
      } // end of operator()

      const T&
      operator[](const unsigned short i) const
      {
	return this->v(In+i);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->v(In+i);
      } // end of operator[]

      //! using tensor_base::operator=
      using tensor_base<TensorExpr>::operator=;

    protected:

      tvector<Mn,T>& v;

    private:

      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      TFEL_STATIC_ASSERT((In<Mn));
      TFEL_STATIC_ASSERT((TensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct TensorExpr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short In,
	     typename T = double>
    struct TensorFromTinyVectorView
    {
      typedef TensorExpr<tensor<N,T>,TensorFromTinyVectorViewExpr<N,Mn,In,T> > type;
    }; // end of struct TensorFromTinyVectorView
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TENSORFROMTINYVECTORVIEW_HXX */

