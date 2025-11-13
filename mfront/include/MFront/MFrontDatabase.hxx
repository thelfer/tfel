/*!
 * \file   MFront/MFrontDatabase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/11/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTDATABASE_HXX
#define LIB_MFRONT_MFRONTDATABASE_HXX

#include <regex>
#include <string>
#include <vector>
#include <optional>
#include <string_view>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief a data structure containing the result of the analysis of some
   * MFront libraries
   */
  struct MFRONTDATABASE_VISIBILITY_EXPORT MFrontDatabase {
    //! \brief an intermediate structure describing an entry point
    struct EntryPoint {
      //! \brief type of material knowledge
      enum MaterialKnowledgeType {
        MATERIALPROPERTY,
        BEHAVIOUR,
        MODEL
      };
      //! \brief library in which the entry point is defined
      std::string library;
      //! \brief name of the entry point
      std::string name;
      //! \brief source of the entry point
      std::string source;
      //! \brief type of material knownledge described by the entry point
      MaterialKnowledgeType type;
    };  // end of EntryPoint
    //! \brief data structure used to filter entry points
    struct Query {
      //! \brief material knownledge type
      std::optional<EntryPoint::MaterialKnowledgeType> type = {};
      //! \brief regular expression to filter the name of entry point
      std::optional<std::string> name_filter = {};
      /*!
       * \brief regular expression to filter the material associated with an
       *  entry point
       */
      std::optional<std::string> material_filter = {};
      /*!
       * \brief regular expression to filter the interface associated with an
       *  entry point
       */
      std::optional<std::string> interface_filter = {};
      //! \brief regular expression syntax
      std::regex_constants::syntax_option_type regular_expression_syntax =
          std::regex_constants::ECMAScript;
    };
    /*!
     * \brief options for the analysis of a directory
     */
    struct AnalyseDirectoryOptions {
      /*!
       * \brief if true, the failure a library analysis is ignored, but the
       * failure is reported in the returned data structure.
       */
      bool ignore_errors = false;
      /*!
       * \brief library prefix
       *
       * If undefined, a default prefix depending on the system considered is
       * used.
       */
      std::optional<std::string> library_prefix = {};
      /*!
       * \brief library prefix
       *
       * If undefined, a default suffix depending on the system considered is
       * used.
       */
      std::optional<std::string> library_suffix = {};
    };
    //! \brief data structure resulting for the analysis of a directory
    struct AnalyseDirectoryResults {
      /*!
       * \brief data structure describing the failure of the analysis of a
       * library
       */
      struct LibraryAnalysisFailureReport {
        //! \brief path of the library the analysis of which failed
        std::string library;
        //! \brief error message
        std::string error_message;
      };
      //! \brief reports of the all failed analyses
      std::vector<LibraryAnalysisFailureReport> library_analysis_failures;
      //! \brief list of all invalid directories
      std::vector<std::string> invalid_directories;
    };
    /*!
     * \brief analyse a library
     * \param[in] l: library to be analysed
     */
    void analyseLibrary(const std::string&);
    /*!
     * \brief analyse a set of libraries
     * \param[in] libraries: libraries to be analysed
     */
    void analyseLibraries(const std::vector<std::string>&);
    /*!
     * \brief method used to analyse a directory with the default options
     * \param[in] d: directory
     * \return the results of the analysis
     */
    AnalyseDirectoryResults analyseDirectory(const std::string&);
    /*!
     * \brief method used to analyse a directory
     * \param[in] d: directory
     * \param[in] opts: options used for the analysis
     * \return the results of the analysis
     */
    AnalyseDirectoryResults analyseDirectory(const std::string&,
                                             const AnalyseDirectoryOptions&);
    /*!
     * \brief method used to analyse a list of directories with the default
     * options
     *
     * \param[in] d: directory
     * \return the results of the analysis
     */
    AnalyseDirectoryResults analyseDirectories(const std::vector<std::string>&);
    /*!
     * \brief method used to analyse a list of directories
     * \param[in] d: directory
     * \param[in] opts: options used for the analysis
     * \return the results of the analysis
     */
    AnalyseDirectoryResults analyseDirectories(const std::vector<std::string>&,
                                               const AnalyseDirectoryOptions&);
    /*!
     * \brief method used to analyse a list of directories given in an
     * environment variable
     *
     * \param[in] d: directory
     * \return the results of the analysis
     *
     * \note if the environment variable is not defined, nothing is done
     */
    AnalyseDirectoryResults analyseDirectoriesListedInEnvironmentVariable(
        const std::string&);
    /*!
     * \brief method used to analyse a list of directories given in an
     * environment variable
     *
     * \param[in] d: directory
     * \param[in] opts: options used for the analysis
     * \return the results of the analysis
     *
     * \note if the environment variable is not defined, nothing is done
     */
    AnalyseDirectoryResults analyseDirectoriesListedInEnvironmentVariable(
        const std::string&, const AnalyseDirectoryOptions&);
    /*!
     * \return the list of entry points matching the given query
     * \param[in] q: query
     */
    std::vector<EntryPoint> getEntryPoints(const Query&) const;
    //! \return all the entry points
    const std::vector<EntryPoint>& getEntryPoints() const;
    //! \brief clear all registred entry points
    void clear();

   private:
    /*!
     * \brief method used to analyse a directory
     * \param[out] r: result of the analysis
     * \param[in] d: directory
     * \param[in] opts: options used for the analysis
     */
    void analyseDirectory(AnalyseDirectoryResults&,
                          const std::string&,
                          const AnalyseDirectoryOptions&);
    //! \brief registred entry points
    std::vector<EntryPoint> epts;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTDATABASE_HXX */
