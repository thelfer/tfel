/*!
 * \file   MatrixNegExprWithoutConstIterator.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_MATRIX_NEG_EXPRWITHOUTCONSTITERATOR_H_
#define _LIB_TFEL_MATRIX_NEG_EXPRWITHOUTCONSTITERATOR_H_ 

#include"Metaprogramming/StaticAssert.hxx"
#include"Metaprogramming/Implements.hxx"
#include"Metaprogramming/InvalidType.hxx"
#include"TypeTraits/IsTemporary.hxx"
#include"Math/Matrix/MatrixConcept.hxx"

namespace tfel{

  namespace math{

    template<typename A>
    class MatrixNegExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const typename A::RunTimeProperties RTP;

      struct invalid_argument;

      typedef typename MatrixTraits<A>::NumType   NumTypeA;
      typedef typename MatrixTraits<A>::IndexType IndexType;

    public:

      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixNegExprWithoutConstIterator<")+Name<A>::getName()+string(">");
      }

    protected:

      typedef typename A::RunTimeProperties RunTimeProperties;
      typedef A first_arg;
      typedef invalid_argument second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE MatrixNegExprWithoutConstIterator(const A& l)
	: a(l), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE const NumType
      operator()(const IndexType i, const IndexType j) const 
      {
	return -a(i,j);
      }

    public:

      TFEL_MATH_INLINE const RunTimeProperties getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATRIX_NEG_EXPRWITHOUTCONSTITERATOR_H */

