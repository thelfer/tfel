/*!
 * \file   TMatrixTVectorExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 avr 2008
 */

#ifndef _LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX_
#define _LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX_ 

#include<string>
#include<iterator>

#include "Config/TFELConfig.hxx"
#include "Utilities/Name.hxx"

#include "Metaprogramming/StaticAssert.hxx"
#include "Metaprogramming/Implements.hxx"
#include "TypeTraits/IsTemporary.hxx"
#include "TypeTraits/Promote.hxx"
#include "FSAlgorithm/inner_product.hxx"
#include "Math/General/ResultType.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/General/RunTimeCheck.hxx"
#include "Math/General/ObjectObjectRandomAccessConstIterator.hxx"
#include "Math/Vector/VectorConcept.hxx"

#include"Math/tvector.hxx"
#include"Math/tmatrix.hxx"

namespace tfel{

  namespace math {
    
    template<unsigned short N,
	     unsigned short M,
	     typename A, typename B>
    class TMatrixTVectorExpr
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,VectorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMult>::Result Result;
      typedef VectorTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename MatrixTraits<A>::NumType NumTypeA;
      typedef typename VectorTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      TMatrixTVectorExpr();

      struct RowConstIterator
      {	
	typedef typename tfel::meta::IF<IsATemporary,
					const A,
					const A&>::type MType;

	typedef typename MatrixTraits<A>::NumType NumType;
	TFEL_MATH_INLINE RowConstIterator(const MType m_,
					  const unsigned short i_)
	  : m(m_),i(i_),j(0)
	{}
	TFEL_MATH_INLINE
	RowConstIterator& operator++ ()
	{
	  ++j;
	  return *this;
	} // end of operator++
	
	TFEL_MATH_INLINE
	NumType
	operator *(void) const
	{
	  using namespace std;
	  return m(i,j);
	}
      private:
	MType m;
	const unsigned short i;
	unsigned short j;
      }; // end of struc RowConstIterator

      struct VectorConstIterator
      {	
	typedef typename tfel::meta::IF<IsBTemporary,const B,const B&>::type VType;
	typedef typename VectorTraits<B>::NumType NumType;
	TFEL_MATH_INLINE VectorConstIterator(const VType v_)
	  : v(v_),i(0)
	{}
	TFEL_MATH_INLINE VectorConstIterator& operator++ ()
	{
	  ++i;
	  return *this;
	} // end of operator++
	TFEL_MATH_INLINE const NumType &
	operator *(void) const
	{
	  return v(i);
	}
      private:
	VType v;
	unsigned short i;
      }; // end of struc VectorConstIterator

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;
      typedef EmptyRunTimeProperties RunTimeProperties;

      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("TMatrixTVectorExpr<")+Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE TMatrixTVectorExpr(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE
      TMatrixTVectorExpr(const TMatrixTVectorExpr<N,M,A,B>& src)
	: a(src.a), b(src.b)
      {}

      TFEL_MATH_INLINE
      NumType 
      operator()(const IndexType i) const 
      {
	using namespace tfel::fsalgo;
	return inner_product<M>::template exe<NumType>(RowConstIterator(a,i),
						       VectorConstIterator(b));
      }

    public:
      
      TFEL_MATH_INLINE
      RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RunTimeProperties();
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TMATRIXTVECTOREXPR_HXX */

