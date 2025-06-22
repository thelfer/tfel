/*!
 * \file   MFrontUtilities.hxx
 * \brief This header declares a few helper function to write
 * information in the mfront storage format
 * \author Thomas Helfer
 * \date   15 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTUTILITIES_HXX
#define LIB_MFRONT_MFRONTUTILITIES_HXX

#include <map>
#include <tuple>
#include <string>
#include <vector>
#include <iosfwd>
#include <initializer_list>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declaration
  struct VariableBoundsDescription;

  /*!
   * \return the keys of a map
   * \param[in] m: map
   */
  template <typename Value>
  std::vector<std::string> getKeys(const std::map<std::string, Value>&);

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
  /*!
   * \brief read the bounds associated to a variable
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return a tuple giving the variable name and the bounds
   */
  MFRONT_VISIBILITY_EXPORT
  std::pair<std::string, VariableBoundsDescription> readVariableBounds(
      tfel::utilities::CxxTokenizer::const_iterator& p,
      const tfel::utilities::CxxTokenizer::const_iterator pe);
  /*!
   * \brief extract the name of a variable and an array position from a string.
   * \param[in] n: variable name and array position
   * \return a tuple giving the variable name, the fact that a
   * position was read, the read position if any.
   *
   * For example, calling this function with the `A[10]` argument
   * results in the following tuple: `{"A",true,10}`.
   *
   * With `B` as argument, the result is the following tuple:
   * `{"B",false,0}`.
   */
  MFRONT_VISIBILITY_EXPORT
  std::tuple<std::string, bool, unsigned short>
  extractVariableNameAndArrayPosition(const std::string&);

}  // namespace mfront

#include "MFront/MFrontUtilities.ixx"

#endif /* LIB_MFRONT_MFRONTUTILITIES_HXX */
