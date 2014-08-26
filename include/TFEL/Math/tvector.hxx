/*!
 * \file   tvector.hxx  
 * \brief    
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_TINY_VECTOR_
#define _LIB_TFEL_TINY_VECTOR_ 1

#include<string>
#include<ostream>
#include<cstddef>
#include<iterator>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/TypeTraits/IsAssignableTo.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/RealPartType.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"

#include"TFEL/FSAlgorithm/copy.hxx"

#include"TFEL/Math/fsarray.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Vector/VectorConceptOperations.hxx"
#include"TFEL/Math/Vector/VectorExpr.hxx"
#include"TFEL/Math/Vector/TinyVectorFromTinyVectorView.hxx"

#include"TFEL/Math/Forward/tmatrix.hxx"
#include"TFEL/Math/Forward/tvector.hxx"

namespace tfel{
  
  namespace math{

    //! Partial specialisation for tvectors.
    /*!
     * This is a VectorConcept requirement.
     * \see VectorTraits.
     */
    template<unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL VectorTraits<tvector<N,T> >{
      //! the type holded by the tvector.
      typedef T NumType;
      //! the type of the index used by the tvector.
      typedef unsigned short IndexType;
      //! a simple alias to the tvector runtime properties
      typedef EmptyRunTimeProperties RunTimeProperties;
    };
    
    /*!
     * An helper class to deal with limitation of Visual Studio 10
     */
    template<typename T,
	     typename T2,
	     typename Op>
    struct IsTVectorScalarOperationValid
    {
      static const bool cond = tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond;
    }; // end of struct IsTVectorScalarOperationValid

    /*!
     * \brief a base for tvector or classes acting like tvector.
     */
    template<typename Child,
	     unsigned short N,
	     typename T>
    struct tvector_base
    {
      //! Assignement operator
      /*!
       * \param const VectorExpr<tvector<N,T2>,Expr>&, a vector expression.
       * \return a reference to this.
       * \rec T2 must be assignable to a T.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Child&
	>::type
      operator=(const VectorExpr<tvector<N,T2>,Expr>&);
      
      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Child&
	>::type
      operator=(const tvector<N,T2>&);

      // Assignement operator
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Child&
	>::type
      operator+=(const VectorExpr<tvector<N,T2>,Expr>&);

      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Child&
	>::type
      operator+=(const tvector<N,T2>&);
    
      // Assignement operator
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Child&
	>::type
      operator-=(const VectorExpr<tvector<N,T2>,Expr>&);
    
      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE
      typename 
      tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	Child&
	>::type
      operator-=(const tvector<N,T2>&);
    
      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	IsTVectorScalarOperationValid<T,T2,OpMult>::cond,
	Child&
      >::type
      operator*=(const T2);

      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	IsTVectorScalarOperationValid<T,T2,OpDiv>::cond,
	Child&
      >::type
      operator/=(const T2);
    };    

    template<unsigned short N, typename T = double>
    struct TFEL_VISIBILITY_LOCAL tvector
      : public VectorConcept<tvector<N,T> >,
	public tvector_base<tvector<N,T>,N,T>,
	public fsarray<N,T>
    {
      //! a simple typedef to the tvector runtime properties
      /*!
       * This is a VectorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * Default constructor.
       */
      tvector();
      /*!
       * Constructor from a scalar.
       * \param const T : initial value.
       */
      explicit
      tvector(const T);

      /*!
       * Constructor from a pointer.
       * \param const T* : initial values.
       */
      explicit
      tvector(const T*const);

      //! Assignement operator
      /*!
       * \param const VectorExpr<tvector<N,T2>,Expr>&, a vector expression.
       * \return tvector<N,T>& a reference to this.
       * \rec T2 must be assignable to a T.
       */
      template<typename T2,typename Expr>
      tvector(const VectorExpr<tvector<N,T2>,Expr>&);

      //! using tvector_base::operator=
      using tvector_base<tvector,N,T>::operator=;
      /*!
       * \brief index operator.
       * This is a vector concept requirement.
       * \param const unsigned short, index.
       * \return const T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE
      const T& 
      operator()(const unsigned short) const;
      
      /*!
       * \brief index operator.
       * \param const unsigned short, index.
       * \return T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE
      T& operator()(const unsigned short);
      /*!
       * \brief index operator.
       * This is a vector concept requirement.
       * \param const unsigned short, index.
       * \return const T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE
      const T& 
      operator[](const unsigned short) const;
      /*!
       * \brief index operator.
       * \param const unsigned short, index.
       * \return T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE
      T& operator[](const unsigned short);
      /*!
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const;
      /*!
       * copy the Nth elements following this argument.
       * \param const InputIterator, an iterator to the first element
       * to be copied.
       */
      template<typename InputIterator>
      TFEL_MATH_INLINE void 
      copy(const InputIterator src);
      /*!
       * \brief create a slice
       * \param[in] I : the starting index
       */
      template<unsigned short I>
      VectorExpr<tvector<N-I,T>,TinyVectorFromTinyVectorViewExpr<N-I,N,I,T,false> >
      slice(void);
      /*!
       * \brief create a slice
       * \param[in] I : the starting index
       * \param[in] J : the size of the slice
       * \note : the slice object contains a reference to the source
       * vector, so this vector shall not be destroyed before the slice
       */
      template<unsigned short I,
	       unsigned short J>
      VectorExpr<tvector<J-I,T>,TinyVectorFromTinyVectorViewExpr<J-I,N,I,T,false> >
      slice(void);
      /*!
       * \brief create a slice (const version)
       * \param[in] I : the starting index
       * \note : the slice object contains a reference to the source
       * vector, so this vector shall not be destroyed before the slice
       */
      template<unsigned short I>
      VectorExpr<tvector<N-I,T>,TinyVectorFromTinyVectorViewExpr<N-I,N,I,T,true> >
      slice(void) const;
      /*!
       * \brief create a slice (const version)
       * \param[in] I : the starting index
       * \param[in] J : the size of the slice
       * \note : the slice object contains a reference to the source
       * vector, so this vector shall not be destroyed before the slice
       */
      template<unsigned short I,
	       unsigned short J>
      VectorExpr<tvector<J-I,T>,TinyVectorFromTinyVectorViewExpr<J-I,N,I,T,true> >
      slice(void) const;
    private:
      //! a simple assertion stating that the dimension is valid.
      TFEL_STATIC_ASSERT(N!=0);
    };

    /*!
     * export the given vector to an array of the 
     */
    template<unsigned short N, typename T,
	     typename OutputIterator>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      void>::type
    exportToBaseTypeArray(const tvector<N,T>&,
			  OutputIterator);

    template<unsigned short N, typename T>
    TFEL_MATH_INLINE2
    typename tfel::typetraits::AbsType<T>::type
    abs(const tvector<N,T>& v);

    //! Serialisation operator
    /*!
     * \param  std::ostream&, stream to which the vector is serialized.
     * \param  const tvector<N,T>&, the tvector serialized.
     * \return std::ostream&, stream to which the vector has been serialized.
     */
    template<unsigned short N,typename T>
    std::ostream&
    operator << (std::ostream &, const tvector<N,T>&);

    template<typename T>
    tvector<1u,T>
    makeTVector1D(const T);

    template<typename T>
    tvector<2u,T>
    makeTVector2D(const T,
		  const T);
    
    template<typename T>
    tvector<3u,T>
    makeTVector3D(const T,
		  const T,
		  const T);

    template<typename T>
    tvector<3u,T>
    cross_product(const tvector<2u,T>&,
		  const tvector<2u,T>&);

    template<typename T>
    tvector<3u,T>
    cross_product(const tvector<3u,T>&,
		  const tvector<3u,T>&);
    /*!
     * \brief create a slice from a tiny vector
     * \param[in] v : vector
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template<unsigned short I,unsigned short N,typename T>
    VectorExpr<tvector<N-I,T>,TinyVectorFromTinyVectorViewExpr<N-I,N,I,T,false> >
    slice(tvector<N,T>&);
    /*!
     * \brief create a slice from a tiny vector
     * \param[in] v : vector
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template<unsigned short I,unsigned short J,
	     unsigned short N,typename T>
    VectorExpr<tvector<J-I,T>,TinyVectorFromTinyVectorViewExpr<J-I,N,I,T,false> >
    slice(tvector<N,T>&);
    /*!
     * \brief create a slice from a tiny vector
     * \param[in] v : vector
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template<unsigned short I,unsigned short N,typename T>
    VectorExpr<tvector<N-I,T>,TinyVectorFromTinyVectorViewExpr<N-I,N,I,T,true> >
    slice(const tvector<N,T>&);
    /*!
     * \brief create a slice from a tiny vector (const version)
     * \param[in] v : vector
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template<unsigned short I,unsigned short J,
	     unsigned short N,typename T>
    VectorExpr<tvector<J-I,T>,TinyVectorFromTinyVectorViewExpr<J-I,N,I,T,true> >
    slice(const tvector<N,T>&);

  } // end of namespace math

  namespace typetraits{
    
    /*!
     * Partial specialisation for tvectors
     */
    template<unsigned short N,typename T2,typename T>
    struct IsAssignableTo<tfel::math::tvector<N,T2>,tfel::math::tvector<N,T> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };

  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/Math/Vector/tvector.ixx"
#include"TFEL/Math/Vector/tvectorResultType.hxx"

#endif /* _LIB_TFEL_TINY_VECTOR_ */
