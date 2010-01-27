/*!
 * \file   tmatrix.hxx  
 * \brief    
 * \author Helfer Thomas
 */

#ifndef _TFEL_MATH_TINY_MATRIX_LIB_
#define _TFEL_MATH_TINY_MATRIX_LIB_ 1

#include <string>
#include <ostream>

#include "Config/TFELConfig.hxx"
#include "Utilities/Name.hxx"
#include "Utilities/ToString.hxx"
#include "Metaprogramming/StaticAssert.hxx"
#include "TypeTraits/IsAssignableTo.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/General/EmptyRunTimeProperties.hxx"
#include "Math/Matrix/MatrixConcept.hxx"
#include "Math/Matrix/MatrixConceptOperations.hxx"
#include "Math/Matrix/MatrixExpr.hxx"

namespace tfel{
  
  namespace math{
    
    /*!
     * \class tmatrix.
     * \brief a class representing tiny matrixs of fixed size.
     * \param unsigned short, the number of rows hold by the tmatrix.
     * \param unsigned short, the number of columns hold by the tmatrix.
     * \param typename T, the type hold by the tmatrix.
     */
    template<unsigned short N,unsigned short M, typename T>
    class tmatrix;

    //! Partial specialisation for tmatrixs.
    /*
     * This is a MatrixConcept requirement.
     * \see MatrixTraits.
     */
    template<unsigned short N,unsigned short M, typename T>
    struct MatrixTraits<tmatrix<N,M,T> >{
      //! the type holded by the tmatrix.
      typedef T NumType;
      //! the type of the index used by the tmatrix.
      typedef unsigned short IndexType;
    };
    
    template<unsigned short N,unsigned short M, typename T = double>
    class tmatrix
      : public MatrixConcept<tmatrix<N,M,T> >
    {
      //! a simple assertion stating that the number of row is valid.
      TFEL_STATIC_ASSERT(N!=0);
      //! a simple assertion stating that the number of column is valid.
      TFEL_STATIC_ASSERT(M!=0);
      // values hold by the tmatrix.
      T m[N*M];
      
    public:
      
      //! a simple typedef to the tmatrix runtime properties
      /*
       * This is a MatrixConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;

      /*!
       * type of the tmatrix's values.
       * (this is a stl requirement).
       */
      typedef T value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type* pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type* const_pointer;
      /*!
       * type of the tmatrix's iterator.
       * (provided for stl compatibility).
       */
      typedef pointer iterator;
      /*!
       * type of the tmatrix's const iterator.
       * (provided for stl compatibility).
       */
      typedef const_pointer const_iterator;
      /*!
       * type of the tmatrix's reverse iterator.
       * (provided for stl compatibility).
       */
#ifdef __GNUG__
      typedef std::reverse_iterator<iterator> reverse_iterator; 
#else
      typedef std::reverse_iterator<iterator,T,
      				    reference,
      				    difference_type> reverse_iterator;
#endif
      /*!
       * type of the tmatrix's const reverse iterator.
       * (provided for stl compatibility).
       */
#ifdef __GNUG__
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator; 
#else
      typedef std::reverse_iterator<const_iterator,T,
      				    const_reference,
      				    difference_type> const_reverse_iterator;
#endif
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type& reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type& const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef size_t size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      typedef ptrdiff_t difference_type;

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("tmatrix<")+ToString(N)+string(",")+
	  ToString(M)+string(",")+Name<T>::getName()+string(">");
      }

      /*
       * Default constructor.
       */
      TFEL_MATH_INLINE
      tmatrix();

      /*
       * Constructor from a scalar.
       * \param const T : initial value.
       */
      explicit
      TFEL_MATH_INLINE
      tmatrix(const T);

      /*
       * Constructor from a pointer.
       * \param const T* : initial values.
       */
      explicit
      TFEL_MATH_INLINE
      tmatrix(const T* const);

      /*!
       * \brief index operator (const version).
       * This is a matrix concept requirement.
       * \param const unsigned short, row index.
       * \param const unsigned short, column index.
       * \return const T&, a reference to the tmatrix ith element.
       */
      TFEL_MATH_INLINE
      const T&
      operator()(const unsigned short,const unsigned short) const;
      
      /*!
       * \brief index operator.
       * \param const unsigned short, row index.
       * \param const unsigned short, column index.
       * \return T&, a reference to the tmatrix ith element.
       */
      TFEL_MATH_INLINE
      T&
      operator()(const unsigned short,const unsigned short);
      
      // Assignement operator
      /*
       * \param const MatrixExpr<tmatrix<N,T2>,Expr>&, a matrix expression.
       * \return tmatrix<N,M,T>& a reference to this.
       * \pre T2 must be assignable to a T.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tmatrix<N,M,T>&
      >::type
      operator=(const MatrixExpr<tmatrix<N,M,T2>,Expr>&);

      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tmatrix<N,M,T>&
      >::type
      operator=(const tmatrix<N,M,T2>&);

      // Assignement operator
      /*
       * \param const MatrixExpr<tmatrix<N,T2>,Expr>&, a matrix expression.
       * \return tmatrix<N,M,T>& a reference to this.
       * \pre T2 must be assignable to a T.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tmatrix<N,M,T>&
      >::type
      operator+=(const MatrixExpr<tmatrix<N,M,T2>,Expr>&);

      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tmatrix<N,M,T>&
      >::type
      operator+=(const tmatrix<N,M,T2>&);

      // Assignement operator
      /*
       * \param const MatrixExpr<tmatrix<N,T2>,Expr>&, a matrix expression.
       * \return tmatrix<N,M,T>& a reference to this.
       * \pre T2 must be assignable to a T.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tmatrix<N,M,T>&
      >::type
      operator-=(const MatrixExpr<tmatrix<N,M,T2>,Expr>&);

      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tmatrix<N,M,T>&
      >::type
      operator-=(const tmatrix<N,M,T2>&);

      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	tmatrix<N,M,T>&
      >::type
      operator*=(const T2);

      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	tmatrix<N,M,T>&
      >::type
      operator/=(const T2);

      //! Return the RunTimeProperties of the tmatrix.
      /*
       * This is a MatrixConcept requirement.
       * \return const RunTimeProperties, the runtime properties of
       * the tmatrix
       */
      TFEL_MATH_INLINE
      const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RunTimeProperties();
      }
      
      TFEL_MATH_INLINE2
      unsigned short getNbCols(void) const ;

      TFEL_MATH_INLINE2
      unsigned short getNbRows(void) const ;

      TFEL_MATH_INLINE2
      T max(void) const ;

      TFEL_MATH_INLINE2
      T abs_max(void) const ;

      TFEL_MATH_INLINE2
      void swap_rows(const unsigned short i,const unsigned short j);
      
      static tmatrix<N,N,T> Id(void);

      /*
       * return an iterator to the first element of the matrix
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      TFEL_MATH_INLINE2
      iterator
      begin(void);
      
      /*
       * return an const iterator to the first element of the matrix
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator to the first element
       */
      TFEL_MATH_INLINE2
      const_iterator
      begin(void) const;
      
      /*
       * return an iterator after the last element of the matrix
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      TFEL_MATH_INLINE2
      iterator 
      end(void);
      
      /*
       * return an const iterator after the last element of the matrix
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator after the last element
       */
      TFEL_MATH_INLINE2
      const_iterator
      end(void) const;

      /*
       * return an reverse iterator to the last element of the matrix
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      TFEL_MATH_INLINE2
      reverse_iterator
      rbegin(void);

      /*
       * return an const reverse iterator to the last element of the matrix
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator to the last element
       */
      TFEL_MATH_INLINE2
      const_reverse_iterator
      rbegin(void) const;
      
      /*
       * return an  reverse iterator before the first element of the matrix
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      TFEL_MATH_INLINE2
      reverse_iterator
      rend(void);
      
      /*
       * return an const reverse iterator before the first element of the matrix
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator before the first element
       */
      TFEL_MATH_INLINE2
      const_reverse_iterator
      rend(void) const;

      template<typename InputIterator>
      
      /*
       * copy the N*Mth elements following this argument.
       * \param const InputIterator, an iterator to the first element
       * to be copied.
       */
      TFEL_MATH_INLINE2
      void
      copy(const InputIterator);

      void
      invert();

    };

    // Transpose
    template<unsigned short N,unsigned short M,typename T>
    TFEL_MATH_INLINE2
    tmatrix<M,N,T> transpose(const tmatrix<N,M,T>&);

    // Serialisation operator
    template<unsigned short N,unsigned short M,typename T>
    std::ostream & operator << (std::ostream &, const tmatrix<N,M,T>&);

  } // end of namespace math

  namespace typetraits{
    
    template<unsigned short N,unsigned short M,typename T2,typename T>
    struct IsAssignableTo<tfel::math::tmatrix<N,M,T2>,
			  tfel::math::tmatrix<N,M,T> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };

  } // end of namespace typetraits

} // end of namespace tfel

#include "Math/Matrix/tmatrix.ixx"
#include "Math/Matrix/tmatrixResultType.hxx"

#endif /* _TFEL_MATH_TINY_MATRIX_LIB_ */
