/*!
 * \file  mfront/include/MFront/SearchPathsHandler.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 21 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTSEARCHPATHSHANDLER_HXX
#define LIB_MFRONT_MFRONTSEARCHPATHSHANDLER_HXX

#include <set>
#include <string>
#include <vector>

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * structure in charge of:
   * - storing search paths
   * - searching imported mfront file
   */
  struct MFRONT_VISIBILITY_EXPORT SearchPathsHandler {
    /*!
     * search a file in the specified paths.
     * The file is searched using
     * the following conventions :
     * - search in the current directory.
     * - search in the search paths from the most recently declared
         search path to the older ones.
     *
     * Search paths declared through the MFRONT_INCLUDE_PATH are
     * stored at the end of the list.
     *
     * \param[in] f : file name
     * \return the full path of the file
     */
    static std::string search(const std::string&);
    /*!
     * \brief Add new search paths.
     *
     * Multiple paths are separated by commas under unices systems and
     * by semicolons under Windows systems.
     * \param[in] p : new search paths
     */
    static void addSearchPaths(const std::string&);
    /*!
     * \return the list of the search paths
     */
    static const std::vector<std::string>& getSearchPaths();

   private:
    /*!
     * return the uniq instance of the class
     */
    static TFEL_VISIBILITY_LOCAL SearchPathsHandler& getSearchPathsHandler();
    /*!
     * \brief default constructor
     *
     * The MFRONT_INCLUDE_PATH environnement variable
     * is used to fill the search paths.
     */
    SearchPathsHandler();
    SearchPathsHandler(const SearchPathsHandler&) = delete;
    SearchPathsHandler(SearchPathsHandler&&) = delete;
    SearchPathsHandler& operator=(const SearchPathsHandler&) = delete;
    SearchPathsHandler& operator=(SearchPathsHandler&&) = delete;
    //! list of search paths
    std::vector<std::string> paths;
  };  // end of struct SearchPathsHandler

}  // namespace mfront

#endif /* LIB_MFRONT_MFRONTSEARCHPATHSHANDLER_HXX */
