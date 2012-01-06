/*!
 * \file   ScalarMatrixExprSpecialisation.ixx
 * \brief  This file partially specialises the ScalarMatrixExpr and MatrixScalarExpr classes for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_SCALARMATRIXEXPRSPECIALISATION_I_
#define _LIB_TFEL_SCALARMATRIXEXPRSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class ScalarMatrixExpr<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MatrixConcept>::cond));
      
      typedef typename MatrixTraits<B>::NumType   NumTypeB;
      typedef typename MatrixTraits<B>::IndexType IndexType;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      const typename B::RunTimeProperties RTP;
      
      ScalarMatrixExpr();

    public:

      typedef typename ComputeBinaryResult<A,NumTypeB,OpMult>::Handle NumType;
      
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ScalarMatrixExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
      }

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

      TFEL_MATH_INLINE ScalarMatrixExpr(const A& l, const B& r)
	: a(l), b(r), RTP(r.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE ScalarMatrixExpr(const ScalarMatrixExpr<A,B,OpMult>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i,const IndexType j) const 
      {
	return a*b(i,j);
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
    class MatrixScalarExpr<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename MatrixTraits<A>::NumType   NumTypeA;
      typedef typename MatrixTraits<A>::IndexType IndexType;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      
      
      MatrixScalarExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,OpMult>::Handle NumType;
      
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixScalarExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
      }
      
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

      TFEL_MATH_INLINE MatrixScalarExpr(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE MatrixScalarExpr(const MatrixScalarExpr<A,B,OpMult>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i,const IndexType j) const 
      {
	return a(i,j)*b;
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
    class MatrixScalarExpr<A,B,OpDiv>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename MatrixTraits<A>::NumType   NumTypeA;
      typedef typename MatrixTraits<A>::IndexType IndexType;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      const typename A::RunTimeProperties RTP;      
      
      MatrixScalarExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,OpDiv>::Handle NumType;
      
      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixScalarExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpDiv>::getName()+string(">");
      }
      
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

      TFEL_MATH_INLINE MatrixScalarExpr(const A& l, const B& r)
	: a(l), b(r), RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE MatrixScalarExpr(const MatrixScalarExpr<A,B,OpDiv>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i,const IndexType j) const 
      {
	return a(i,j)/b;
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

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_SCALARMATRIXEXPRSPECIALISATION_I_ */

