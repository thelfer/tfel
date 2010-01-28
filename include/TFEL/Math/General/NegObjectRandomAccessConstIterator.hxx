/*!
 * \file   NegObjectRandomAccessConstIterator.hxx
 * \brief  This file declares the NegObjectRandomAccessConstIterator class
 * \author Helfer Thomas
 * \date   18 Oct 2006
 */

#ifndef _LIB_TFEL_NEGOBJECTRANDOMACCESSCONSTITERATOR_H_
#define _LIB_TFEL_NEGOBJECTRANDOMACCESSCONSTITERATOR_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsRandomAccessIterator.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

namespace tfel{

  namespace math{

    template<typename A>								    
    class NegObjectRandomAccessConstIterator							    
    {											    
      typedef typename A::const_iterator ConstIteratorType;				    
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorType>::cond));   
      typedef typename std::iterator_traits<ConstIteratorType>::value_type NumTypeA;	    
											    
      ConstIteratorType p;								    
											    
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
	return string("NegObjectRandomAccessConstIterator<")+Name<A>::getName()+string(">");             
      }											    
											    
      typedef typename std::iterator_traits<ConstIteratorType>::difference_type Distance;   
											    
      typedef std::random_access_iterator_tag iterator_category;			    
      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;		    
      typedef const NumType   value_type;						    
      typedef Distance difference_type;							    
      typedef const NumType*  pointer;							    
      typedef const NumType&  reference;						    
											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator()						    
      {}										    
											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator(const ConstIteratorType p_)       	    
	: p(p_)										    
      {}										    
											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator(const NegObjectRandomAccessConstIterator& src)	    
	: p(src.p)									    
      {}										    
      											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator& 						    
      operator=(const NegObjectRandomAccessConstIterator& src)					    
      {											    
	/* self assignement does not have to be checked. */				    
	p = src.p;									    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE value_type 							    
      operator*(void) const 								    
      {											    
	return -(*p);									    
      }											    
      											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator& 						    
      operator++(void)									    
      {											    
	p++;										    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator& 						    
      operator++(int)									    
      {											    
	++p;										    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator& 						    
      operator+=(const Distance n)							    
      {											    
	p+=n;										    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator& 						    
      operator+(const Distance n)							    
      {											    
	return NegObjectRandomAccessConstIterator(p+n);						    
      }											    
											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator& 						    
      operator-=(const Distance n)							    
      {											    
	p-=n;										    
	return *this;									    
      }											    
											    
      TFEL_MATH_INLINE NegObjectRandomAccessConstIterator& 						    
      operator-(const Distance n)							    
      {											    
	return NegObjectRandomAccessConstIterator(p-n);						    
      }											    
											    
      TFEL_MATH_INLINE value_type 							    
      operator[](const Distance n) const 						    
      {											    
	return -(p[n]);				              				    
      }						       					    
											    
      TFEL_MATH_INLINE Distance 							    
      operator-(const NegObjectRandomAccessConstIterator& src) const				    
      {											    
	return p-src.p;									    
      }											    
											    
      TFEL_MATH_INLINE bool 							    
      operator==(const NegObjectRandomAccessConstIterator& src) const				    
      {											    
	return p==src.p;								    
      }											    
											    
      TFEL_MATH_INLINE bool 							    
      operator!=(const NegObjectRandomAccessConstIterator& src) const				    
      {											    
	return p!=src.p;								    
      }											    
											    
    };											    
											    
    template<typename A>								    
    TFEL_MATH_INLINE NegObjectRandomAccessConstIterator<A> 						    
    operator+(const typename NegObjectRandomAccessConstIterator<A>::Distance n,			    
	      const NegObjectRandomAccessConstIterator<A>& p)
    {
      return p+n;
    }

  } // end of namespace math

  namespace typetraits{

    template<typename A>
    struct IsInvalid<tfel::math::NegObjectRandomAccessConstIterator< A > >
    {
    private:
      typedef typename tfel::math::NegObjectRandomAccessConstIterator< A >::value_type T;
    public:
      /*!
       *  Result
       */
      static const bool cond = IsInvalid<T>::cond;
    };

  } // end of namespace typetraits

} // end of namespace tfel


#endif /* _LIB_TFEL_NEGOBJECTRANDOMACCESSCONSTITERATOR_H */

