/*!
 * \file   ST2toST2ST2toST2ExprWithoutConstIteratorSpecialisation.ixx
 * \brief  This file partially specialises the ST2toST2ST2toST2ExprWithoutConstIterator classe for usual operations.
 * \see    NO_EXPRWITHOUTCONSTITERATORESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_ST2TOST2ST2TOST2EXPRWITHOUTCONSTITERATORSPECIALISATION_I_
#define _LIB_TFEL_ST2TOST2ST2TOST2EXPRWITHOUTCONSTITERATORSPECIALISATION_I_ 

#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpPlus.
     */
    template<typename A, typename B>
    class ST2toST2ST2toST2ExprWithoutConstIterator<A,B,OpPlus>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ST2toST2Concept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ST2toST2Concept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpPlus>::Result Result;
      typedef ST2toST2Traits<Result> traits;

      typedef typename ST2toST2Traits<A>::NumType NumTypeA;
      typedef typename ST2toST2Traits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpPlus>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2ST2toST2ExprWithoutConstIterator<")+Name<A>::getName()
	  +string(",")+Name<B>::getName()+string(",")+Name<OpPlus>::getName()+string(">");
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

      TFEL_MATH_INLINE ST2toST2ST2toST2ExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      ST2toST2ST2toST2ExprWithoutConstIterator(const ST2toST2ST2toST2ExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return a(i,j)+b(i,j);
      }
      
    };

    /*
     * \brief Partial Specialisation for OpMinus.
     */
    template<typename A, typename B>
    class ST2toST2ST2toST2ExprWithoutConstIterator<A,B,OpMinus>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ST2toST2Concept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ST2toST2Concept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMinus>::Result Result;
      typedef ST2toST2Traits<Result> traits;

      typedef typename ST2toST2Traits<A>::NumType NumTypeA;
      typedef typename ST2toST2Traits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMinus>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ST2toST2ST2toST2ExprWithoutConstIterator<")+Name<A>::getName()+string(",")
	  +Name<B>::getName()+string(",")+Name<OpMinus>::getName()+string(">");
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

      TFEL_MATH_INLINE ST2toST2ST2toST2ExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      ST2toST2ST2toST2ExprWithoutConstIterator(const ST2toST2ST2toST2ExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i,const unsigned short j) const 
      {
	return a(i,j)-b(i,j);
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ST2TOST2ST2TOST2EXPRWITHOUTCONSTITERATORSPECIALISATION_I_ */

