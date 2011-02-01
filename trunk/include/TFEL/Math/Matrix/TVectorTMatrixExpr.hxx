/*!
 * \file   TVectorTMatrixExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 avr 2008
 */

#ifndef _LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX_
#define _LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX_ 

#include<string>
#include<iterator>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/TypeTraits/Promote.hxx"
#include"TFEL/FSAlgorithm/inner_product.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel{

  namespace math {
    
    template<unsigned short N,
	     unsigned short M,
	     typename A, typename B>
    class TVectorTMatrixExpr
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MatrixConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMult>::Result Result;
      typedef VectorTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename VectorTraits<A>::NumType NumTypeA;
      typedef typename MatrixTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      TVectorTMatrixExpr();

      struct ColumnConstIterator
      {	
	typedef typename tfel::meta::IF<IsBTemporary,const B,const B&>::type MType;
	typedef typename MatrixTraits<B>::NumType NumType;
	TFEL_MATH_INLINE ColumnConstIterator(const MType m_,
					     const unsigned short j_)
	  : m(m_),i(0),j(j_)
	{}
	TFEL_MATH_INLINE ColumnConstIterator& operator++ ()
	{
	  ++i;
	  return *this;
	} // end of operator++
	TFEL_MATH_INLINE const NumType &
	operator *(void) const
	{
	  return m(i,j);
	}
      private:
	MType m;
	unsigned short i;
	const unsigned short j;
      }; // end of struc ColumnConstIterator

      struct VectorConstIterator
      {	
	typedef typename tfel::meta::IF<IsBTemporary,const A,const A&>::type VType;
	typedef typename VectorTraits<A>::NumType NumType;
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

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("TVectorTMatrixExpr<")+Name<A>::getName()+string(",")+
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

      TFEL_MATH_INLINE TVectorTMatrixExpr(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE TVectorTMatrixExpr(const TVectorTMatrixExpr<N,M,A,B>& src)
	: a(src.a), b(src.b)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i) const 
      {
	using namespace tfel::fsalgo;
	return inner_product<N>::template exe<NumType>(VectorConstIterator(a),
						       ColumnConstIterator(b,i));
      }

    public:
      
      TFEL_MATH_INLINE const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RunTimeProperties();
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TVECTORTMATRIXEXPR_HXX */

