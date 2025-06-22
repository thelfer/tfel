/*!
 * \file   include/TFEL/System/ExternalMaterialKnowledgeDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15 June 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_EXTERNALMATERIALKNOWLEDGEDESCRIPTION_HXX
#define LIB_TFEL_SYSTEM_EXTERNALMATERIALKNOWLEDGEDESCRIPTION_HXX

#include <string>
#include <vector>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::system {

  /*!
   * \brief a structure containing the information about a given material
   * knowledge.
   */
  struct TFELSYSTEM_VISIBILITY_EXPORT ExternalMaterialKnowledgeDescription {
    /*!
     * \param[in] l: library
     * \param[in] f: entry point corresponding to the material knowledge
     */
    ExternalMaterialKnowledgeDescription(const std::string&,
                                         const std::string&);
    //! \brief default constructor
    ExternalMaterialKnowledgeDescription();
    //! \brief move constructor
    ExternalMaterialKnowledgeDescription(
        ExternalMaterialKnowledgeDescription&&);
    //! \brief move constructor
    ExternalMaterialKnowledgeDescription(
        const ExternalMaterialKnowledgeDescription&);
    //! \brief move assignement
    ExternalMaterialKnowledgeDescription& operator=(
        ExternalMaterialKnowledgeDescription&&);
    //! \brief standard assignement
    ExternalMaterialKnowledgeDescription& operator=(
        const ExternalMaterialKnowledgeDescription&);
    //! \brief destructor
    ~ExternalMaterialKnowledgeDescription();
    //! \brief name of the library
    std::string library;
    //! \brief name of the entry point
    std::string entry_point;
    //! \brief version of TFEL used to generate the entry point
    std::string tfel_version;
    /*!
     * \brief unit system used by the entry point (can be empty if no unit
     * system has been specified)
     */
    std::string unit_system;
    //! \brief build identifier
    std::string build_id;
    //! \brief MFront source file
    std::string source;
    //! \brief name of the interface used to generate the entry point
    std::string mfront_interface;
    //! \brief name of the material
    std::string material;
    //! \brief author of the implementation
    std::string author;
    //! \brief date of the implementation
    std::string date;
    //! \brief description of the material knowledge
    std::string description;
  };  // end of struct ExternalMaterialKnowledgeDescription

}  // end of namespace tfel::system

#endif /* LIB_TFEL_SYSTEM_EXTERNALMATERIALKNOWLEDGEDESCRIPTION_HXX */
