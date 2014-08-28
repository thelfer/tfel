/*!
 * \file   include/TFEL/Math/Vector/Range.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 oct 2006
 */

#ifndef _LIB_TFEL_RANGE_HXX_
#define _LIB_TFEL_RANGE_HXX_ 

#include<cstddef>

#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Vector/VectorExpr.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/tvector.hxx"

namespace tfel{

  namespace math{

    template<typename NumType_,typename IntegerType>
    struct Range
    {

      typedef NumType_ NumType;

      struct RangeData
      {
	NumType     start_;
	NumType     delta_;
	IntegerType stepNumber_;
	
	RangeData(const NumType start,const NumType stop,const IntegerType stepNumber)
	  : start_(start),delta_(stop-start),stepNumber_(stepNumber)
	{}
	
      };

    protected:

      typedef const RangeData         first_arg;
      typedef tfel::meta::InvalidType second_arg;

      typedef tfel::meta::EmptyClass RunTimeProperties;
      typedef NumType                value_type;                                                
      typedef NumType*               pointer;	    						
      typedef const NumType*         const_pointer; 						
      typedef NumType&               reference;	    						
      typedef const NumType&         const_reference;						
      typedef IntegerType            size_type;	    						
      typedef ptrdiff_t              difference_type;

      NumType     start_;
      NumType     delta_;
      IntegerType stepNumber_;

    protected:

      Range(const RangeData& src)
	: start_(src.start_),delta_(src.delta_),stepNumber_(src.stepNumber_)
      {}

      Range(const Range& src)
	: start_(src.start_),delta_(src.delta_),stepNumber_(src.stepNumber_)
      {}


      const NumType operator()(const IntegerType i) const
      {
	return this->start_+(this->delta_)*i;///(this->stepNumber_);
      }

    };

    template<typename VectorType = vector<double> >
    class range
    {

      typedef Range<typename VectorTraits<VectorType>::NumType,
		    typename VectorTraits<VectorType>::IndexType> RangeType;
      typedef typename RangeType::RangeData RangeData;
      typedef VectorExpr<VectorType,RangeType> Result;

    public:

      static Result exe(typename VectorTraits<VectorType>::NumType   a, 
			typename VectorTraits<VectorType>::NumType   b,
			typename VectorTraits<VectorType>::IndexType nb)
      {
	return Result(RangeData(a,b,nb));
      }

    };

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_RANGE_HXX */

