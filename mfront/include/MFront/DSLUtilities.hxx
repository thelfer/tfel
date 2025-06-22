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
#include <vector>
#include <string>
#include <string_view>

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Glossary/Forward/Glossary.hxx"
#include "MFront/MaterialKnowledgeType.hxx"

namespace mfront {

  // forward declarations
  struct VariableDescription;
  struct VariableDescriptionContainer;
  struct VariableBoundsDescription;
  struct StaticVariableDescription;
  struct StaticVariableDescriptionContainer;
  struct FileDescription;
  struct MaterialKnowledgeDescription;
  struct MaterialPropertyDescription;
  struct BehaviourDescription;

  /*!
   * \return a list of standard types defined in the `tfel::config::Type` type
   * \note this method internally calls the `getStandardScalarTFELTypedefs`
   * function.
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getTypeAliases();
  /*!
   * \return a list of standard scalar types defined in the
   * `tfel::config::Type` type
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getScalarTypeAliases();
  /*!
   * \return a list of standard tiny vector types defined in the
   * `tfel::config::Type` type
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getTinyVectorTypeAliases();
  /*!
   * \return a list of standard symmetric tensor types defined in the
   * `tfel::config::Type` type
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getStensorTypeAliases();
  /*!
   * \return a list of standard tensor types defined in the
   * `tfel::config::Type` type
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getTensorTypeAliases();
  /*!
   * \return a list of standard st2tost2 types defined in the
   * `tfel::config::Type` type
   */
  MFRONT_VISIBILITY_EXPORT std::vector<std::string> getST2toST2TypeAliases();

  /*!
   * \brief export a symbol using the `MFRONT_EXPORT_SYMBOL` macro
   * \param[out] os: output stream
   * \param[in] t: type of the symbol
   * \param[in] n: name of the symbol
   * \param[in] v: value of the symbol
   */
  template <typename ValueType>
  void exportSymbol(std::ostream&,
                    const std::string_view,
                    const std::string_view,
                    const ValueType&);
  /*!
   * \brief export string symbol using the `MFRONT_EXPORT_SYMBOL` macro
   * \param[out] os: output stream
   * \param[in] n: name of the symbol
   */
  void exportStringSymbol(std::ostream&,
                          const std::string_view,
                          const std::string_view);
  /*!
   * \brief export string symbol using the `MFRONT_EXPORT_SYMBOL` macro
   * \param[out] os: output stream
   * \param[in] n: name of the symbol
   */
  void exportUnsignedShortSymbol(std::ostream&,
                                 const std::string_view,
                                 const unsigned short);
  /*!
   * \brief write the name of the variable
   * \param[out] os: output stream
   * \param[in]  n:  name of the law
   * \param[in]  mpd:  material property description
   */
  MFRONT_VISIBILITY_EXPORT void writeVariablesNamesSymbol(
      std::ostream&,
      const std::string_view,
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
      const std::string_view,
      const std::string_view,
      const std::string_view,
      const mfront::VariableBoundsDescription&);
  /*!
   * \brief write the bounds associated with the given variables
   * \param[out] os: output stream
   * \param[in] n:  name of the symbol
   * \param[in] vc:  variables
   */
  MFRONT_VISIBILITY_EXPORT void writeBoundsSymbols(
      std::ostream&,
      const std::string_view,
      const VariableDescriptionContainer&);
  /*!
   * \brief write the physical bounds associated with the given variables
   * \param[out] os: output stream
   * \param[in] n:  name of the symbol
   * \param[in] vc:  variables
   */
  MFRONT_VISIBILITY_EXPORT void writePhysicalBoundsSymbols(
      std::ostream&,
      const std::string_view,
      const VariableDescriptionContainer&);

  /*!
   * \brief write the name of the variable
   * \param[out] os: output stream
   * \param[in]  n:  name of the law
   * \param[in]  mpd:  material property description
   */
  MFRONT_VISIBILITY_EXPORT void writeVariablesBoundsSymbols(
      std::ostream&,
      const std::string_view,
      const mfront::MaterialPropertyDescription&);
  /*!
   * \brief write symbols associated with the file description.
   * \param[out] os: output stream
   * \param[in]  n: entry point name
   * \param[in] fd: file description
   */
  MFRONT_VISIBILITY_EXPORT void writeFileDescriptionSymbols(
      std::ostream&, const std::string_view, const FileDescription&);

  /*!
   * \brief write a symbol associated with the build identifier.
   * \param[out] os: output stream
   * \param[in]  n: entry point name
   * \param[in] d: description
   */
  MFRONT_VISIBILITY_EXPORT void writeBuildIdentifierSymbol(
      std::ostream&,
      const std::string_view,
      const MaterialKnowledgeDescription&);
  /*!
   * \brief write a specific symbol stating that a given name entry
   * point (`C`-like function for example) has been created using
   * `MFront`.
   * \param[out] os: output stream
   * \param[in]  n:  entry point
   * name
   */
  MFRONT_VISIBILITY_EXPORT void writeEntryPointSymbol(std::ostream&,
                                                      const std::string_view);
  /*!
   * \brief write a specific symbol giving the version of `MFront`
   * used to generate the given entry point.
   * \param[out] os: output stream
   * \param[in]  n:  entry point name
   */
  MFRONT_VISIBILITY_EXPORT void writeTFELVersionSymbol(std::ostream&,
                                                       const std::string_view);
  /*!
   * \brief write a specific symbol giving the version of `MFront`
   * used to generate the given entry point.
   * \param[out] os: output stream
   * \param[in]  n:  entry point name
   * \param[in]  d:  description
   */
  MFRONT_VISIBILITY_EXPORT void writeUnitSystemSymbol(
      std::ostream&,
      const std::string_view,
      const MaterialKnowledgeDescription&);
  /*!
   * \brief write a specific symbol stating that a given name entry
   * point (`C`-like function for example) has been created using
   * `MFront`.
   * \param[out] os: output stream
   * \param[out] n:  entry point alias
   * \param[out] n2: entry point name
   */
  MFRONT_VISIBILITY_EXPORT void writeEntryPointSymbol(std::ostream&,
                                                      const std::string_view,
                                                      const std::string_view);
  /*!
   * \brief write a specific symbol for the interface of an entry
   * point.
   * \param[out] os: output stream
   * \param[out] n:  entry point alias
   * \param[out] i: entry point name
   */
  MFRONT_VISIBILITY_EXPORT void writeInterfaceSymbol(std::ostream&,
                                                     const std::string_view,
                                                     const std::string_view);
  /*!
   * \brief write a symbol stating that a given entry point is
   * associated to a material law.
   * \param[out] os: output stream
   * \param[out] n: entry point name
   * \param[in]  l: law name
   * name
   */
  MFRONT_VISIBILITY_EXPORT void writeLawSymbol(std::ostream&,
                                               const std::string_view,
                                               const std::string_view);
  /*!
   * \brief write a symbol stating that a given entry point is
   * associated to a material.
   * \param[out] os: output stream
   * \param[out] n: entry point name
   * \param[in]  m:  material name
   * name
   */
  MFRONT_VISIBILITY_EXPORT void writeMaterialSymbol(std::ostream&,
                                                    const std::string_view,
                                                    const std::string_view);
  /*!
   * \brief write the symbol giving the material knowledge type.
   * \param[out] os: output stream
   * \param[in]  n:  entry point
   * \param[in]  t:  material knowledge type
   */
  MFRONT_VISIBILITY_EXPORT void writeMaterialKnowledgeTypeSymbol(
      std::ostream&, const std::string_view, const MaterialKnowledgeType&);

  MFRONT_VISIBILITY_EXPORT void writeMaterialLaws(
      std::ostream&, const std::vector<std::string>&);

  MFRONT_VISIBILITY_EXPORT void writeStaticVariables(
      std::ostream&,
      const StaticVariableDescriptionContainer&,
      const std::string_view);
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
   * \brief define the MFRONT_SHAREDOBJ macro which contains visibility
   * attributes.
   * \param[out] os: output file
   * \param[in] b: boolean stating if the `MFRONT_COMPILING` macro shall be
   * taken into account.
   */
  MFRONT_VISIBILITY_EXPORT void writeExportDirectives(std::ostream&,
                                                      const bool = true);

  MFRONT_VISIBILITY_EXPORT std::string makeUpperCase(const std::string_view);

  MFRONT_VISIBILITY_EXPORT std::string makeLowerCase(const std::string_view);
  /*!
   * \brief add the given symbol.
   * \param[in,out] symbols: mapping between a symbol and a replacement string
   * \param[in] s: symbol
   * \param[in] r: replacement string
   */
  MFRONT_VISIBILITY_EXPORT void addSymbol(std::map<std::string, std::string>&,
                                          const std::string_view,
                                          const std::string_view);
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
      std::ostream&,
      const std::string_view,
      const MaterialPropertyDescription&);
  /*!
   * \brief export parameters declarations
   * \param[out] os: output stream
   * \param[out] n: name of the entry point
   * \param[out] parameters: list of parameters
   */
  MFRONT_VISIBILITY_EXPORT void writeParametersDeclarationSymbols(
      std::ostream&,
      const std::string_view,
      const VariableDescriptionContainer&);
  /*!
   * \brief export the default values of a set of parameters
   * \param[out] os: output stream
   * \param[out] n: name of the entry point
   * \param[out] parameters: list of parameters
   */
  MFRONT_VISIBILITY_EXPORT void writeParametersDefaultValuesSymbols(
      std::ostream&,
      const std::string_view,
      const VariableDescriptionContainer&);
  /*!
   * \brief write a symbol describing an array of integers
   * \param[out] os: output stream
   * \param[out] n: name of the generated symbol
   * \param[out] values: array of integers
   */
  MFRONT_VISIBILITY_EXPORT
  void exportArrayOfIntegersSymbol(std::ostream&,
                                   const std::string_view,
                                   const std::vector<int>&);
  /*!
   * \brief write a symbol describing an array of strings
   * \param[out] os: output stream
   * \param[in] n: name of the generated symbol
   * \param[in] values: array of strings
   */
  MFRONT_VISIBILITY_EXPORT
  void exportArrayOfStringsSymbol(std::ostream&,
                                  const std::string_view,
                                  const std::vector<std::string>&);
  /*!
   * \param[out] os: output stream
   * \param[in] v: variable description
   * \param[in] n: name of the variable tested. If the variable describes an
   * array of variables, this argument may point to an element of this array.
   * \param[in] space_dimension: string containing the space dimension
   * \param[in] policy: name of the variable holding the policy
   * \param[in] addThis: add "this->" in front of the variables
   * \param[in] checkEndOfTimeStepValue: also check the value at the end of
   * the time step given by the value of the variable plus its increment.
   */
  MFRONT_VISIBILITY_EXPORT
  void writeBoundsChecks(std::ostream&,
                         const VariableDescription&,
                         const std::string_view,
                         const std::string_view,
                         const std::string_view,
                         const bool,
                         const bool);
  /*!
   * \param[out] os: output stream
   * \param[in] v: variable description
   * \param[in] n: name of the variable tested. If the variable describes an
   * array of variables, this argument may point to an element of this array.
   * \param[in] space_dimension: string containing the space dimension
   * \param[in] addThis: add "this->" in front of the variables
   * \param[in] checkEndOfTimeStepValue: also check the value at the end of
   * the time step given by the value of the variable plus its increment.
   */
  MFRONT_VISIBILITY_EXPORT
  void writePhysicalBoundsChecks(std::ostream&,
                                 const VariableDescription&,
                                 const std::string_view,
                                 const std::string_view,
                                 const bool,
                                 const bool);

#ifdef MFRONT_HAVE_MADNEX

  /*!
   * \brief decompose an implementation location in a madnex file. The path
   * is assumed to have the following form:
   * `madnex:<file>:<type>:<material>:<implementation>`
   * \param[in] path : implementation
   * \return a tuple of strings containng the file path, the type of the
   * implementation (material property, behaviour or model), the material
   * name, the name of the implementation
   */
  MFRONT_VISIBILITY_EXPORT
  std::tuple<std::string, std::string, std::string, std::string>
  decomposeImplementationPathInMadnexFile(const std::string_view);

#endif /* MFRONT_HAVE_MADNEX */

}  // end of namespace mfront

#include "MFront/DSLUtilities.ixx"

#endif /* LIB_MFRONT_DSLUTILITIES_HXX */
