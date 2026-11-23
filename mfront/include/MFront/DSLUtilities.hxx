/*!
 * \file   mfront/include/MFront/DSLUtilities.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   23 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_PARSERUTILITIES_HXX_
#define LIB_MFRONT_PARSERUTILITIES_HXX_

#include <iosfwd>

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Glossary/Forward/Glossary.hxx"
#include "MFront/StaticVariableDescription.hxx"

namespace mfront {

  MFRONT_VISIBILITY_EXPORT void writeMaterialLaws(
      const std::string&, std::ostream&, const std::vector<std::string>&);

  MFRONT_VISIBILITY_EXPORT void writeStaticVariables(
      const std::string&,
      std::ostream&,
      const StaticVariableDescriptionContainer&,
      const std::string&);

  MFRONT_VISIBILITY_EXPORT std::string getMaterialLawLibraryNameBase(
      const std::string&, const std::string&);
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
   * \brief display the complete description of a glossary entry
   * \param[out] os : output stream
   * \param[in]  e  : glossary entry
   */
  MFRONT_VISIBILITY_EXPORT void displayGlossaryEntryCompleteDescription(
      std::ostream&, const tfel::glossary::GlossaryEntry&);


  /*!
   * \brief write the contents of a generated file only if the file does
   * not exist or if its contents differ from the given contents.
   *
   * Several mfront processes may generate the files associated with an
   * external mfront file in the same directory (for instance when
   * several behaviours use the same external mfront file and are
   * compiled in parallel). Rewriting such a file which may be
   * concurrently opened by another process or by the compiler can lead
   * to spurious failures, in particular on Windows, and triggers
   * useless rebuilds. This function avoids both by only writing the
   * file when its contents actually change.
   *
   * \param[in] f: file name
   * \param[in] contents: contents of the file
   * \return true if the file has been written, false if it was
   * up-to-date.
   */
  MFRONT_VISIBILITY_EXPORT bool writeIfDifferentFromExistingFileContent(
      const std::string& f, const std::string& contents);

}  // end of namespace mfront

#endif /* LIB_MFRONT_PARSERUTILITIES_HXX_ */
