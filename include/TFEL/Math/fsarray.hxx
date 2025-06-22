/*!
 * \file  include/TFEL/Math/fsarray.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 30 août 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FSARRAY_HXX
#define LIB_TFEL_MATH_FSARRAY_HXX

#include <iterator>
#include <type_traits>
#include <initializer_list>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"

namespace tfel {

  namespace math {

    /*!
     * an aggregate type
     * \tparam N : size of the array
     * \tparam T : value type
     */
    template <unsigned short N, typename T>
    struct array_container {
      //! values holded
      T v[N];
    };

    /*!
     * \brief a container for finite size array.
     * This is mainly equivalent to the array class of C++11.
     * \tparam N : size of the array
     * \tparam T : value type
     * \note we could not get rid of this class of favor of std::array
     * because two many classes who inherits from tfel::math::fsarry
     * used direct access to the values through the `v` member.
     */
    template <unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL fsarray : protected array_container<N, T> {
      /*!
       * type of the fsarray's values.
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
      typedef unsigned short size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      typedef ptrdiff_t difference_type;
      /*!
       * type of the fsarray's iterator.
       * (provided for stl compatibility).
       */
      typedef pointer iterator;
      /*!
       * type of the fsarray's const iterator.
       * (provided for stl compatibility).
       */
      typedef const_pointer const_iterator;
      /*!
       * type of the fsarray's reverse iterator.
       * (provided for stl compatibility).
       */
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      /*!
       * type of the fsarray's const reverse iterator.
       * (provided for stl compatibility).
       */
      typedef std::reverse_iterator<iterator> reverse_iterator;
      /*!
       * default constructor
       */
      TFEL_MATH_INLINE fsarray() noexcept;
      /*!
       * constructor
       * \param[in] values
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr fsarray(const fsarray<N, T2>&);
      /*!
       * constructor
       * \param[in] values
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr fsarray(const std::initializer_list<T2>&);
      /*!
       * constructor from a single value
       * \param[in] value
       */
      template <
          typename T2,
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  bool>::type = true>
      TFEL_MATH_INLINE constexpr fsarray(const T2&);
      /*!
       * \brief member function for structured bindings support
       * \tparam I: index to be retrieved
       */
      template <std::size_t I>
      constexpr const_reference get() const;
      /*!
       * \return an pointer to the first element of the vector
       * (provided for stl compatibility)
       */
      TFEL_MATH_INLINE pointer data() noexcept;
      /*!
       * \return an pointer to the first element of the vector
       * (provided for stl compatibility)
       */
      TFEL_MATH_INLINE const_pointer data() const noexcept;
      /*!
       * \brief return an iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      TFEL_MATH_INLINE iterator begin() noexcept;
      /*!
       * \brief return an const iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator to the first element
       */
      TFEL_MATH_INLINE const_iterator begin() const noexcept;
      /*!
       * \brief return an const iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator to the first element
       */
      TFEL_MATH_INLINE const_iterator cbegin() const noexcept;
      /*!
       * \brief return an iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      TFEL_MATH_INLINE iterator end() noexcept;
      /*!
       * \brief return an const iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator after the last element
       */
      TFEL_MATH_INLINE const_iterator end() const noexcept;
      /*!
       * \brief return an const iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator after the last element
       */
      TFEL_MATH_INLINE const_iterator cend() const noexcept;
      /*!
       * \brief return an reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      TFEL_MATH_INLINE reverse_iterator rbegin() noexcept;
      /*!
       * \brief return an const reverse iterator to the last element of the
       * vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator to the last
       * element
       */
      TFEL_MATH_INLINE const_reverse_iterator rbegin() const noexcept;
      /*!
       * \brief return an  reverse iterator before the first element of the
       * vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      TFEL_MATH_INLINE reverse_iterator rend() noexcept;
      /*!
       * \brief return an const reverse iterator before the first element of the
       * vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator before the
       * first element
       */
      TFEL_MATH_INLINE const_reverse_iterator rend() const noexcept;
      /*!
       * size of the finite size array
       * (compatibility with array)
       */
      constexpr TFEL_MATH_INLINE unsigned short size() const noexcept;
      /*!
       * access operator
       */
      TFEL_MATH_INLINE reference operator[](const size_type) noexcept;
      /*!
       * access operator
       */
      constexpr TFEL_MATH_INLINE const_reference
      operator[](const size_type) const noexcept;

     private:
      //! a simple assertion stating that the dimension is valid.
      TFEL_STATIC_ASSERT(N != 0);
    };

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/General/fsarray.ixx"

#endif /* LIB_TFEL_MATH_FSARRAY_HXX */
