/*!
 * \file   ScalarST2toST2ExprWithoutConstIteratorSpecialisation.ixx
 * \brief  This file partially specialises the ST2toST2ST2toST2ExprWithoutConstIterator classe for usual operations.
 * \see    NO_EXPRWITHOUTCONSTITERATORESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_SCALARST2TOST2EXPRWITHOUTCONSTITERATORSPECIALISATION_I_
#define _LIB_TFEL_SCALARST2TOST2EXPRWITHOUTCONSTITERATORSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class ScalarST2toST2ExprWithoutConstIterator<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ST2toST2Concept>::cond));
      
      typedef typename ST2toST2Traits<B>::NumType   NumTypeB;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
    public:
      
      typedef typename ComputeBinaryResult<A,NumTypeB,OpMult>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ScalarST2toST2ExprWithoutConstIterator<")+Name<A>::getName()
	  +string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ScalarST2toST2ExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      ScalarST2toST2ExprWithoutConstIterator(const ScalarST2toST2ExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return a*b(i,j);
      }

    };

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class ST2toST2ScalarExprWithoutConstIterator<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ST2toST2Concept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename ST2toST2Traits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,OpMult>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2ScalarExprWithoutConstIterator<")+Name<A>::getName()
	  +string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;
      
      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ST2toST2ScalarExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      ST2toST2ScalarExprWithoutConstIterator(const ST2toST2ScalarExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return a(i,j)*b;
      }

    };

    /*
     * \brief Partial Specialisation for OpDiv.
     */
    template<typename A, typename B>
    class ST2toST2ScalarExprWithoutConstIterator<A,B,OpDiv>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ST2toST2Concept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename ST2toST2Traits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,OpDiv>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2ScalarExprWithoutConstIterator<")+Name<A>::getName()+string(",")
	  +Name<B>::getName()+string(",")+Name<OpDiv>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;
      
      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ST2toST2ScalarExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      ST2toST2ScalarExprWithoutConstIterator(const ST2toST2ScalarExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return a(i,j)/b;
      }

    };

    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_SCALARST2TOST2EXPRWITHOUTCONSTITERATORSPECIALISATION_I_ */

