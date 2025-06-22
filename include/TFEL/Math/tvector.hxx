/*!
 * \file   include/TFEL/Math/tvector.hxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TINY_VECTOR_
#define LIB_TFEL_TINY_VECTOR_ 1

#include <cstddef>
#include <iterator>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/RealPartType.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Vector/VectorConceptOperations.hxx"
#include "TFEL/Math/Vector/TinyVectorFromTinyVectorView.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Forward/tvector.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief Partial specialisation for tvectors.
     * \note This is a VectorConcept requirement.
     * \see VectorTraits.
     */
    template <unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL VectorTraits<tvector<N, T>> {
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
    template <typename T, typename T2, typename Op>
    struct IsTVectorScalarOperationValid {
      static constexpr bool cond =
          tfel::typetraits::IsScalar<T2>::cond &&
          std::is_same<typename ResultType<T, T2, OpMult>::type, T>::value;
    };  // end of struct IsTVectorScalarOperationValid

    /*!
     * \brief a base for tvector or classes acting like tvector.
     */
    template <typename Child, unsigned short N, typename T>
    struct tvector_base {
      //! Assignement operator
      /*!
       * \param src: a vector expression.
       * \return a reference to this.
       * \pre T2 must be assignable to a T.
       */
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  Child&>::type
          operator=(const Expr<tvector<N, T2>, Operation>&);

      // Assignement operator
      template <typename T2>
      TFEL_MATH_INLINE
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  Child&>::type
          operator=(const tvector<N, T2>&);

      // Assignement operator
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  Child&>::type
          operator+=(const Expr<tvector<N, T2>, Operation>&);

      // Assignement operator
      template <typename T2>
      TFEL_MATH_INLINE
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  Child&>::type
          operator+=(const tvector<N, T2>&);

      // Assignement operator
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  Child&>::type
          operator-=(const Expr<tvector<N, T2>, Operation>&);

      // Assignement operator
      template <typename T2>
      TFEL_MATH_INLINE
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  Child&>::type
          operator-=(const tvector<N, T2>&);

      /*!
       * operator*=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          IsTVectorScalarOperationValid<T, T2, OpMult>::cond,
          Child&>::type
      operator*=(const T2);

      /*!
       * operator/=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          IsTVectorScalarOperationValid<T, T2, OpDiv>::cond,
          Child&>::type
      operator/=(const T2);
    };

    template <unsigned short N, typename T = double>
    struct TFEL_VISIBILITY_LOCAL tvector
        : public VectorConcept<tvector<N, T>>,
          public tvector_base<tvector<N, T>, N, T>,
          public fsarray<N, T> {
      //! a simple typedef to the tvector runtime properties
      /*!
       * This is a VectorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! \brief default constructor.
      TFEL_MATH_INLINE explicit constexpr tvector();
      //! copy constructor
      TFEL_MATH_INLINE constexpr tvector(const tvector&) = default;
      /*!
       * \brief Default Constructor
       * \param[in] init: value used to initialise the components of the vector
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr explicit tvector(const T2&);
      /*!
       * \brief Default Constructor
       * \param[in] init: values used to initialise the components of the vector
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr tvector(const std::initializer_list<T2>&);
      /*!
       * Constructor from a pointer.
       * \param const T* : initial values.
       */
      TFEL_MATH_INLINE explicit tvector(const T* const);
      //! Assignement operator
      /*!
       * \param src: a vector expression.
       * \return tvector<N,T>& a reference to this.
       * \pre T2 must be assignable to a T.
       */
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE tvector(const Expr<tvector<N, T2>, Operation>&);
      //! assignement operator
      tvector& operator=(const tvector&) = default;
      //! using tvector_base::operator=
      using tvector_base<tvector, N, T>::operator=;
      /*!
       * \brief index operator.
       * This is a vector concept requirement.
       * \param const unsigned short, index.
       * \return const T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE constexpr const T& operator()(
          const unsigned short) const noexcept;
      /*!
       * \brief index operator.
       * This is a vector concept requirement.
       * \param const unsigned short, index.
       * \return const T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE T& operator()(const unsigned short) noexcept;
      // using fsarray assignement operator
      using fsarray<N, T>::operator[];
      /*!
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      TFEL_MATH_INLINE constexpr RunTimeProperties getRunTimeProperties()
          const noexcept;
      /*!
       * copy the Nth elements following this argument.
       * \param const InputIterator, an iterator to the first element
       * to be copied.
       */
      template <typename InputIterator>
      TFEL_MATH_INLINE void copy(const InputIterator src);
      /*!
       * \brief create a slice
       * \param[in] I : the starting index
       */
      template <unsigned short I>
      Expr<tvector<N - I, T>,
           TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, false>>
      slice();
      /*!
       * \brief create a slice
       * \param[in] I : the starting index
       * \param[in] J : the size of the slice
       * \note : the slice object contains a reference to the source
       * vector, so this vector shall not be destroyed before the slice
       */
      template <unsigned short I, unsigned short J>
      Expr<tvector<J - I, T>,
           TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, false>>
      slice();
      /*!
       * \brief create a slice (const version)
       * \param[in] I : the starting index
       * \note : the slice object contains a reference to the source
       * vector, so this vector shall not be destroyed before the slice
       */
      template <unsigned short I>
      Expr<tvector<N - I, T>,
           TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, true>>
      slice() const;
      /*!
       * \brief create a slice (const version)
       * \param[in] I : the starting index
       * \param[in] J : the size of the slice
       * \note : the slice object contains a reference to the source
       * vector, so this vector shall not be destroyed before the slice
       */
      template <unsigned short I, unsigned short J>
      Expr<tvector<J - I, T>,
           TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, true>>
      slice() const;

     private:
      //! a simple assertion stating that the dimension is valid.
      TFEL_STATIC_ASSERT(N != 0);
    };
    /*!
     * \brief create a new tvector by applying a functor
     * \param[in] f: functor
     * \param[in] x: inital value
     */
    template <typename F, typename T, unsigned short N>
    tvector<N, typename std::result_of<F(T)>::type> map(F,
                                                        const tvector<N, T>&);
    /*!
     * export the given vector to an array of the
     */
    template <unsigned short N, typename T, typename OutputIterator>
    TFEL_MATH_INLINE2
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, void>::type
        exportToBaseTypeArray(const tvector<N, T>&, OutputIterator);

    template <unsigned short N, typename T>
    TFEL_MATH_INLINE2 typename tfel::typetraits::AbsType<T>::type abs(
        const tvector<N, T>& v);

    template <typename T>
    tvector<1u, T> makeTVector1D(const T);

    template <typename T>
    tvector<2u, T> makeTVector2D(const T, const T);

    template <typename T>
    tvector<3u, T> makeTVector3D(const T, const T, const T);

    template <typename T>
    tvector<3u, T> cross_product(const tvector<2u, T>&, const tvector<2u, T>&);

    template <typename T>
    tvector<3u, T> cross_product(const tvector<3u, T>&, const tvector<3u, T>&);

    /*!
     * find a vector perpendicular to the second one
     */
    template <typename T>
    void find_perpendicular_vector(tvector<3u, T>&, const tvector<3u, T>&);

    /*!
     * \brief create a slice from a tiny vector
     * \param[in] v : vector
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I, unsigned short N, typename T>
    Expr<tvector<N - I, T>,
         TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, false>>
    slice(tvector<N, T>&);
    /*!
     * \brief create a slice from a tiny vector
     * \param[in] v : vector
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I, unsigned short J, unsigned short N, typename T>
    Expr<tvector<J - I, T>,
         TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, false>>
    slice(tvector<N, T>&);
    /*!
     * \brief create a slice from a tiny vector
     * \param[in] v : vector
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I, unsigned short N, typename T>
    Expr<tvector<N - I, T>,
         TinyVectorFromTinyVectorViewExpr<N - I, N, I, T, true>>
    slice(const tvector<N, T>&);
    /*!
     * \brief create a slice from a tiny vector (const version)
     * \param[in] v : vector
     * \note : the slice object contains a reference to the source
     * vector, so this vector shall not be destroyed before the slice
     */
    template <unsigned short I, unsigned short J, unsigned short N, typename T>
    Expr<tvector<J - I, T>,
         TinyVectorFromTinyVectorViewExpr<J - I, N, I, T, true>>
    slice(const tvector<N, T>&);

  }  // end of namespace math

  namespace typetraits {

    /*!
     * Partial specialisation for tvectors
     */
    template <unsigned short N, typename T2, typename T>
    struct IsAssignableTo<tfel::math::tvector<N, T2>,
                          tfel::math::tvector<N, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = IsAssignableTo<T2, T>::cond;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#include "TFEL/Math/Vector/tvector.ixx"
#include "TFEL/Math/Vector/tvectorResultType.hxx"

#endif /* LIB_TFEL_TINY_VECTOR_ */
