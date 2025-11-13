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

namespace mfront{

  /*!
   * \brief a data structure containing the result of the analysis of some
   * MFront libraries
   */
  struct MFRONTDATABASE_VISIBILITY_EXPORT MFrontDatabase {
    //! \brief an intermediate structure describing an entry point
    struct EntryPoint {
      //! type of material knowledge
      enum MaterialKnowledgeType {
        MATERIALPROPERTY,
        BEHAVIOUR,
        MODEL
      };  // end of MaterialKnowledgeType
      std::string library;
      std::string name;
      std::string source;
      MaterialKnowledgeType type;
    };  // end of EntryPoint
    //! \brief class 
    struct Query {
      std::optional<EntryPoint::MaterialKnowledgeType> type = {};
      std::optional<std::string> name_filter = {};
      std::optional<std::string> material_filter = {};
      std::optional<std::string> interface_filter = {};
      //
      std::regex_constants::syntax_option_type regular_expression_syntax =
          std::regex_constants::ECMAScript;
    };
    struct AnalyseDirectoryOptions {
      bool ignore_errors = false;
      std::optional<std::string> library_prefix = {};
      std::optional<std::string> library_suffix = {};
    };
    struct LibraryAnalysisFailureReport {
      std::string library;
      std::string error_message;
    };
    struct AnalyseDirectoryResults {
      std::vector<LibraryAnalysisFailureReport> errors;
    };
    //
    void analyseLibrary(const std::string&);
    //
    void analyseLibraries(const std::vector<std::string>&);
    //
    AnalyseDirectoryResults analyseDirectory(const std::string&);
    AnalyseDirectoryResults analyseDirectory(const std::string&,
                                             const AnalyseDirectoryOptions&);
    //
    AnalyseDirectoryResults analyseDirectories(const std::vector<std::string>&);
    AnalyseDirectoryResults analyseDirectories(const std::vector<std::string>&,
                                               const AnalyseDirectoryOptions&);
    //
    std::vector<EntryPoint> getEntryPoints(const Query&) const;
    //
    const std::vector<EntryPoint>& getEntryPoints() const;

   private:
    void analyseDirectory(AnalyseDirectoryResults&,
                          const std::string&,
                          const AnalyseDirectoryOptions&);
    //! \brief registred entry points
    std::vector<EntryPoint> epts;
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTDATABASE_HXX */
