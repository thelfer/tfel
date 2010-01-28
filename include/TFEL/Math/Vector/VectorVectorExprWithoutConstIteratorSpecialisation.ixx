/*!
 * \file   VectorVectorExprWithoutConstIteratorSpecialisation.ixx
 * \brief  This file partially specialises the
 * VectorVectorExprWithoutConstIterator class for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_VECTORVECTOREXPRWITHOUTCONSTITERATORSPECIALISATION_I_
#define _LIB_TFEL_VECTORVECTOREXPRWITHOUTCONSTITERATORSPECIALISATION_I_ 

#include"TFEL/Math/General/RunTimeCheck.hxx"

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpPlus.
     */
    template<typename A, typename B>
    class VectorVectorExprWithoutConstIterator<A,B,OpPlus>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,VectorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpPlus>::Result Result;
      typedef VectorTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename VectorTraits<A>::NumType NumTypeA;
      typedef typename VectorTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      VectorVectorExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpPlus>::Handle NumType;
      
      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorVectorExprWithoutConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpPlus>::getName()+string(">");
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
      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const VectorVectorExprWithoutConstIterator<A,B,OpPlus>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return a(i)+b(i);
      }

    public:
      
      TFEL_MATH_INLINE const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

    /*
     * \brief Partial Specialisation for OpMinus.
     */
    template<typename A, typename B>
    class VectorVectorExprWithoutConstIterator<A,B,OpMinus>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,VectorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMinus>::Result Result;
      typedef VectorTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename VectorTraits<A>::NumType NumTypeA;
      typedef typename VectorTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      VectorVectorExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMinus>::Handle NumType;
      
      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorVectorExprWithoutConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMinus>::getName()+string(">");
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
      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const VectorVectorExprWithoutConstIterator<A,B,OpMinus>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return a(i)-b(i);
      }

    public:
      
      TFEL_MATH_INLINE const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class VectorVectorExprWithoutConstIterator<A,B,OpMult>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,VectorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMult>::Result Result;
      typedef VectorTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename VectorTraits<A>::NumType NumTypeA;
      typedef typename VectorTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      VectorVectorExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;
      
      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorVectorExprWithoutConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
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
      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const VectorVectorExprWithoutConstIterator<A,B,OpMult>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return a(i)*b(i);
      }

    public:
      
      TFEL_MATH_INLINE const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

    /*
     * \brief Partial Specialisation for OpDiv.
     */
    template<typename A, typename B>
    class VectorVectorExprWithoutConstIterator<A,B,OpDiv>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,VectorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpDiv>::Result Result;
      typedef VectorTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename VectorTraits<A>::NumType NumTypeA;
      typedef typename VectorTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      VectorVectorExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpDiv>::Handle NumType;
      
      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorVectorExprWithoutConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpDiv>::getName()+string(">");
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
      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE VectorVectorExprWithoutConstIterator(const VectorVectorExprWithoutConstIterator<A,B,OpDiv>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return a(i)/b(i);
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

#endif /* _LIB_TFEL_VECTORVECTOREXPRWITHOUTCONSTITERATORSPECIALISATION_I_ */

