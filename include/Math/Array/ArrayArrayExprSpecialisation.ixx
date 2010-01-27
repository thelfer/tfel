/*!
 * \file   ArrayArrayExprSpecialisation.ixx
 * \brief  This file partially specialises the ArrayArrayExpr classe for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_MATH_ARRAYARRAYEXPRSPECIALISATION_I_
#define _LIB_TFEL_MATH_ARRAYARRAYEXPRSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpPlus.
     */
    template<typename A, typename B>
    class ArrayArrayExpr<A,B,OpPlus>
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

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayArrayExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpPlus>::getName()+string(">");
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties());
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExpr(const ArrayArrayExpr& src)
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

      typedef ObjectObjectRandomAccessConstIterator<A,B,OpPlus> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      
      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b.begin());
      }
 
      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b.end());
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

    /*
     * \brief Partial Specialisation for OpMinus.
     */
    template<typename A, typename B>
    class ArrayArrayExpr<A,B,OpMinus>
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

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayArrayExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMinus>::getName()+string(">");
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
      
#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties());
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExpr(const ArrayArrayExpr& src)
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
      
      typedef ObjectObjectRandomAccessConstIterator<A,B,OpMinus> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b.begin());
      }
 
      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b.end());
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

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class ArrayArrayExpr<A,B,OpMult>
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

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayArrayExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties());
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExpr(const ArrayArrayExpr& src)
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
      
      typedef ObjectObjectRandomAccessConstIterator<A,B,OpMult> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      
      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b.begin());
      }
 
      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b.end());
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

    /*
     * \brief Partial Specialisation for OpDiv.
     */
    template<typename A, typename B>
    class ArrayArrayExpr<A,B,OpDiv>
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

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayArrayExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpDiv>::getName()+string(">");
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
      
#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), 
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE ArrayArrayExpr(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties());
      {}
#endif

      TFEL_MATH_INLINE ArrayArrayExpr(const ArrayArrayExpr& src)
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
      
      typedef ObjectObjectRandomAccessConstIterator<A,B,OpDiv> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b.begin());
      }
 
      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b.end());
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

#endif /* _LIB_TFEL_MATH_ARRAYARRAYEXPRSPECIALISATION_I_ */

