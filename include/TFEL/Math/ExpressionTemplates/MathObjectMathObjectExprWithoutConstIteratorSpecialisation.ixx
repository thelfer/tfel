/*!
 * \file   include/TFEL/Math/ExpressionTemplates/MathObjectMathObjectExprWithoutConstIteratorSpecialisation.ixx
 * \brief  This file partially specialises the
 * MathObjectMathObjectExprWithoutConstIterator class for usual operations.
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

#ifndef _LIB_TFEL_MATHOBJECTMATHOBJECTEXPRWITHOUTCONSTITERATORSPECIALISATION_I_
#define _LIB_TFEL_MATHOBJECTMATHOBJECTEXPRWITHOUTCONSTITERATORSPECIALISATION_I_ 

#include"TFEL/Math/General/RunTimeCheck.hxx"

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpPlus.
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B>
    class MathObjectMathObjectExprWithoutConstIterator<MathObjectConcept,MathObjectTraits,A,B,OpPlus>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpPlus>::Result Result;
      typedef MathObjectTraits<Result> traits;

      typedef typename MathObjectTraits<A>::NumType NumTypeA;
      typedef typename MathObjectTraits<B>::NumType NumTypeB;
  
      static constexpr bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static constexpr bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename std::conditional<IsATemporary,const A,const A&>::type a;
      typename std::conditional<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      MathObjectMathObjectExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpPlus>::Handle NumType;
      typedef typename traits::IndexType IndexType;
      
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const MathObjectMathObjectExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return a(i)+b(i);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a(i,j)+b(i,j);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a(i,j,k)+b(i,j,k);
      }

    public:
      
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

    /*
     * \brief Partial Specialisation for OpMinus.
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B>
    class MathObjectMathObjectExprWithoutConstIterator<MathObjectConcept,MathObjectTraits,A,B,OpMinus>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMinus>::Result Result;
      typedef MathObjectTraits<Result> traits;

      typedef typename MathObjectTraits<A>::NumType NumTypeA;
      typedef typename MathObjectTraits<B>::NumType NumTypeB;
  
      static constexpr bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static constexpr bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename std::conditional<IsATemporary,const A,const A&>::type a;
      typename std::conditional<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      MathObjectMathObjectExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMinus>::Handle NumType;
      typedef typename traits::IndexType IndexType;
      
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const MathObjectMathObjectExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return a(i)-b(i);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a(i,j)-b(i,j);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a(i,j,k)-b(i,j,k);
      }

    public:
      
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B>
    class MathObjectMathObjectExprWithoutConstIterator<MathObjectConcept,MathObjectTraits,A,B,OpMult>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMult>::Result Result;
      typedef MathObjectTraits<Result> traits;

      typedef typename MathObjectTraits<A>::NumType NumTypeA;
      typedef typename MathObjectTraits<B>::NumType NumTypeB;
  
      static constexpr bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static constexpr bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename std::conditional<IsATemporary,const A,const A&>::type a;
      typename std::conditional<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      MathObjectMathObjectExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;
      typedef typename traits::IndexType IndexType;
      
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const MathObjectMathObjectExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return a(i)*b(i);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a(i,j)*b(i,j);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a(i,j,k)*b(i,j,k);
      }

    public:
      
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

    /*
     * \brief Partial Specialisation for OpDiv.
     */
    template<template<typename> class MathObjectConcept,
	     template<typename> class MathObjectTraits,
	     typename A, typename B>
    class MathObjectMathObjectExprWithoutConstIterator<MathObjectConcept,MathObjectTraits,A,B,OpDiv>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MathObjectConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MathObjectConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpDiv>::Result Result;
      typedef MathObjectTraits<Result> traits;

      typedef typename MathObjectTraits<A>::NumType NumTypeA;
      typedef typename MathObjectTraits<B>::NumType NumTypeB;
  
      static constexpr bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static constexpr bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename std::conditional<IsATemporary,const A,const A&>::type a;
      typename std::conditional<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      MathObjectMathObjectExprWithoutConstIterator();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpDiv>::Handle NumType;
      typedef typename traits::IndexType IndexType;
      
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE MathObjectMathObjectExprWithoutConstIterator(const MathObjectMathObjectExprWithoutConstIterator& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i) const 
      {
	return a(i)/b(i);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j) const 
      {
	return a(i,j)/b(i,j);
      }

      TFEL_MATH_INLINE NumType 
      operator()(const IndexType i,
		 const IndexType j,
		 const IndexType k) const 
      {
	return a(i,j,k)/b(i,j,k);
      }

    public:

      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATHOBJECTMATHOBJECTEXPRWITHOUTCONSTITERATORSPECIALISATION_I_ */

