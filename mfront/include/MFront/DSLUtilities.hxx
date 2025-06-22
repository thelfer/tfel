/*!
 * \file   mfront/include/MFront/DSLUtilities.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   23 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DSLUTILITIES_HXX
#define LIB_MFRONT_DSLUTILITIES_HXX

#include <map>
#include <tuple>
#include <iosfwd>
#include <vector>
#include <string>

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Glossary/Forward/Glossary.hxx"
#include "MFront/MaterialKnowledgeType.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/VariableDescription.hxx"

namespace mfront {

  // forward declaration
  struct StaticVariableDescription;
  // forward declaration
  struct MaterialPropertyDescription;
  // forward declaration
  struct BehaviourDescription;

  /*!
   * \brief write the name of the variable
   * \param[out] os: output stream
   * \param[in]  n:  name of the law
   * \param[in]  mpd:  material property description
   */
  MFRONT_VISIBILITY_EXPORT void writeVariablesNamesSymbol(
      std::ostream&,
      const std::string&,
      const mfront::MaterialPropertyDescription&);

  /*!
   * \brief write the bounds
   * \param[out] os: output stream
   * \param[in]  n:  name of the law
   * \param[in]  vn:  name of the variable
   * \param[in]  bt:  type of bounds (physical or not)
   * \param[in]  b:  bound description
   */
  MFRONT_VISIBILITY_EXPORT void writeBoundsSymbol(
      std::ostream&,
      const std::string&,
      const std::string&,
      const std::string&,
      const mfront::VariableBoundsDescription&);
  /*!
   * \brief write the bounds associated with the given variables
   * \param[out] os: output stream
   * \param[in] n:  name of the symbol
   * \param[in] vc:  variables
   */
  MFRONT_VISIBILITY_EXPORT void writeBoundsSymbols(
      std::ostream&, const std::string&, const VariableDescriptionContainer&);
  /*!
   * \brief write the physical bounds associated with the given variables
   * \param[out] os: output stream
   * \param[in] n:  name of the symbol
   * \param[in] vc:  variables
   */
  MFRONT_VISIBILITY_EXPORT void writePhysicalBoundsSymbols(
      std::ostream&, const std::string&, const VariableDescriptionContainer&);

  /*!
   * \brief write the name of the variable
   * \param[out] os: output stream
   * \param[in]  n:  name of the law
   * \param[in]  mpd:  material property description
   */
  MFRONT_VISIBILITY_EXPORT void writeVariablesBoundsSymbols(
      std::ostream&,
      const std::string&,
      const mfront::MaterialPropertyDescription&);

  /*!
   * \brief write a specific symbol stating that a given name entry
   * point (`C`-like function for example) has been created using
   * `MFront`.
   * \param[out] os: output stream
   * \param[in]  n:  entry point
   * name
   */
  MFRONT_VISIBILITY_EXPORT void writeEntryPointSymbol(std::ostream&,
                                                      const std::string&);
  /*!
   * \brief write a specific symbol giving the version of `MFront`
   * used to generate the given entry point.
   * \param[out] os: output stream
   * \param[in]  n:  entry point
   * name
   */
  MFRONT_VISIBILITY_EXPORT void writeTFELVersionSymbol(std::ostream&,
                                                       const std::string&);
  /*!
   * \brief write a specific symbol stating that a given name entry
   * point (`C`-like function for example) has been created using
   * `MFront`.
   * \param[out] os: output stream
   * \param[out] n:  entry point alias
   * \param[out] n2: entry point name
   */
  MFRONT_VISIBILITY_EXPORT void writeEntryPointSymbol(std::ostream&,
                                                      const std::string&,
                                                      const std::string&);
  /*!
   * \brief write a specific symbol for the interface of an entry
   * point.
   * \param[out] os: output stream
   * \param[out] n:  entry point alias
   * \param[out] i: entry point name
   */
  MFRONT_VISIBILITY_EXPORT void writeInterfaceSymbol(std::ostream&,
                                                     const std::string&,
                                                     const std::string&);
  /*!
   * \brief write a symbol stating that a given entry point is
   * associated to a material.
   * \param[out] os: output stream
   * \param[out] n: entry point name
   * \param[in]  m:  material name
   * name
   */
  MFRONT_VISIBILITY_EXPORT void writeMaterialSymbol(std::ostream&,
                                                    const std::string&,
                                                    const std::string&);
  /*!
   * \brief write the symbol giving the material knowledge type.
   * \param[out] os: output stream
   * \param[in]  n:  entry point
   * \param[in]  t:  material knowledge type
   */
  MFRONT_VISIBILITY_EXPORT void writeMaterialKnowledgeTypeSymbol(
      std::ostream&, const std::string&, const MaterialKnowledgeType&);

  MFRONT_VISIBILITY_EXPORT void writeMaterialLaws(
      std::ostream&, const std::vector<std::string>&);

  MFRONT_VISIBILITY_EXPORT void writeStaticVariables(
      std::ostream&,
      const StaticVariableDescriptionContainer&,
      const std::string&);
  /*!
   * \return the base name (without interface specification) of the
   * library to be generated by `MFront`.
   * \param[in] mpd: material property description
   */
  MFRONT_VISIBILITY_EXPORT std::string getMaterialLawLibraryNameBase(
      const MaterialPropertyDescription&);
  /*!
   * \brief define the F77_FUNC and F77_FUNC_ macros. We support the
   * GNU (gfortran) and the intel compilers. By default, the GNU
   * fortran naming scheme is used. This can be changed when compiling
   * mfront generated sources by defining the INTEL_FORTRAN_COMPILER
   * flag.
   * \param[out] f   : output file
   *
   * https://en.wikipedia.org/wiki/Name_mangling :
   *
   * Many compilers, including SGI's IRIX compilers, GNU Fortran, and
   * Intel's Fortran compiler (except on Microsoft Windows), convert
   * all identifiers to lower case plus an underscore ("foo_" and
   * "foo_bar_"). On Microsoft Windows, the Intel Fortran compiler
   * defaults to uppercase without an underscore.[2]
   *
   * [2] User and Reference Guide for the Intel Fortran Compiler
   * 15.0, Intel Corporation (2014), Summary of Mixed-Language
   * Issues. Accessed 17 Nov. 2014.
   */
  MFRONT_VISIBILITY_EXPORT void writeF77FUNCMacros(std::ostream&);
  /*!
   * \brief define export directives (visibility attributes) and
   * calling conventions through the MFRONT_SHAREDOBJ and the
   * MFRONT_CALLING_CONVENTION macros
   * \param[out] f   : output file
   */
  MFRONT_VISIBILITY_EXPORT void writeExportDirectives(std::ostream&);

  MFRONT_VISIBILITY_EXPORT std::string makeUpperCase(const std::string&);

  MFRONT_VISIBILITY_EXPORT std::string makeLowerCase(const std::string&);
  /*!
   * \brief add the given symbol.
   * \param[in,out] symbols: mapping between a symbol and a replacement string
   * \param[in] s: symbol
   * \param[in] r: replacement string
   */
  MFRONT_VISIBILITY_EXPORT void addSymbol(std::map<std::string, std::string>&,
                                          const std::string&,
                                          const std::string&);
  /*!
   * \brief display the complete description of a glossary entry
   * \param[out] os : output stream
   * \param[in]  e  : glossary entry
   */
  MFRONT_VISIBILITY_EXPORT void displayGlossaryEntryCompleteDescription(
      std::ostream&, const tfel::glossary::GlossaryEntry&);
  /*!
   * \brief export all symbols related to parameters
   * \param[out] os: output stream
   * \param[out] n: name of the entry point
   * \param[out] parameters: list of parameters
   *
   * This functions calls successively:
   *
   * - `writeParametersDecarationSymbols`
   * - `writeParametersDefaultValuesSymbols`
   * - `writeBoundsSymbols`
   * - `writePhysicalBoundsSymbols`
   */
  MFRONT_VISIBILITY_EXPORT void writeParametersSymbols(
      std::ostream&, const std::string&, const MaterialPropertyDescription&);
  /*!
   * \brief export parameters declarations
   * \param[out] os: output stream
   * \param[out] n: name of the entry point
   * \param[out] parameters: list of parameters
   */
  MFRONT_VISIBILITY_EXPORT void writeParametersDeclarationSymbols(
      std::ostream&, const std::string&, const VariableDescriptionContainer&);
  /*!
   * \brief export the default values of a set of parameters
   * \param[out] os: output stream
   * \param[out] n: name of the entry point
   * \param[out] parameters: list of parameters
   */
  MFRONT_VISIBILITY_EXPORT void writeParametersDefaultValuesSymbols(
      std::ostream&, const std::string&, const VariableDescriptionContainer&);
  /*!
   * \brief write a symbol describing an array of strings
   * \param[out] os: output stream
   * \param[out] n: name of the generated symbol
   * \param[out] values: array of strings
   */
  MFRONT_VISIBILITY_EXPORT
  void writeArrayOfStringsSymbol(std::ostream&,
                                 const std::string&,
                                 const std::vector<std::string>&);

#ifdef MFRONT_HAVE_MADNEX

  /*!
   * \brief decompose an implementation location in a madnex file. The path
   * is assumed to have the following form:
   * `madnex:<file>:<type>:<material>:<implementation>`
   * \param[in] path : implementation
   * \return a tuple of strings containng the file path, the type of the
   * implementation (material property, behaviour or model), the material name,
   * the name of the implementation
   */
  MFRONT_VISIBILITY_EXPORT
  std::tuple<std::string, std::string, std::string, std::string>
  decomposeImplementationPathInMadnexFile(const std::string&);

#endif /* MFRONT_HAVE_MADNEX */

}  // end of namespace mfront

#endif /* LIB_MFRONT_DSLUTILITIES_HXX */
