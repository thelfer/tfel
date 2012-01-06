/*!
 * \file   ScalarMatrixExprWithoutConstIterator.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _TFEL_MATH_SCALAR_MATRIX_EXPRWITHOUTCONSTITERATOR_LIB_
#define _TFEL_MATH_SCALAR_MATRIX_EXPRWITHOUTCONSTITERATOR_LIB_ 1

#include <string>
#include<cstddef>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/Name.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class ScalarMatrixExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MatrixConcept>::cond));
      
      typedef typename MatrixTraits<B>::NumType   NumTypeB;
      typedef typename MatrixTraits<B>::IndexType IndexType;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      const typename B::RunTimeProperties RTP;
      
      ScalarMatrixExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<A,NumTypeB,Op>::Handle NumType;
      
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ScalarMatrixExprWithoutConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;
      typedef typename B::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ScalarMatrixExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), RTP(r.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE ScalarMatrixExprWithoutConstIterator(const ScalarMatrixExprWithoutConstIterator<A,B,Op>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i,const IndexType j) const 
      {
	return Op::apply(a,b(i,j));
      }      
           
    public:

      TFEL_MATH_INLINE const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

    template<typename A, typename B, class Op>
    class MatrixScalarExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename MatrixTraits<A>::NumType   NumTypeA;
      typedef typename MatrixTraits<A>::IndexType IndexType;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      
      
      MatrixScalarExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,Op>::Handle NumType;
      
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixScalarExprWithoutConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
      }
      
    protected:

      typedef A first_arg;
      typedef B second_arg;
      typedef typename A::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE MatrixScalarExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE MatrixScalarExprWithoutConstIterator(const MatrixScalarExprWithoutConstIterator<A,B,Op>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i,const IndexType j) const 
      {
	return Op::apply(a(i,j),b);
      }
      
    public:

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };
        
  } // end of namespace math

} // end of namespace tfel

#ifndef NO_EXPRESSION_TEMPLATE_SPECIALISATION 
#include"TFEL/Math/Matrix/ScalarMatrixExprWithoutConstIteratorSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _TFEL_MATH_SCALAR_MATRIX_EXPRWITHOUTCONSTITERATOR_LIB_ */
