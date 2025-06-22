/*!
 * \file  include/TFEL/System/RecursiveFind.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_RECURSIVEFIND_HXX
#define LIB_TFEL_SYSTEM_RECURSIVEFIND_HXX

#include <map>
#include <regex>
#include <vector>
#include <string>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace system {

    /*!
     * \brief recursively search files matching a given regular
     * expression from the given directory.
     * \param re     : a regular expression
     * \param name   : name of the directory from which the search starts
     * \param b      : throw an exception if something wrong happens
     * \param depth  : depth reached in the recursive search
     * \param mdepth : maximum depth allowed
     * \return the matching files, ordered by directories
     */
    TFELSYSTEM_VISIBILITY_EXPORT
    std::map<std::string, std::vector<std::string>> recursiveFind(
        const std::string&,
        const std::string&,
        const bool b = true,
        const unsigned short = 0u,
        const unsigned short = 100u);

    /*!
     * \brief recursively search files matching a given regular
     * expression from the given directory.
     * \param[out] r     : the matching files, ordered by directories
     * \param[in] re     : a regular expression
     * \param[in] b      : throw an exception if something wrong happens.
     * \param[in] name   : name of the directory from which the search starts
     * \param[in] depth  : depth reached in the recursive search
     * \param[in] mdepth : maximum depth allowed
     */
    TFELSYSTEM_VISIBILITY_EXPORT void recursiveFind(
        std::map<std::string, std::vector<std::string>>&,
        const std::string&,
        const std::string&,
        const bool = true,
        const unsigned short = 0u,
        const unsigned short = 100u);

    /*!
     * \brief recursively search files matching a given regular
     * expression from the given directory.
     * \param[in] re     : a regular expression
     * \param[in] name   : name of the directory from which the search starts
     * \param[in] b      : throw an exception if something wrong happens.
     * \param[in] depth  : depth reached in the recursive search
     * \param[in] mdepth : maximum depth allowed
     * \return the matching files, ordered by directories
     */
    TFELSYSTEM_VISIBILITY_EXPORT
    std::map<std::string, std::vector<std::string>> recursiveFind(
        const std::regex&,
        const std::string&,
        const bool = true,
        const unsigned short = 0u,
        const unsigned short = 100u);

    /*!
     * \brief recursively search files matching a given regular
     * expression from the given directory.
     * \param[out] r      : the matching files, ordered by directories
     * \param[in]  re     : a regular expression
     * \param[in]  b      : throw an exception if something wrong happens.
     * \param[in]  name   : name of the directory from which the search starts
     * \param[in]  depth  : depth reached in the recursive search
     * \param[in]  mdepth : maximum depth allowed
     */
    TFELSYSTEM_VISIBILITY_EXPORT void recursiveFind(
        std::map<std::string, std::vector<std::string>>&,
        const std::regex&,
        const std::string&,
        const bool = true,
        const unsigned short = 0u,
        const unsigned short = 100u);

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_SYSTEM_RECURSIVEFIND_HXX */
