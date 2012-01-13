/*!
 * \file   ScalarArrayExprSpecialisation.ixx
 * \brief  This file partially specialises the ScalarArrayExpr and ArrayScalarExpr classes for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_SCALARARRAYEXPRSPECIALISATION_I_
#define _LIB_TFEL_SCALARARRAYEXPRSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class ScalarArrayExpr<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ArrayConcept>::cond));
      
      typedef typename ArrayTraits<B>::NumType   NumTypeB;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename B::RunTimeProperties RTP;
      
    public:

      typedef typename ComputeBinaryResult<A,NumTypeB,OpMult>::Handle NumType;
      
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

      TFEL_MATH_INLINE ScalarArrayExpr(const ScalarArrayExpr<A,B,OpMult>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return a*b(i);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,const unsigned int j) const 
      {
	return a*b(i,j);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,const unsigned int j,const unsigned int k) const 
      {
	return a*b(i,j,k);
      }

    public:

      typedef ScalarObjectRandomAccessConstIterator<A,B,OpMult> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      
      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      const_iterator begin(void) const
      {
	return const_iterator(a,b.begin());
      }

      const_iterator end(void) const
      {
	return const_iterator(a,b.end());
      }

      const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };

    /*
     * \brief Partial Specialisation for OpDiv.
     */
    template<typename A, typename B>
    class ScalarArrayExpr<A,B,OpDiv>
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,ArrayConcept>::cond));
      
      typedef typename ArrayTraits<B>::NumType   NumTypeB;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename B::RunTimeProperties RTP;
      
    public:

      typedef typename ComputeBinaryResult<A,NumTypeB,OpDiv>::Handle NumType;

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

      TFEL_MATH_INLINE ScalarArrayExpr(const ScalarArrayExpr<A,B,OpDiv>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i) const 
      {
	return a/b(i);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,const unsigned int j) const 
      {
	return a/b(i,j);
      }

      TFEL_MATH_INLINE const NumType
      operator()(const unsigned int i,const unsigned int j,const unsigned int k) const 
      {
	return a/b(i,j,k);
      }

    public:
      
      typedef ScalarObjectRandomAccessConstIterator<A,B,OpDiv> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      const_iterator begin(void) const
      {
	return const_iterator(a,b.begin());
      }

      const_iterator end(void) const
      {
	return const_iterator(a,b.end());
      }

      const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class ArrayScalarExpr<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename ArrayTraits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;

    public:


      typedef typename ComputeBinaryResult<NumTypeA,B,OpMult>::Handle NumType;

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
	return a(i)*b;
      }

      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned int i,const unsigned int j) const 
      {
	return a(i,j)*b;
      }

      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned int i,const unsigned int j,const unsigned int k) const 
      {
	return a(i,j,k)*b;
      }
      
    public:
      
      typedef ObjectScalarRandomAccessConstIterator<A,B,OpMult> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b);
      }

      const_iterator end(void) const
      {
	return const_iterator(a.end(),b);
      }

      const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };

    /*
     * \brief Partial Specialisation for OpDiv.
     */
    template<typename A, typename B>
    class ArrayScalarExpr<A,B,OpDiv>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,ArrayConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename ArrayTraits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;

    public:


      typedef typename ComputeBinaryResult<NumTypeA,B,OpDiv>::Handle NumType;

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
	return a(i)/b;
      }

      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned int i,const unsigned int j) const 
      {
	return a(i,j)/b;
      }

      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned int i,const unsigned int j,const unsigned int k) const 
      {
	return a(i,j,k)/b;
      }
      
    public:
      
      typedef ObjectScalarRandomAccessConstIterator<A,B,OpDiv> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b);
      }

      const_iterator end(void) const
      {
	return const_iterator(a.end(),b);
      }

      const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_SCALARARRAYEXPRSPECIALISATION_I_ */

