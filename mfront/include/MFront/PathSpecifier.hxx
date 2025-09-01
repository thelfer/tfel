/*!
 * \file   PathSpecifier.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06/10/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_PATHSPECIFIER_HXX
#define LIB_MFRONT_PATHSPECIFIER_HXX

#include <string>
#include <vector>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief a structure allowing to specify paths of MFront implementations in
   * a file.
   */
  struct MFRONT_VISIBILITY_EXPORT PathSpecifier {
    //! \brief default constructor
    PathSpecifier();
    //! \brief move constructor
    PathSpecifier(PathSpecifier&&);
    //! \brief copy constructor
    PathSpecifier(const PathSpecifier&);
    //! \brief move assignement
    PathSpecifier& operator=(PathSpecifier&&);
    //! \brief standard assignement
    PathSpecifier& operator=(const PathSpecifier&);
    //! \brief destructor
    ~PathSpecifier();
    //! \brief file name
    std::string file;
    //! \brief identifier of the material
    std::string material_identifier;
    //! \brief identifier for material properties
    std::string material_property_identifier;
    //! \brief identifier for behaviours
    std::string behaviour_identifier;
    //! \brief identifier for models
    std::string model_identifier;
  };  // end of struct PathSpecifier

  /*!
   * \brief treat command line arguments associated with the specification of a
   * path specifier.
   * \return true if the argument was treated
   * \param[in] paths: registred path specifiers
   * \param[in] s: current path specifier
   * \param[in] a: current command line argument
   */
  MFRONT_VISIBILITY_EXPORT bool parsePathSpecifierArguments(
      std::vector<PathSpecifier>&, PathSpecifier&, const std::string&);
  /*!
   * \brief finalize the treatment of command line arguments associated with
   * path specifiers.
   *
   * \param[in] paths: registred path specifiers
   * \param[in] s: current path specifier
   */
  MFRONT_VISIBILITY_EXPORT void finalizePathSpecifierArgumentsParsing(
      std::vector<PathSpecifier>&, const PathSpecifier&);

  /*!
   * \return the paths to `MFront` implementations matching the given
   * pattern in the given file.
   * \param[in] f: file name
   * \param[in] material_identifier: identifier of the material (may be empty).
   * \param[in] behaviour_identifier: identifier for material properties.
   * \param[in] behaviour_identifier: identifier for behaviours.
   * \param[in] model_identifier: identifier for behaviours.
   *
   * \note  Standard `MFront` files only contains one implementations. `Madnex`
   * files may contain several implementations.
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getImplementationsPaths(
      const std::string&,
      const std::string&,
      const std::string&,
      const std::string&,
      const std::string&);
  /*!
   * \return the paths to `MFront` implementations given a path specifier.
   *
   * \param[in] p: path specifier
   * \note  Standard `MFront` files only contains one implementations. `Madnex`
   * files may contain several implementations.
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getImplementationsPaths(
      const PathSpecifier&);
  /*!
   * \return the paths to `MFront` implementations given
   * a set of path specifiers.
   *
   * \param[in] paths: path specifiers
   * \note  Standard `MFront` files only contains one implementations. `Madnex`
   * files may contain several implementations.
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getImplementationsPaths(
      const std::vector<PathSpecifier>&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_PATHSPECIFIER_HXX */
