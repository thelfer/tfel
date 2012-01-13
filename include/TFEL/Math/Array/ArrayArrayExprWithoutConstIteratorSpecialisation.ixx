/*!
 * \file   ArrayArrayExprWithoutConstIteratorSpecialisation.ixx
 * \brief  This file partially specialises the ArrayArrayExprWithoutConstIterator classe for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_MATH_ARRAYARRAYEXPRWITHOUTCONSTITERATORSPECIALISATION_I_
#define _LIB_TFEL_MATH_ARRAYARRAYEXPRWITHOUTCONSTITERATORSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpPlus.
     */
    template<typename A, typename B>
    class ArrayArrayExprWithoutConstIterator<A,B,OpPlus>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT(ArrayTraits<A>::dimension==ArrayTraits<B>::dimension);
      TFEL_STATIC_ASSERT((tfel::meta::IsSameType<typename A::RunTimeProperties,
		                            typename B::RunTimeProperties>::cond));

      typedef typename ComputeBinaryResult<A,B,OpPlus>::Result Result;
      typedef ArrayTraits<Result> traits;

      typedef typename ArrayTraits<A>::NumType NumTypeA;
      typedef typename ArrayTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
      const typename A::RunTimeProperties RTP;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpPlus>::Handle NumType;

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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties());
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const ArrayArrayExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return a(i)+b(i);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i, 
		 const unsigned int j) const 
      {
	return a(i,j)+b(i,j);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,
		 const unsigned int j,
		 const unsigned int k) const 
      {
	return a(i,j,k)+b(i,j,k);
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
    class ArrayArrayExprWithoutConstIterator<A,B,OpMinus>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT(ArrayTraits<A>::dimension==ArrayTraits<B>::dimension);
      TFEL_STATIC_ASSERT((tfel::meta::IsSameType<typename A::RunTimeProperties,
		                            typename B::RunTimeProperties>::cond));

      typedef typename ComputeBinaryResult<A,B,OpMinus>::Result Result;
      typedef ArrayTraits<Result> traits;

      typedef typename ArrayTraits<A>::NumType NumTypeA;
      typedef typename ArrayTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
      const typename A::RunTimeProperties RTP;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMinus>::Handle NumType;

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
      
#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties());
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const ArrayArrayExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return a(i)-b(i);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i, 
		 const unsigned int j) const 
      {
	return a(i,j)-b(i,j);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,
		 const unsigned int j,
		 const unsigned int k) const 
      {
	return a(i,j,k)-b(i,j,k);
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
    class ArrayArrayExprWithoutConstIterator<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT(ArrayTraits<A>::dimension==ArrayTraits<B>::dimension);
      TFEL_STATIC_ASSERT((tfel::meta::IsSameType<typename A::RunTimeProperties,
		                            typename B::RunTimeProperties>::cond));

      typedef typename ComputeBinaryResult<A,B,OpMult>::Result Result;
      typedef ArrayTraits<Result> traits;

      typedef typename ArrayTraits<A>::NumType NumTypeA;
      typedef typename ArrayTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
      const typename A::RunTimeProperties RTP;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;

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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties());
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const ArrayArrayExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return a(i)*b(i);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i, 
		 const unsigned int j) const 
      {
	return a(i,j)*b(i,j);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,
		 const unsigned int j,
		 const unsigned int k) const 
      {
	return a(i,j,k)*b(i,j,k);
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
    class ArrayArrayExprWithoutConstIterator<A,B,OpDiv>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT(ArrayTraits<A>::dimension==ArrayTraits<B>::dimension);
      TFEL_STATIC_ASSERT((tfel::meta::IsSameType<typename A::RunTimeProperties,
		                            typename B::RunTimeProperties>::cond));

      typedef typename ComputeBinaryResult<A,B,OpDiv>::Result Result;
      typedef ArrayTraits<Result> traits;

      typedef typename ArrayTraits<A>::NumType NumTypeA;
      typedef typename ArrayTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
      const typename A::RunTimeProperties RTP;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpDiv>::Handle NumType;

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
      
#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties());
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExprWithoutConstIterator(const ArrayArrayExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return a(i)/b(i);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i, 
		 const unsigned int j) const 
      {
	return a(i,j)/b(i,j);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,
		 const unsigned int j,
		 const unsigned int k) const 
      {
	return a(i,j,k)/b(i,j,k);
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

#endif /* _LIB_TFEL_MATH_ARRAYARRAYEXPRWITHOUTCONSTITERATORSPECIALISATION_I_ */

