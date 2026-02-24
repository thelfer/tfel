/*!
 * \file   MFront/DSLUtilities.hxx
 * \brief  This header declares a set of functions to read
 * information from an mfront file
 * \author Thomas Helfer
 * \date   23/04/2025
 */

#ifndef LIB_MFRONT_DSLUTILITIES_HXX
#define LIB_MFRONT_DSLUTILITIES_HXX

#include <string>
#include <string_view>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableBoundsDescription.hxx"

namespace mfront {

  /*!
   * \return true if the next tokens are '<safe>', false otherwise
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   */
  MFRONT_VISIBILITY_EXPORT bool readSafeOptionTypeIfPresent(
      tfel::utilities::CxxTokenizer::const_iterator&,
      const tfel::utilities::CxxTokenizer::const_iterator);
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
   * \brief read a data map from a stream created by the
   * CxxTokenizer class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the DataMap read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT tfel::utilities::DataMap read(
      tfel::utilities::CxxTokenizer::const_iterator&,
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
   * \brief read the bounds associated to a variable
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return a tuple giving the variable name and the bounds
   */
  MFRONT_VISIBILITY_EXPORT
  std::pair<std::string, VariableBoundsDescription> readVariableBounds(
      tfel::utilities::CxxTokenizer::const_iterator& p,
      const tfel::utilities::CxxTokenizer::const_iterator pe);
}  // namespace mfront

#include "MFront/DSLUtilities.ixx"

#endif /* LIB_MFRONT_DSLUTILITIES_HXX */
