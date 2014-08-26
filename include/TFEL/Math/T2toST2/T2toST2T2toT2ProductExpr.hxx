/*! 
 * \file  T2toST2T2toT2ProductExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 juin 2014
 */

#ifndef _LIB_TFEL_MATH_T2TOST2T2TOT2PRODUCTEXPR_H_
#define _LIB_TFEL_MATH_T2TOST2T2TOT2PRODUCTEXPR_H_ 

#include"TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include"TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include"TFEL/Math/t2tost2.hxx"

namespace tfel
{

  namespace math
  {

    //! Empty structure used for partial specialisation of the
    //! T2toST2Expr class
    struct T2toST2T2toT2ProductExpr
    {}; // end of struct T2toST2T2toT2ProductExpr

    /*!
     * Partial specialisation
     */
    template<typename T2toST2ResultType>
    struct T2toST2Expr<T2toST2ResultType,T2toST2T2toT2ProductExpr>
      : public T2toST2Concept<T2toST2Expr<T2toST2ResultType,T2toST2T2toT2ProductExpr> >,
	public t2tost2_base<T2toST2Expr<T2toST2ResultType,T2toST2T2toT2ProductExpr> >,
	public fsarray<StensorDimeToSize<T2toST2Traits<T2toST2ResultType>::dime>::value*
		       TensorDimeToSize <T2toST2Traits<T2toST2ResultType>::dime>::value,
		       typename T2toST2Traits<T2toST2ResultType>::NumType>
    {
      //! a simple alias
      typedef typename T2toST2Traits<T2toST2ResultType>::NumType value_type;
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template<typename T2toST2Type,
	       typename T2toT2Type>
      T2toST2Expr(const T2toST2Type& a,
		  const T2toT2Type&  b)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toST2Traits<T2toST2Type>::dime==T2toT2Traits <T2toT2Type>::dime));
	const value_type zero = value_type(0);
	for(unsigned short i=0;i!=StensorDimeToSize<T2toST2Traits<T2toST2ResultType>::dime>::value;++i){
	  for(unsigned short j=0;j!=TensorDimeToSize<T2toST2Traits<T2toST2ResultType>::dime>::value;++j){
	    const unsigned short offset = i*T2toST2Traits<T2toST2ResultType>::dime+j;
	    this->v[offset] = zero;
	    for(unsigned short k=0;k!=TensorDimeToSize<T2toST2Traits<T2toST2ResultType>::dime>::value;++k){
	      this->v[offset] += a(i,k)*b(k,j);
	    }
	  }
	}
      } // end of T2toST2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*T2toST2Traits<T2toST2ResultType>::dime+j];
      } // end of operator()
    }; // end of struct T2toST2Expr<T2toST2ResultType,T2T2toST2T2toT2ProductExp>

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_T2TOST2T2TOT2PRODUCTEXPR_H */

