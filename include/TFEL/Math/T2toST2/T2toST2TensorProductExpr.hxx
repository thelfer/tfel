/*! 
 * \file  T2toST2TensorProductExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 juin 2014
 */

#ifndef _LIB_TFEL_MATH_T2TOST2TENSORPRODUCTEXPR_H_
#define _LIB_TFEL_MATH_T2TOST2TENSORPRODUCTEXPR_H_ 

#include"TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/Stensor/StensorExpr.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel
{

  namespace math
  {

    //! Empty structure used for partial specialisation of the
    //! StensorExpr class
    struct T2toST2TensorProductExpr
    {}; // end of struct T2toST2TensorProductExpr

    /*!
     * Partial specialisation
     */
    template<typename StensorResultType>
    struct StensorExpr<StensorResultType,T2toST2TensorProductExpr>
      : public T2toST2Concept<StensorExpr<StensorResultType,T2toST2TensorProductExpr> >,
	public fsarray<StensorDimeToSize<StensorTraits<StensorResultType>::dime>::value,
		       typename StensorTraits<StensorResultType>::NumType>
    {
      //! a simple alias
      typedef typename StensorTraits<StensorResultType>::NumType value_type;
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template<typename T2toST2Type,
	       typename TensorType>
      StensorExpr(const T2toST2Type& a,
		  const TensorType&  b)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime==TensorTraits <TensorType>::dime));
	const value_type zero = value_type(0);
	for(unsigned short i=0;i!=StensorDimeToSize<StensorTraits<StensorResultType>::dime>::value;++i){
	  this->v[i] = zero;
	  for(unsigned short j=0;j!=TensorDimeToSize<StensorTraits<StensorResultType>::dime>::value;++j){
	    this->v[i] += a(i,j)*b[j];
	  }
	}
      } // end of StensorExpr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type&
      operator()(const unsigned short i) const
      {
	return this->v[i];
      } // end of operator()
    }; // end of struct StensorExpr<StensorResultType,T2T2toST2TensorProductExp>

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_T2TOST2TENSORPRODUCTEXPR_H */

