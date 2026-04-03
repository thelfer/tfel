/*!
 * \file   CMakeGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   16 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CMAKEGENERATOR_HXX
#define LIB_MFRONT_CMAKEGENERATOR_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declaration
  struct TargetsDescription;
  // forward declaration
  struct GeneratorOptions;

  /*!
   * \brief a function generating a standard CMake from a target
   * description.
   * \param[in] t : target description
   * \param[in] o : options
   * \param[in] d : directory
   * \param[in] f : file name
   */
  MFRONT_VISIBILITY_EXPORT void generateCMakeListsFile(
      const TargetsDescription&,
      const GeneratorOptions&,
      const std::string& = "src");
  /*!
   * \brief a function calling the `make` utility in the given
   * directory using the specified file.
   * \param[in] t : target name
   * \param[in] d : directory
   * \param[in] f : file name
   */
  MFRONT_VISIBILITY_EXPORT void callCMake(const std::string&,
                                          const std::string& = "src");

}  // end of namespace mfront

#endif /* LIB_MFRONT_CMAKEGENERATOR_HXX */
