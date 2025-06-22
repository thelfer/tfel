/*!
 * \file   FCString.hxx
 * \brief This header declares the `FCString` class which provides a
 * description of a string with fixed capacity.
 * \author Thomas Helfer
 * \date   23 janv. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_FCSTRING_HXX
#define LIB_TFEL_UTILITIES_FCSTRING_HXX

#include <array>
#include <string>
#include <ostream>

namespace tfel {

  namespace utilities {

    /*!
     * \brief provides a view of a C-String which is not necessarily
     * null-terminalted
     */
    template <std::size_t N, typename CharT>
    struct CStringNarrowedView {
      const CharT* const value;
    };  // end of struct CStringNarrowedView

    /*!
     * \brief an helper function to build a `CStringNarrowedView` object;
     * \param[in] s: C-string
     * \return a restricted view of the input
     */
    template <std::size_t N, typename CharT>
    CStringNarrowedView<N, CharT> narrow(const CharT*);

    /*!
     * \brief This class implements a string with fixed capacity.
     * In pratice, a simple wrapper around std::array<CharT> providing an
     * API inspired by `std::basic_string`.
     * \tparam CharT:  Type of character
     * \tparam Traits: Traits for character type, defaults to
     *                 std::char_traits<CharT>.
     */
    template <std::size_t N, typename CharT, typename Traits>
    struct basic_fcstring : public std::array<CharT, N + 1> {
      //! default constructor
      basic_fcstring();
      /*!
       * constructor from `std::string`
       * \param[in] src: source
       */
      template <typename Alloc>
      basic_fcstring(const std::basic_string<CharT, Traits, Alloc>&);
      /*!
       * constructor from C-string
       * \param[in] src: source
       */
      basic_fcstring(const CharT*);
      /*!
       * constructor from C-string
       * \param[in] src: source
       * \param[in] b:   restrict the source
       */
      basic_fcstring(const CharT*, const bool);
      /*!
       * \brief constructor from a restricted view of a
       * `C-string`. This allows build a basic_fcstring from a static
       * array of characters.
       * \param[in] src: source
       */
      template <std::size_t N2>
      basic_fcstring(const CStringNarrowedView<N2, CharT>&);
      //! move constructor
      basic_fcstring(basic_fcstring&&);
      //! copy constructor
      basic_fcstring(const basic_fcstring&);
      //! move constructor
      basic_fcstring& operator=(basic_fcstring&&);
      //! copy constructor
      basic_fcstring& operator=(const basic_fcstring&);
      /*!
       * constructor from `std::string`
       * \param[in] rhs: source
       * \return this object
       */
      template <typename Alloc>
      basic_fcstring& operator=(const std::basic_string<CharT, Traits, Alloc>&);
      /*!
       * \brief constructor from a restricted view of a
       * `C-string`. This allows build a basic_fcstring from a static
       * array of characters.
       * \param[in] rhs: source
       */
      template <std::size_t N2>
      basic_fcstring& operator=(const CStringNarrowedView<N2, CharT>&);
      /*!
       * \brief access opeartor
       * \param[in] i: index
       * \return the element at the given position
       */
      CharT& operator[](const std::size_t);
      /*!
       * \brief access opeartor
       * \param[in] i: index
       * \return the element at the given position
       */
      const CharT& operator[](const std::size_t) const;
      /*!
       * \param[in] i: index
       * \return the element at the given position
       */
      CharT& at(const std::size_t);
      /*!
       * \param[in] i: index
       * \return the element at the given position
       */
      const CharT& at(const std::size_t) const;
      /*!
       * constructor from C-string
       * \param[in] rhs: source
       * \return this object
       */
      basic_fcstring& operator=(const CharT*);
      //! \brief implicit conversion operator to `std::string`
      explicit operator std::basic_string<CharT, Traits>() const;
      //! \return true if the first character is '\0'
      bool empty() const;
      //! \return the actual size of the string
      std::size_t size() const;
      //! \return an iterator past-the end of the string
      typename std::array<CharT, N + 1>::iterator end();
      //! \return an iterator past-the end of the string
      typename std::array<CharT, N + 1>::const_iterator end() const;
      //! \return an iterator past-the end of the string
      typename std::array<CharT, N + 1>::const_iterator cend() const;
      //! \return an reverse iterator at the end of the string
      typename std::array<CharT, N + 1>::reverse_iterator rbegin();
      //! \return an reverse iterator at the end of the string
      typename std::array<CharT, N + 1>::const_reverse_iterator rbegin() const;
      //! \return an reverse iterator at the end of the string
      typename std::array<CharT, N + 1>::const_reverse_iterator crbegin() const;
      //! \return the last character of the string
      CharT& back();
      //! \return the last character of the string
      const CharT& back() const;
      //! \brief remove last character of the string
      void pop_back();
      /*!
       * \brief comparison operator
       * \param[in] s: string
       * \return true if the two string are equals
       */
      bool strcmp(const CharT*) const;
      /*!
       * \brief comparison operator
       * \param[in] s: string
       * \param[in] n: size of the string
       * \return true if the two string are equals
       */
      bool strcmp(const CharT*,
                  const typename std::array<CharT, N + 1>::size_type) const;
      /*!
       * \return the size of an external C-string if a null character
       * is found in the N first characters, N+1 otherwise.
       * \param[in] s: external C-string
       */
      static std::size_t strnlen(const CharT*);
    };  // end of struct basic_fcstring

    //! a simple alias for `char`
    template <std::size_t N>
    using fcstring = basic_fcstring<N, char, std::char_traits<char>>;

    /*!
     * \brief output operator
     * \param[out] os: output stream
     * \param[in]  s:  fixed sized string
     */
    template <std::size_t N, typename CharT, typename Traits>
    std::ostream& operator<<(std::ostream&,
                             const basic_fcstring<N, CharT, Traits>&);
    /*!
     * \brief output operator
     * \param[out] os: output stream
     * \param[out] s:  fixed sized string
     */
    template <std::size_t N, typename CharT, typename Traits>
    std::istream& operator>>(std::istream&, basic_fcstring<N, CharT, Traits>&);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, std::size_t N2, typename CharT, typename Traits>
    bool operator==(const basic_fcstring<N, CharT, Traits>&,
                    const basic_fcstring<N2, CharT, Traits>&);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, typename CharT, typename Traits, typename Alloc>
    bool operator==(const basic_fcstring<N, CharT, Traits>&,
                    const std::basic_string<CharT, Traits, Alloc>&);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, typename CharT, typename Traits, typename Alloc>
    bool operator==(const std::basic_string<CharT, Traits, Alloc>&,
                    const basic_fcstring<N, CharT, Traits>&);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, typename CharT, typename Traits>
    bool operator==(const basic_fcstring<N, CharT, Traits>&, const CharT*);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, typename CharT, typename Traits>
    bool operator==(const CharT*, const basic_fcstring<N, CharT, Traits>&);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, std::size_t N2, typename CharT, typename Traits>
    bool operator!=(const basic_fcstring<N, CharT, Traits>&,
                    const basic_fcstring<N2, CharT, Traits>&);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, typename CharT, typename Traits, typename Alloc>
    bool operator!=(const basic_fcstring<N, CharT, Traits>&,
                    const std::basic_string<CharT, Traits, Alloc>&);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, typename CharT, typename Traits, typename Alloc>
    bool operator!=(const std::basic_string<CharT, Traits, Alloc>&,
                    const basic_fcstring<N, CharT, Traits>&);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, typename CharT, typename Traits>
    bool operator!=(const basic_fcstring<N, CharT, Traits>&, const CharT*);
    /*!
     * \brief comparison operator
     * \param[in] lhs: left hand side
     * \param[in] rhs: left hand side
     */
    template <std::size_t N, typename CharT, typename Traits>
    bool operator!=(const CharT*, const basic_fcstring<N, CharT, Traits>&);

  }  // end of namespace utilities

}  // end of namespace tfel

#include "TFEL/Utilities/FCString.ixx"

#endif /* LIB_TFEL_UTILITIES_FCSTRING_HXX */
