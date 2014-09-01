/*!
 * \file   include/TFEL/Math/General/FctObjectRandomAccessConstIterator.hxx
 * \brief  This file declares the FctObjectRandomAccessConstIterator class
 * \author Helfer Thomas
 * \date   17 Oct 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_FCTOBJECTRANDOMACCESSCONSTITERATOR_H_
#define _LIB_TFEL_FCTOBJECTRANDOMACCESSCONSTITERATOR_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsRandomAccessIterator.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Function/StandardFunctions.hxx"

namespace tfel{

  namespace math{
    
    template<typename A,typename Fct>
    class FctObjectRandomAccessConstIterator						    
    {											    
      typedef typename A::const_iterator ConstIteratorType;				    
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorType>::cond));   
      typedef typename std::iterator_traits<ConstIteratorType>::value_type NumTypeA;	    
											    
      ConstIteratorType p;
      Fct f;
											    
    public:										    

      typedef typename std::iterator_traits<ConstIteratorType>::difference_type Distance;   
											    
      typedef std::random_access_iterator_tag iterator_category;			    
      typedef typename ComputeUnaryResult<NumTypeA,Fct>::Handle NumType;		    
      typedef const NumType   value_type;						    
      typedef Distance difference_type;							    
      typedef const NumType*  pointer;							    
      typedef const NumType&  reference;						    
											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator()				    
      {}										    
											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator(const ConstIteratorType p_,
							  const Fct f_)       
	: p(p_), f(f_)
      {}										    
											    
      TFEL_MATH_INLINE									    
      FctObjectRandomAccessConstIterator(const FctObjectRandomAccessConstIterator& src)	    
	: p(src.p), f(src.f)
      {}										    
      											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator& 				    
      operator=(const FctObjectRandomAccessConstIterator& src)				    
      {											    
	/* self assignement does not have to be checked. */				    
	p = src.p;									    
	f = src.f;
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE value_type 							    
      operator*(void) const 								    
      {											    
	return f(*p);
      }
      											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator&
      operator++(void)									    
      {											    
	p++;										    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator& 				    
      operator++(int)									    
      {											    
	++p;										    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator& 				    
      operator+=(const Distance n)							    
      {											    
	p+=n;										    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator& 				    
      operator+(const Distance n)							    
      {											    
	return FctObjectRandomAccessConstIterator(p+n);					    
      }											    
											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator& 				    
      operator-=(const Distance n)							    
      {											    
	p-=n;										    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE FctObjectRandomAccessConstIterator& 				    
      operator-(const Distance n)							    
      {											    
	return FctObjectRandomAccessConstIterator(p-n);					    
      }											    
											    
      TFEL_MATH_INLINE value_type 							    
      operator[](const Distance n) const 						    
      {											    
	return f(p[n]);				              				    
      }						       					    
											    
      TFEL_MATH_INLINE Distance 							    
      operator-(const FctObjectRandomAccessConstIterator& src) const			    
      {											    
	return p-src.p;									    
      }											    
											    
      TFEL_MATH_INLINE bool 							    
      operator==(const FctObjectRandomAccessConstIterator& src) const			    
      {											    
	return p==src.p;								    
      }											    
											    
      TFEL_MATH_INLINE bool 							    
      operator!=(const FctObjectRandomAccessConstIterator& src) const			    
      {											    
	return p!=src.p;								    
      }											    
											    
    };											    
											    
    template<typename A,typename Fct>
    TFEL_MATH_INLINE FctObjectRandomAccessConstIterator<A,Fct>
    operator+(const typename FctObjectRandomAccessConstIterator<A,Fct>::Distance n,
	      const FctObjectRandomAccessConstIterator<A,Fct>& p)	    
    {
      return p+n;
    }

  } // end of namespace math

  namespace typetraits{

    template<typename A,typename Fct>
    struct IsInvalid<tfel::math::FctObjectRandomAccessConstIterator<A,Fct> >
    {											    
    private:		
      typedef tfel::math::FctObjectRandomAccessConstIterator<A,Fct> FctIterator;
      typedef typename FctIterator::value_type T;               
    public:										    
      /*!										    
       *  Result									    
       */										    
      static const bool cond = IsInvalid<T>::cond;                                          
    };

  } // end of namespace typetraits

} // end of namespace tfel

#endif /* _LIB_TFEL_FCTOBJECTRANDOMACCESSCONSTITERATOR_H */

