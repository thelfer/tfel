/*!
 * \file  include/TFEL/Utilities/StringAlgorithms.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mar 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_STRINGALGORITHMS_HXX
#define LIB_TFEL_UTILITIES_STRINGALGORITHMS_HXX

#include <vector>
#include <string>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace utilities {

    /*!
     * \param[in] s1: first string
     * \param[in] s2: second string
     * \return true if the first string starts with the second
     */
    TFELUTILITIES_VISIBILITY_EXPORT
    bool starts_with(const std::string&, const std::string&);
    /*!
     * \param[in] s1: first string
     * \param[in] s2: second string
     * \return true if the first string ends with the second
     */
    TFELUTILITIES_VISIBILITY_EXPORT
    bool ends_with(const std::string&, const std::string&);
    /*!
     * split a string into tokens using the specified delimiter
     * \param s : string to be splitted
     * \param c : delimiter
     * \return the tokens
     */
    TFELUTILITIES_VISIBILITY_EXPORT
    std::vector<std::string> tokenize(const std::string&, const char);

    /*!
     * split a string into tokens using the specified delimiter
     * \param s     : string to be splitted
     * \param delim : delimiter
     * \return the tokens
     */
    TFELUTILITIES_VISIBILITY_EXPORT
    std::vector<std::string> tokenize(const std::string&, const std::string&);

    /*!
     * \brief replace all occurences of substring s1 by s2
     * \param s  : original string
     * \param s1 : substring to be changed
     * \param s2 : replacement string
     * \param ps : beginning of the search
     * \return a copy of s where all occurences of substring s1 have
     * been replaced by s2
     */
    TFELUTILITIES_VISIBILITY_EXPORT std::string replace_all(
        const std::string&,
        const std::string&,
        const std::string&,
        const std::string::size_type = 0u);

    /*!
     * \brief replace all occurences of substring s1 by s2
     * \param r  : result
     * \param s  : original string
     * \param s1 : substring to be changed
     * \param s2 : replacement string
     * \param ps : beginning of the search
     */
    TFELUTILITIES_VISIBILITY_EXPORT void replace_all(
        std::string&,
        const std::string&,
        const std::string&,
        const std::string&,
        const std::string::size_type = 0u);

    /*!
     * \brief replace all occurences of character c1 by c2
     * \param c  : original string
     * \param c1 : character to be changed
     * \param c2 : replacement character
     * \return a copy of c where all occurences of c1 have
     * been replaced by c2
     */
    TFELUTILITIES_VISIBILITY_EXPORT std::string replace_all(const std::string&,
                                                            const char,
                                                            const char);

    /*!
     * \brief replace all occurences of character c by n
     * \param[in,out] s : original string
     * \param[in]     c : character to be changed
     * \param[in]     n : replacement character
     * \return a copy of c where all occurences of c have
     * been replaced by n
     */
    TFELUTILITIES_VISIBILITY_EXPORT void replace_all(std::string&,
                                                     const char,
                                                     const std::string&);
    /*!
     * \brief convert a string to the given type
     * \param[in] s: string to be converted
     */
    template <typename T>
    T convert(const std::string&);
    /*!
     * \brief partial specialisation for double
     * \param[in] s: string to be converted
     */
    template <>
    TFELUTILITIES_VISIBILITY_EXPORT double convert<>(const std::string&);
    /*!
     * \brief partial specialisation for double
     * \param[in] s: string to be converted
     */
    template <>
    TFELUTILITIES_VISIBILITY_EXPORT long double convert<>(const std::string&);

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_STRINGALGORITHMS_HXX */
