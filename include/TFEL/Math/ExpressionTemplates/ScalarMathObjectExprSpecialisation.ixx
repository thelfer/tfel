/*!
 * \file   include/TFEL/Math/ExpressionTemplates/ScalarMathObjectExprSpecialisation.ixx
 * \brief  This file partially specialises the ScalarMathObjectExpr and MathObjectScalarExpr classes for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_SCALARMATHOBJECTEXPRSPECIALISATION_I_
#define _LIB_TFEL_SCALARMATHOBJECTEXPRSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B>
    class ScalarMathObjectExpr<MathObjectConcept,MathObjectTraits,A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
  
      typedef typename MathObjectTraits<B>::NumType   NumTypeB;
      
      static constexpr bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename std::conditional<IsBTemporary,const B,const B&>::type b;
      const typename B::RunTimeProperties RTP;
      
      ScalarMathObjectExpr();

    public:
      
      typedef typename ComputeBinaryResult<A,NumTypeB,OpMult>::Handle NumType;
      typedef typename MathObjectTraits<B>::IndexType IndexType;

    protected:

      typedef const A first_arg;
      typedef const B second_arg;
      typedef typename B::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ScalarMathObjectExpr(const A l, const B& r)
	: a(l), b(r), RTP(r.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE ScalarMathObjectExpr(const ScalarMathObjectExpr& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return a*b(i);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a*b(i,j);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a*b(i,j,k);
      }

    public:

      typedef ScalarObjectRandomAccessConstIterator<A,B,OpMult> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE RunTimeProperties 
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
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B>
    class MathObjectScalarExpr<MathObjectConcept,MathObjectTraits,A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename MathObjectTraits<A>::NumType   NumTypeA;
      
      static constexpr bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename std::conditional<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      

      MathObjectScalarExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,OpMult>::Handle NumType;
      typedef typename MathObjectTraits<A>::IndexType IndexType;
      
    protected:

      typedef const A first_arg;
      typedef const B second_arg;
      typedef typename A::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE MathObjectScalarExpr(const A& l, const B r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE MathObjectScalarExpr(const MathObjectScalarExpr& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return a(i)*b;
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a(i,j)*b;
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a(i,j,k)*b;
      }
      
    public:

      typedef ObjectScalarRandomAccessConstIterator<A,B,OpMult> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE RunTimeProperties 
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
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B>
    class MathObjectScalarExpr<MathObjectConcept,MathObjectTraits,A,B,OpDiv>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename MathObjectTraits<A>::NumType   NumTypeA;
      
      static constexpr bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename std::conditional<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      

      MathObjectScalarExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,OpDiv>::Handle NumType;
      typedef typename MathObjectTraits<A>::IndexType IndexType;
      
    protected:

      typedef const A first_arg;
      typedef const B second_arg;
      typedef typename A::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE MathObjectScalarExpr(const A& l, const B r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE MathObjectScalarExpr(const MathObjectScalarExpr& src)
	: a(src.a), b(src.b),RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return a(i)/b;
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a(i,j)/b;
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a(i,j,k)/b;
      }
      
    public:

      typedef ObjectScalarRandomAccessConstIterator<A,B,OpDiv> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE RunTimeProperties 
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


#endif /* _LIB_TFEL_SCALARMATHOBJECTEXPRSPECIALISATION_I_ */

