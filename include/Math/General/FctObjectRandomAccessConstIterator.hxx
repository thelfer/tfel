/*!
 * \file   FctObjectRandomAccessConstIterator.hxx
 * \brief  This file declares the FctObjectRandomAccessConstIterator class
 * \author Helfer Thomas
 * \date   17 Oct 2006
 */

#ifndef _LIB_TFEL_FCTOBJECTRANDOMACCESSCONSTITERATOR_H_
#define _LIB_TFEL_FCTOBJECTRANDOMACCESSCONSTITERATOR_H_ 

#include"Config/TFELConfig.hxx"
#include"Metaprogramming/StaticAssert.hxx"
#include"TypeTraits/IsScalar.hxx"
#include"TypeTraits/IsInvalid.hxx"
#include"TypeTraits/IsRandomAccessIterator.hxx"
#include"Math/General/ComputeUnaryResult.hxx"
#include"Math/General/BasicOperations.hxx"
#include"Math/Function/StandardFunctions.hxx"

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
											    
      /*!										    
       * brief  Return the name of the class.						    
       * param  void.									    
       * return std::string, the name of the class.					    
       * see    Name.									    
       */										    
      static const std::string getName(void){							    
	using namespace std;								    
	using namespace tfel::utilities;						    
	return string("FctObjectConstIterator<")
	  +Name<A>::getName()+string(",")
	  +Name<Fct>::getName()+string(">");             
      }											    
											    
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

