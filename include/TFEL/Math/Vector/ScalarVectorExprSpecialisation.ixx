/*!
 * \file   ScalarVectorExprSpecialisation.ixx
 * \brief  This file partially specialises the ScalarVectorExpr and VectorScalarExpr classes for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_SCALARVECTOREXPRSPECIALISATION_I_
#define _LIB_TFEL_SCALARVECTOREXPRSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class ScalarVectorExpr<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,VectorConcept>::cond));
  
      typedef typename VectorTraits<B>::NumType   NumTypeB;
      typedef typename VectorTraits<B>::IndexType IndexType;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      const typename B::RunTimeProperties RTP;
      
      ScalarVectorExpr();

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
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ScalarVectorExpr(const A l, const B& r)
	: a(l), b(r), RTP(r.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE ScalarVectorExpr(const ScalarVectorExpr<A,B,OpMult>& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return a*b(i);
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
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class VectorScalarExpr<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename VectorTraits<A>::NumType   NumTypeA;
      typedef typename VectorTraits<A>::IndexType IndexType;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      

      VectorScalarExpr();

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
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE VectorScalarExpr(const A& l, const B r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE VectorScalarExpr(const VectorScalarExpr<A,B,OpMult>& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return a(i)*b;
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
    class VectorScalarExpr<A,B,OpDiv>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename VectorTraits<A>::NumType   NumTypeA;
      typedef typename VectorTraits<A>::IndexType IndexType;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      

      VectorScalarExpr();

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
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE VectorScalarExpr(const A& l, const B r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE VectorScalarExpr(const VectorScalarExpr<A,B,OpDiv>& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	return a(i)/b;
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


#endif /* _LIB_TFEL_SCALARVECTOREXPRSPECIALISATION_I_ */

