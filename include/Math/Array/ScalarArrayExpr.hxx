/*!
 * \file   ScalarArrayExpr.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 May 2006  
 */

#ifndef _MATH_SCALAR_ARRAY_EXPR_LIB_
#define _MATH_SCALAR_ARRAY_EXPR_LIB_ 1

#include <string>

#include "Config/TFELConfig.hxx"

#include "Utilities/Name.hxx"

#include "Metaprogramming/StaticAssert.hxx"
#include "Metaprogramming/Implements.hxx"

#include "TypeTraits/IsScalar.hxx"
#include "TypeTraits/IsTemporary.hxx"

#include "Math/General/ResultType.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/General/ScalarObjectRandomAccessConstIterator.hxx"
#include "Math/Array/ArrayConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B, class Op>
    class ScalarArrayExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ArrayConcept>::cond));
      
      typedef typename ArrayTraits<B>::NumType   NumTypeB;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename B::RunTimeProperties RTP;
      
    public:

      typedef typename ComputeBinaryResult<A,NumTypeB,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ScalarArrayExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
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
      typedef unsigned int   size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ScalarArrayExpr(const A& l, const B& r)
	: a(l), b(r), RTP(r.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE ScalarArrayExpr(const ScalarArrayExpr<A,B,Op>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return Op::apply(a,b(i));
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,const unsigned int j) const 
      {
	return Op::apply(a,b(i,j));
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,const unsigned int j,const unsigned int k) const 
      {
	return Op::apply(a,b(i,j,k));
      }

    public:

      typedef ScalarObjectRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a,b.begin());
      }

      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a,b.end());
      }

      TFEL_MATH_INLINE const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      TFEL_MATH_INLINE const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };

    template<typename A, typename B, class Op>
    class ArrayScalarExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename ArrayTraits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;

    public:


      typedef typename ComputeBinaryResult<NumTypeA,B,Op>::Handle NumType;

      static const std::string getName(void){
	using namespace tfel::utilities;
	using namespace std;
	return string("ArrayScalarExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<Op>::getName()+string(">");
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
      typedef unsigned int   size_type;	    						
      typedef ptrdiff_t      difference_type;
      
      TFEL_MATH_INLINE ArrayScalarExpr(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE ArrayScalarExpr(const ArrayScalarExpr& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned int i) const 
      {
	return Op::apply(a(i),b);
      }

      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned int i,const unsigned int j) const 
      {
	return Op::apply(a(i,j),b);
      }

      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned int i,const unsigned int j,const unsigned int k) const 
      {
	return Op::apply(a(i,j,k),b);
      }

    public:

      typedef ObjectScalarRandomAccessConstIterator<A,B,Op> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b);
      }

      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b);
      }

      TFEL_MATH_INLINE const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      TFEL_MATH_INLINE const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };
        
  } // end of namespace math

} // end of namespace tfel

#ifndef NO_EXPRESSION_TEMPLATE_SPECIALISATION 
#include"Math/Array/ScalarArrayExprSpecialisation.ixx"
#endif  /* NO_EXPRESSION_TEMPLATE_SPECIALISATION */

#endif /* _MATH_SCALAR_ARRAY_EXPR_LIB_ */
