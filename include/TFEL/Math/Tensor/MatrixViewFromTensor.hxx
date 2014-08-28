/*! 
 * \file  include/TFEL/Math/Tensor/MatrixViewFromTensor.hxx
 * \brief The MatrixViewFromTensor describes how a tensor can be interpreted as a
 * tiny matrix
 * \author Helfer Thomas
 * \brief  03 déc. 2013
 */

#ifndef _LIB_TFEL_MATH_MATRIXVIEWFROMTENSOR_H_
#define _LIB_TFEL_MATH_MATRIXVIEWFROMTENSOR_H_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/Matrix/MatrixExpr.hxx"
#include"TFEL/Math/Forward/tmatrix.hxx"
#include"TFEL/Math/Forward/tensor.hxx"

namespace tfel
{
  
  namespace math
  {

    namespace internals
    {
      template<unsigned short N>
      struct TensorConceptMatrixAccessOperator;
    }

    template<typename T>
     class TensorConcept;

    template<typename T>
    struct TensorTraits;

    /*!
     * Matrix view from tensor
     */
    template<typename TensorType>
    struct MatrixViewFromTensorExpr
    {}; // end of struct MatrixViewFromTensorExpr

    template<typename TensorType>
    struct MatrixExpr<tmatrix<3u,3u,typename TensorTraits<TensorType>::NumType>,
		      MatrixViewFromTensorExpr<TensorType> >
      : public MatrixConcept<MatrixExpr<tmatrix<3u,3u,
						typename TensorTraits<TensorType>::NumType>,
					MatrixViewFromTensorExpr<TensorType> > >
    {
      
      typedef typename TensorTraits<TensorType>::NumType NumType;
      typedef unsigned short IndexType;
      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      MatrixExpr(const TensorType& t_)
	: t(t_)
      {} // end of MatrixExpr

      const NumType
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	using tfel::math::internals::TensorConceptMatrixAccessOperator;
	return TensorConceptMatrixAccessOperator<TensorTraits<TensorType>::dime>::exe(this->t,i,j);
      } // end of operator() const

    protected:

      //! says if the underlying object is a temporary
      static const bool IsTemporary = tfel::typetraits::IsTemporary<TensorType>::cond;
      //! The tensor object
      typename tfel::meta::IF<IsTemporary,
			      const TensorType,
			      const TensorType&>::type t;

    private:

      /*!
       * a simple check
       */
      TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));

    }; // end of struct MatrixExpr
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_MATRIXVIEWFROMTENSOR_H */

