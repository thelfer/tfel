/*!
 * \file   MatrixMatrixExprSpecialisation.ixx
 * \brief  This file partially specialises the MatrixMatrixExpr class for usual operations.
 * \see    NO_EXPRESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_MATRIXMATRIXEXPRSPECIALISATION_I_
#define _LIB_TFEL_MATRIXMATRIXEXPRSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpPlus.
     */
    template<typename A, typename B>
    class MatrixMatrixExpr<A,B,OpPlus>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MatrixConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpPlus>::Result Result;
      typedef MatrixTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename MatrixTraits<A>::NumType NumTypeA;
      typedef typename MatrixTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;
      
      MatrixMatrixExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpPlus>::Handle NumType;

      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixMatrixExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpPlus>::getName()+string(">");
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MatrixMatrixExpr(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE MatrixMatrixExpr(const A& l, const B& r)
	: a(l), b(r)
      {}
#endif

      TFEL_MATH_INLINE MatrixMatrixExpr(const MatrixMatrixExpr<A,B,OpPlus>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i,const IndexType j) const 
      {
	return a(i,j)+b(i,j);
      }
      
    public:

      typedef ObjectObjectRandomAccessConstIterator<A,B,OpPlus> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b);
      }

      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b);
      }

      TFEL_MATH_INLINE const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      TFEL_MATH_INLINE const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };

    /*
     * \brief Partial Specialisation for OpMinus.
     */
    template<typename A, typename B>
    class MatrixMatrixExpr<A,B,OpMinus>
    {
      
    private:
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,MatrixConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,MatrixConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMinus>::Result Result;
      typedef MatrixTraits<Result> traits;
      typedef typename traits::IndexType IndexType;

      typedef typename MatrixTraits<A>::NumType NumTypeA;
      typedef typename MatrixTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;
      
      MatrixMatrixExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMinus>::Handle NumType;

      static std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixMatrixExpr<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMinus>::getName()+string(">");
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

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE MatrixMatrixExpr(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE MatrixMatrixExpr(const A& l, const B& r)
	: a(l), b(r)
      {}
#endif

      TFEL_MATH_INLINE MatrixMatrixExpr(const MatrixMatrixExpr<A,B,OpMinus>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const IndexType i,const IndexType j) const 
      {
	return a(i,j)-b(i,j);
      }
      
    public:
      
      typedef ObjectObjectRandomAccessConstIterator<A,B,OpMinus> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

      TFEL_MATH_INLINE const_iterator begin(void) const
      {
	return const_iterator(a.begin(),b);
      }

      TFEL_MATH_INLINE const_iterator end(void) const
      {
	return const_iterator(a.end(),b);
      }

      TFEL_MATH_INLINE const_reverse_iterator rbegin(void) const
      {
	return const_reverse_iterator(end());
      }

      TFEL_MATH_INLINE const_reverse_iterator rend(void) const
      {
	return const_reverse_iterator(begin());
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATRIXMATRIXEXPRSPECIALISATION_I_ */

