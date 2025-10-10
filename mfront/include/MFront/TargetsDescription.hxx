/*!
 * \file  TargetsDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_TARGETSDESCRIPTION_HXX
#define LIB_MFRONT_TARGETSDESCRIPTION_HXX

#include <map>
#include <string>
#include <vector>
#include <iosfwd>

#include "MFront/MFrontConfig.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/LibraryDescription.hxx"
#include "MFront/ExecutableDescription.hxx"
#include "MFront/SpecificTargetDescription.hxx"

namespace mfront {

  /*!
   * \brief structure containing the results of the analysis of a
   * MFront file. This structure is used to build the compilation
   * rules for the libraries and specific targets defined by MFront
   * interfaces.
   */
  struct MFRONT_VISIBILITY_EXPORT TargetsDescription {
    TargetsDescription();
    TargetsDescription(const TargetsDescription&);
    TargetsDescription(TargetsDescription&&);
    ~TargetsDescription();
    /*!
     * \return the library description associated with the given
     * library name or a newly created one if non existed.
     * \param[in] n  : name of the library searched
     * \param[in] pr : library prefix
     * \param[in] s  : library suffix
     * \param[in] t  : library type
     * \note If the the library already exists and the suffix and/or
     * the type does not match, an exception is thrown
     */
    LibraryDescription& getLibrary(const std::string&,
                                   const std::string&,
                                   const std::string&,
                                   const LibraryDescription::LibraryType);
    /*!
     * \return the library description associated with the given
     * library name or a newly created one if non existed.
     * \param[in] n : name of the library searched
     * \param[in] p : library prefix
     * \param[in] s : library suffix
     */
    LibraryDescription& getLibrary(const std::string&,
                                   const std::string&,
                                   const std::string&);
    /*!
     * \return the library description associated with the given
     * library name or a newly created one if non existed.
     * \param[in] n : name of the library searched
     * \param[in] p : library prefix
     */
    LibraryDescription& getLibrary(const std::string&, const std::string&);
    /*!
     * \return the library description associated with the given
     * library name or a newly created one if non existed.
     * \param[in] n : name of the library searched
     */
    LibraryDescription& getLibrary(const std::string&);
    /*!
     * \return the library description associated with the given
     * library name.
     * \param[in] n : name of the library searched
     */
    const LibraryDescription& getLibrary(const std::string&) const;
    //! generated headers
    std::vector<std::string> headers;
    //! target system
#if defined __APPLE__
    LibraryDescription::TargetSystem system = LibraryDescription::MACOSX;
#elif (defined _WIN32 || defined _WIN64)
    LibraryDescription::TargetSystem system = LibraryDescription::WINDOWS;
#elif defined __CYGWIN__
    LibraryDescription::TargetSystem system = LibraryDescription::CYGWIN;
#else
    LibraryDescription::TargetSystem system = LibraryDescription::UNIX;
#endif
    //! default library type
    LibraryDescription::LibraryType libraryType =
        LibraryDescription::SHARED_LIBRARY;
    //! list of libraries to be generated
    std::vector<LibraryDescription> libraries;
    /*!
     * Specific targets and associated dependencies to other targets
     * (for example, mfront generated library) and commands required
     * to build the target.
     *
     * A specific target will define the following Makefile rule:
     * \code{.txt}
     * target : dep1 dep2 src1 src1...
     *    cmd1
     *    cmd2
     *    ....
     * \endcode
     */
    std::map<std::string, SpecificTargetDescription> specific_targets;
  };  // end of struct TargetsDescription

  /*!
   * \return true if the target description describes a library with
   * the given name
   * \param[in] t : target description
   * \param[in] n : library name
   */
  MFRONT_VISIBILITY_EXPORT bool describes(const TargetsDescription&,
                                          const std::string&);
  /*!
   * \brief merge two targets description
   * \param[out] d : destination
   * \param[in]  s : source
   * \param[in]  b : override specific targets
   */
  MFRONT_VISIBILITY_EXPORT void mergeTargetsDescription(
      TargetsDescription&, const TargetsDescription&, const bool);
  /*!
   * \brief write a target description to a stream
   * \param[out] os : output stream
   * \param[in]  t  : target description
   */
  MFRONT_VISIBILITY_EXPORT std::ostream& operator<<(std::ostream&,
                                                    const TargetsDescription&);
  /*!
   * \brief read a TargetDescription from a stream created by the
   * CxxTokenizer class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the target description read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT TargetsDescription
  read(tfel::utilities::CxxTokenizer::const_iterator&,
       const tfel::utilities::CxxTokenizer::const_iterator);

}  // end of namespace mfront

#endif /* LIB_MFRONT_TARGETSDESCRIPTION_HXX */
