/*!
 * \file   MaterialPropertyParametersHandler.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 janv. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALPROPERTYPARAMETERSHANDLER_HXX
#define LIB_MFRONT_MATERIALPROPERTYPARAMETERSHANDLER_HXX

#include <iosfwd>
#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDescription;

  struct WriteMaterialPropertyParametersHandlerArguments {
    //!\brief name of the material property
    const std::string material_property_name;
    //!\brief floating point type
    const std::string floating_point_type;
    //!\brief interface associated with the interface
    const std::string interface_namespace;
    /*!
     * \brief if true, the copy constructor of the parameters's handler is not
     * disabled.
     */
    const bool allow_copy_constructor = false;
  };

  /*!
   * \brief write the implementation of the class able to manage the parameters
   * of a material property for a given interface.
   *
   * \param[out] os: ouptut stream
   * \param[in]  mpd: material property description
   * \param[in]  args: additional arguments
   */
  MFRONT_VISIBILITY_EXPORT void writeMaterialPropertyParametersHandler(
      std::ostream&,
      const MaterialPropertyDescription&,
      const WriteMaterialPropertyParametersHandlerArguments&);

  /*!
   * \param[out] os: ouptut stream
   * \param[in]  mpd: material property description
   * \param[in]  n: name of the material property
   * \param[in]  t: type of the parameters
   * \param[in]  i: namspace associated with the interface
   */
  MFRONT_VISIBILITY_EXPORT void writeMaterialPropertyParametersHandler(
      std::ostream&,
      const MaterialPropertyDescription&,
      const std::string&,
      const std::string&,
      const std::string&);

  /*!
   * \param[out] os: ouptut stream
   * \param[in]  mpd: material property description
   * \param[in]  n: name of the material property
   * \param[in]  t: type of the parameters
   * \param[in]  i: namespace associated with the interface
   */
  MFRONT_VISIBILITY_EXPORT void writeAssignMaterialPropertyParameters(
      std::ostream&,
      const MaterialPropertyDescription&,
      const std::string&,
      const std::string&,
      const std::string&);
  /*!
   * \param[in]  n: name of the material property
   */
  MFRONT_VISIBILITY_EXPORT std::string
  getMaterialPropertyParametersHandlerClassName(const std::string&);

  struct WriteMaterialPropertyParametersSetterFunctionArguments {
    //!\brief name of the material property
    const std::string material_property_name;
    //!\brief floating point type
    const std::string floating_point_type;
    //!\brief interface associated with the interface
    const std::string interface_namespace;
  };

  /*!
   * \brief write the declaration of the function allowing the set the values
   * of the parameters of a material property
   *
   * \param[out] os: ouptut stream
   * \param[in]  mpd: material property description
   * \param[in]  args: additional arguments
   */
  MFRONT_VISIBILITY_EXPORT void writeParametersSetterFunctionDeclaration(
      std::ostream&,
      const MaterialPropertyDescription&,
      const WriteMaterialPropertyParametersSetterFunctionArguments&);

  /*!
   * \brief write the implementation of the function allowing the set the values
   * of the parameters of a material property
   *
   * \param[out] os: ouptut stream
   * \param[in]  mpd: material property description
   * \param[in]  args: additional arguments
   */
  MFRONT_VISIBILITY_EXPORT void writeParametersSetterFunctionImplementation(
      std::ostream&,
      const MaterialPropertyDescription&,
      const WriteMaterialPropertyParametersSetterFunctionArguments&);

}  // namespace mfront

#endif /* LIB_MFRONT_MATERIALPROPERTYPARAMETERSHANDLER_HXX */
