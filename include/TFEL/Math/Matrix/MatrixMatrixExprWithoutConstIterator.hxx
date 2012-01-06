/*!
 * \file   MatrixMatrixExprWithoutConstIterator.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _TFEL_MATH_MATRIX_MATRIX_EXPRWITHOUTCONSTITERATOR_LIB_
#define _TFEL_MATH_MATRIX_MATRIX_EXPRWITHOUTCONSTITERATOR_LIB_ 1

#include <string>
#include<cstddef>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class MatrixMatrixExprWithoutConstIterator
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MatrixConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,Op>::Result Result;
      typedef MatrixTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename MatrixTraits<A>::NumType NumTypeA;
      typedef typename MatrixTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;
      
      MatrixMatrixExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,Op>::Handle NumType;

      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixMatrixExprWithoutConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MatrixMatrixExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),
						   r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE MatrixMatrixExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}
#endif

      TFEL_MATH_INLINE 
      MatrixMatrixExprWithoutConstIterator(const MatrixMatrixExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i,const IndexType j) const 
      {
	return Op::apply(a(i,j),b(i,j));
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
#include"TFEL/Math/Matrix/MatrixMatrixExprWithoutConstIteratorSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _TFEL_MATH_MATRIX_MATRIX_EXPRWITHOUTCONSTITERATOR_LIB_ */
