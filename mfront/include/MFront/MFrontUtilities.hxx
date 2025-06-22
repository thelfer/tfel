/*!
 * \file   MFrontUtilities.hxx
 * \brief This header declares a few helper function to write
 * information in the mfront storage format
 * \author THOMAS HELFER
 * \date   15 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTUTILITIES_HXX_
#define LIB_MFRONT_MFRONTUTILITIES_HXX_

#include <string>
#include <vector>
#include <iosfwd>
#include <initializer_list>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief conditionnally insert a string into
   * a vector of string : an element is inserted if not already
   * present or empty.
   * \param[out] d : destination
   * \param[in]  v : value
   */
  MFRONT_VISIBILITY_EXPORT void insert_if(std::vector<std::string>&,
                                          const std::string&);

  /*!
   * \brief conditionnally insert a string into
   * a vector of string : an element is inserted if not already
   * present or has zero size or is nulltpr.
   * \param[out] d : destination
   * \param[in]  v : value
   */
  MFRONT_VISIBILITY_EXPORT void insert_if(std::vector<std::string>&,
                                          const char* const);

  /*!
   * \brief call insert_if on every element of a
   * container.
   * \pre Container::value_type must be convertible to a string
   * \tparam Container : type of source
   * \param[out] d : destination
   * \param[in]  s : source
   */
  template <typename Container>
  void insert_if(std::vector<std::string>&, const Container&);

  /*!
   * \brief call insert_if on every element of an
   * initializer_list.
   * \pre Value must be convertible to a string
   * \tparam Value : type of the objects of the initializer list
   * \param[out] d : destination
   * \param[in]  s : source
   */
  template <typename Value>
  void insert_if(std::vector<std::string>&,
                 const std::initializer_list<Value>&);
  /*!
   * \brief write a vector of string to the output stream
   * \param[in] os : output stream
   * \param[in] v  : vector of string
   * \param[in] id : identifier
   */
  MFRONT_VISIBILITY_EXPORT void write(std::ostream&,
                                      const std::vector<std::string>&,
                                      const std::string&);
  /*!
   * \brief read an object of type T from a stream created by the
   * CxxTokenizer class
   * \tparam T         : type to be read
   * \param[in,out] p  : current position in the stream
   * \param[in] pe     : end of the stream
   * \return the object read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p shall be unchanged.
   */
  template <typename T>
  T read(tfel::utilities::CxxTokenizer::const_iterator&,
         const tfel::utilities::CxxTokenizer::const_iterator);
  /*!
   * \brief read an object of type T from a stream created by the
   * CxxTokenizer class
   * \tparam T         : type to be read
   * \tparam v         : value read
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the object read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p shall be unchanged.
   */
  template <typename T>
  void read(T&,
            tfel::utilities::CxxTokenizer::const_iterator&,
            const tfel::utilities::CxxTokenizer::const_iterator);
  /*!
   * \brief read a string from a stream created by the CxxTokenizer
   * class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the string read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT double read(
      tfel::utilities::CxxTokenizer::const_iterator&,
      const tfel::utilities::CxxTokenizer::const_iterator);
  /*!
   * \brief read a string from a stream created by the CxxTokenizer
   * class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the string read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT std::string read(
      tfel::utilities::CxxTokenizer::const_iterator&,
      const tfel::utilities::CxxTokenizer::const_iterator);
  /*!
   * \brief read a vector of strings from a stream created by the
   * CxxTokenizer class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the vector read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> read(
      tfel::utilities::CxxTokenizer::const_iterator&,
      const tfel::utilities::CxxTokenizer::const_iterator);

}  // namespace mfront

#include "MFront/MFrontUtilities.ixx"

#endif /* LIB_MFRONT_MFRONTUTILITIES_HXX_ */
