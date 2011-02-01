/*!
 * \file   ScalarVectorExprWithoutConstIterator.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _MATH_SCALARVECTOREXPRWITHOUTCONSTITERATOR_LIB_
#define _MATH_SCALARVECTOREXPRWITHOUTCONSTITERATOR_LIB_ 1

#include <string>
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class ScalarVectorExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,VectorConcept>::cond));
  
      typedef typename VectorTraits<B>::NumType   NumTypeB;
      typedef typename VectorTraits<B>::IndexType IndexType;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      const typename B::RunTimeProperties RTP;
      
      ScalarVectorExprWithoutConstIterator();

    public:
      
      typedef typename ComputeBinaryResult<A,NumTypeB,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarVectorExprWithoutConstIterator<")+Name<A>::getName()
	  +string(",")+Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
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

      TFEL_MATH_INLINE ScalarVectorExprWithoutConstIterator(const A l, const B& r)
	: a(l), b(r), RTP(r.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE 
      ScalarVectorExprWithoutConstIterator(const ScalarVectorExprWithoutConstIterator& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return Op::apply(a,b(i));
      }

    public:

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                         
      
    };

    template<typename A, typename B, class Op>
    class VectorScalarExprWithoutConstIterator
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename VectorTraits<A>::NumType   NumTypeA;
      typedef typename VectorTraits<A>::IndexType IndexType;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      

      VectorScalarExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,Op>::Handle NumType;
      
      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorScalarExprWithoutConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
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

      TFEL_MATH_INLINE VectorScalarExprWithoutConstIterator(const A& l, const B r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE VectorScalarExprWithoutConstIterator(const VectorScalarExprWithoutConstIterator<A,B,Op>& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return Op::apply(a(i),b);
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
#include"TFEL/Math/Vector/ScalarVectorExprWithoutConstIteratorSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _MATH_SCALARVECTOREXPRWITHOUTCONSTITERATOR_LIB_ */
