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
      std::optional<EntryPoint::MaterialKnowledgeType> type;
      std::optional<std::string> name_filter;
      std::optional<std::string> material_filter;
      std::optional<std::string> interface_filter;
      //
      std::regex_constants::syntax_option_type regular_expression_syntax =
          std::regex_constants::ECMAScript;
    };
    struct AnalyseDirectoryOptions {
      std::optional<std::string> library_prefix;
      std::optional<std::string> library_suffix;
    };
    //
    void analyseLibrary(const std::string&);
    //
    void analyseLibraries(const std::vector<std::string>&);
    //
    void analyseDirectory(const std::string&, const AnalyseDirectoryOptions&);
    //
    void analyseDirectories(const std::vector<std::string>&,
                            const AnalyseDirectoryOptions&);
    //
    std::vector<EntryPoint> getEntryPoints(const Query&) const;
    //
    const std::vector<EntryPoint>& getEntryPoints() const;

   private:
    //! \brief registred entry points
    std::vector<EntryPoint> epts;
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTDATABASE_HXX */
