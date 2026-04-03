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

  /*!
   * \param[out] os: ouptut stream
   * \param[in]  mpd: material property description
   * \param[in]  n: name of the material property
   * \param[in]  t: type of the parameters
   * \param[in]  i: interface name
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
   * \param[in]  i: interface name
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

}  // namespace mfront

#endif /* LIB_MFRONT_MATERIALPROPERTYPARAMETERSHANDLER_HXX */
