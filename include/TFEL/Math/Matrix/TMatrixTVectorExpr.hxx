/*!
 * \file   include/TFEL/Math/Matrix/TMatrixTVectorExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 avr 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX_
#define _LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX_ 

#include<cstddef>
#include<iterator>
#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/FSAlgorithm/inner_product.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel{

  namespace math {
    
    template<unsigned short N,unsigned short M,
	     typename A, typename B>
    struct TMatrixTVectorExpr
      : public ExprBase
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RunTimeProperties();
      }
      /*!
       * a pseudo iterator allowing to iterate over values in the same
       * row
       */
      struct RowConstIterator
      {	
	using MType   = typename std::decay<A>::type;
	using NumType = typename MatrixTraits<MType>::NumType;
	//! default constructor
	TFEL_MATH_INLINE RowConstIterator(const MType& m_,
					  const unsigned short i_)
	  : m(m_),i(i_),j(0)
	{}
	//! go to the next column
	TFEL_MATH_INLINE RowConstIterator& operator++ ()
	{
	  ++j;
	  return *this;
	} // end of operator++
	//! \return the current matrix value
	TFEL_MATH_INLINE NumType
	operator *(void) const
	{
	  return m(i,j);
	}
      private:
	const MType& m;         //! reference to the underlying matrix
	const unsigned short i; //! row index
	unsigned short j;       //! current column 
      }; // end of struc RowConstIterator
      //! a pseudo iterator for the vector. This iterator works even if
      // VType does not provide an iterator
      struct VectorConstIterator
      {	
	using VType   = typename std::decay<B>::type;
	using NumType = typename VectorTraits<VType>::NumType;
	TFEL_MATH_INLINE VectorConstIterator(const VType& v_)
	  : v(v_),i(0)
	{}
	TFEL_MATH_INLINE VectorConstIterator& operator++ ()
	{
	  ++i;
	  return *this;
	} // end of operator++
	TFEL_MATH_INLINE const NumType &
	operator *(void) const
	{
	  return v(i);
	}
      private:
	const VType& v;
	unsigned short i;
      }; // end of struc VectorConstIterator

    protected:

      
      typedef typename ComputeBinaryResult<A,B,OpMult>::Result Result;
      using NumType   = typename VectorTraits<Result>::NumType;
      using IndexType = typename VectorTraits<Result>::IndexType;

      typedef A first_arg;
      typedef B second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TMatrixTVectorExpr() = delete;

      TFEL_MATH_INLINE TMatrixTVectorExpr(A l,B r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	using namespace tfel::fsalgo;
	return inner_product<M>::template exe<NumType>(RowConstIterator(a,i),
						       VectorConstIterator(b));
      }

      ArgumentStorage<A> a;
      ArgumentStorage<B> b;
      TFEL_STATIC_ASSERT((tfel::meta::Implements<typename std::decay<A>::type,MatrixConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<typename std::decay<B>::type,VectorConcept>::cond));
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX */

