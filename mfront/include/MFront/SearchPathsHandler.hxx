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
#include <tuple>
#include <string>
#include <vector>
#include <variant>
#include <string_view>

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * structure in charge of:
   * - storing search paths
   * - searching imported mfront file
   */
  struct MFRONT_VISIBILITY_EXPORT SearchPathsHandler {
    //! \brief structure describing a path in a madnex file
    struct MadnexPath {
      //! \brief file path
      std::string file_path;
      //! \brief material
      std::string material;
      //! \brief type of material knowledge
      enum MaterialKnowledgeType {
        MADNEX_ALL,
        MADNEX_MATERIAL_PROPERTY,
        MADNEX_BEHAVIOUR,
        MADNEX_MODEL
      } mkt = MADNEX_ALL;
    };
    //! \brief a simple alias
    using Path = std::variant<std::string, MadnexPath>;
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
     * \brief append a madnex search path
     *
     * The path has one of the form:
     *
     * - `<file_path>`
     * - `<file_path>:<material_id>`
     * - `<file_path>:<material_id>:<mkt>`
     *
     * If the material identifier is not specified, all
     * materials are selected, including the empty one.
     *
     * `<material_id>` can be a regular expression.
     * `<material_id>` can also have the special value `<none>`.
     *
     * <mkt> can have one of the values `MaterialProperties`, `Behaviours` or
     * `Models`.
     *
     * \param[in] p: path
     */
    static void addMadnexSearchPath(const std::string&);
    /*!
     * \brief Add new search paths.
     *
     * Multiple paths are separated by commas under unices systems and
     * by semicolons under Windows systems.
     * \param[in] p : new search paths
     */
    static void addSearchPaths(const std::string&);
    /*!
     * \brief add search paths for implementations paths.
     * \param[in] paths : implementation paths
     */
    static void addSearchPathsFromImplementationPaths(
        const std::vector<std::string>&);
    //! \return the list of the search paths associated with directories
    static std::vector<std::string> getSearchPaths();
    //! \return the registred paths
    static std::vector<Path> getRegistredPaths();
    /*!
     * \brief reset the paths
     * \param[in] npaths: new paths
     */
    static void resetPaths(const std::vector<Path>&);

   private:
    //! \return the unique instance of the class
    TFEL_VISIBILITY_LOCAL static SearchPathsHandler& getSearchPathsHandler();
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
    //! \brief return the path to a madnex file
    TFEL_VISIBILITY_LOCAL static std::string searchMadnexFile(
        const std::string&);
    //! \brief list of search paths
    std::vector<Path> paths;
  };  // end of struct SearchPathsHandler

#ifdef MFRONT_HAVE_MADNEX

  /*!
   * \brief decompose an implementation location in a madnex file. The path
   * is assumed to have the following form:
   * `madnex:<file>:<type>:<material>:<implementation>`
   * \param[in] path : implementation
   * \return a tuple of strings containng the file path, the type of the
   * implementation (material property, behaviour or model), the material
   * name, the name of the implementation
   */
  MFRONT_VISIBILITY_EXPORT
  std::tuple<std::string, std::string, std::string, std::string>
  decomposeImplementationPathInMadnexFile(const std::string_view);

#endif /* MFRONT_HAVE_MADNEX */

}  // namespace mfront

#endif /* LIB_MFRONT_MFRONTSEARCHPATHSHANDLER_HXX */
