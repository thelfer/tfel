/*!
 * \file   include/TFEL/System/ExternalMaterialPropertyDescription.hxx
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

#ifndef LIB_TFEL_SYSTEM_EXTERNALMATERIALPROPERTYDESCRIPTION_HXX
#define LIB_TFEL_SYSTEM_EXTERNALMATERIALPROPERTYDESCRIPTION_HXX

#include <string>
#include <vector>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace system {

    /*!
     * \brief a structure containing the information  a given behaviour.
     */
    struct TFELSYSTEM_VISIBILITY_EXPORT ExternalMaterialPropertyData {
      //! \brief default constructor
      ExternalMaterialPropertyData();
      //! \brief move constructor
      ExternalMaterialPropertyData(ExternalMaterialPropertyData&&);
      //! \brief move constructor
      ExternalMaterialPropertyData(const ExternalMaterialPropertyData&);
      //! \brief move assignement
      ExternalMaterialPropertyData& operator=(ExternalMaterialPropertyData&&);
      //! \brief standard assignement
      ExternalMaterialPropertyData& operator=(
          const ExternalMaterialPropertyData&);
      //! \brief version of TFEL used to generate the behaviour
      std::string tfel_version;
      //! \brief build identifier
      std::string build_id;
      //! \brief MFront source file
      std::string source;
      //! \brief name of the interface used to generate the material property
      std::string mfront_interface;
      //! name of the library
      std::string library;
      //! name of the material property
      std::string material_property;
      //! list of arguments
      std::vector<std::string> arguments;
    };  // end of struct ExternalMaterialPropertyData

    /*!
     * A structure containing the information that could be retrieved
     * by the `ExternalLibraryManger` class about a material property
     */
    struct TFELSYSTEM_VISIBILITY_EXPORT ExternalMaterialPropertyDescription
        : public ExternalMaterialPropertyData {
      /*!
       * \param[in] l: library
       * \param[in] m: material property
       */
      ExternalMaterialPropertyDescription(const std::string&,
                                          const std::string&);
      //! \brief default constructor
      ExternalMaterialPropertyDescription();
      //! \brief copy constructor
      ExternalMaterialPropertyDescription(
          const ExternalMaterialPropertyDescription&);
      //! \brief move constructor
      ExternalMaterialPropertyDescription(
          ExternalMaterialPropertyDescription&&);
      //! \brief assignement
      ExternalMaterialPropertyDescription& operator=(
          const ExternalMaterialPropertyDescription&);
      //! \brief move assignement
      ExternalMaterialPropertyDescription& operator=(
          ExternalMaterialPropertyDescription&&);
      //! \brief destructor
      ~ExternalMaterialPropertyDescription();
    };  // end of ExternalMaterialPropertyDescription

  }  // end of namespace system

}  // end of namespace tfel

extern "C" {

/*!
 * \author DeltaCAD
 * \brief build a new behaviour data
 * \note the behaviour data must be freed using
 * `freeExternalMaterialPropertyData`
 */
TFELSYSTEM_VISIBILITY_EXPORT tfel::system::ExternalMaterialPropertyData*
newExternalMaterialPropertyData();
/*!
 * \author DeltaCAD
 * \brief build a new behaviour data
 * \note  the behaviour data
 */
TFELSYSTEM_VISIBILITY_EXPORT void freeExternalMaterialPropertyData(
    tfel::system::ExternalMaterialPropertyData*);
/*!
 * \author DeltaCAD
 * \brief retrieving the information from a behaviour
 * \param[in] d: data to be fetched
 * \param[in] l: library
 * \param[in] m: material property
 *
 * \return nullptr on success, an error message otherwise.\n
 * If an error message is set, it must be freed  using std::free
 * If the allocation of the error message fails, std::exit is called
 */
TFELSYSTEM_VISIBILITY_EXPORT char* getExternalMaterialPropertyData(
    tfel::system::ExternalMaterialPropertyData* const,
    const char* const,
    const char* const);

}  // end of extern "C"

#endif /* LIB_TFEL_SYSTEM_EXTERNALMATERIALPROPERTYDESCRIPTION_HXX */
