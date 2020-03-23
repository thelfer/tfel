/*!
 * \file   include/TFEL/Utilities/Span.hxx
 * \brief
 * \author Thomas Helfer
 * \date   22/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_SPAN_HXX
#define LIB_TFEL_UTILITIES_SPAN_HXX

#include <type_traits>

namespace tfel {

  namespace utilities {

    /*!
     * \brief a simple implementation of a `std::span`-like structure (not
     * available in C++-11)
     */
    template <typename T, bool const_view = false>
    struct Span {
      //! a simple alias
      using value_type =
          typename std::conditional<const_view, const T, T>::type;
      //! a simple alias
      using size_type = size_t;
      //! a simple alias
      using iterator = value_type*;
      //! a simple alias
      using const_iterator = const value_type*;
      //! \brief default constructor
      Span() noexcept;
      //! \brief constructor
      template <typename Container>
      Span(Container&) noexcept;
      //! \brief constructor
      template <size_t N>
      Span(value_type[N]) noexcept;
      //! \brief constructor
      Span(value_type* const, const size_type) noexcept;
      //! \brief move constructor
      Span(Span&&) noexcept;
      //! \brief copy constructor
      Span(const Span&) noexcept;
      //! \brief move assignement
      Span& operator=(Span&&) noexcept;
      //! \brief copy assignement
      Span& operator=(const Span&) noexcept;
      /*!
       * \return the pointer to the first element or nullptr is the
       * sequence is empty
       */
      iterator begin() noexcept;
      /*!
       * \return a pointer after to last element or nullptr is the
       * sequence is empty
       */
      iterator end() noexcept;
      /*!
       * \return the pointer to the first element or nullptr is the
       * sequence is empty
       */
      const_iterator begin() const noexcept;
      /*!
       * \return a pointer after to last element or nullptr is the
       * sequence is empty
       */
      const_iterator end() const noexcept;
      //! \return the size of the buffer
      size_type size() const noexcept;
      //! \return true if the buffer has zero size
      bool empty() const noexcept;
      /*!
       * \return the ith element
       * \param[in] i: rank
       */
      value_type& operator[](const size_type) noexcept;
      /*!
       * \return the ith element
       * \param[in] i: rank
       */
      value_type& operator()(const size_type) noexcept;
      /*!
       * \return the ith element
       * \param[in] i: rank
       */
      const value_type& operator[](const size_type) const noexcept;
      /*!
       * \return the ith element
       * \param[in] i: rank
       */
      const value_type& operator()(const size_type) const noexcept;

     private:
      //! pointer to the beginning of the buffer
      value_type* b;
      //! size of the memory buffer
      size_type s;
    };  // end of Span

    //! \brief a simple alias
    template <typename T>
    using ConstSpan = Span<T, true>;

  }  // end of namespace utilities

}  // end of namespace tfel

#include "TFEL/Utilities/Span.ixx"

#endif /* LIB_TFEL_UTILITIES_SPAN_HXX */
