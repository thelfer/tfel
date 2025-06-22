/*!
 * \file  LibraryDescription.hxx
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

#ifndef LIB_MFRONT_LIBRARYDESCRIPTION_H
#define LIB_MFRONT_LIBRARYDESCRIPTION_H

#include <string>
#include <vector>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/CompiledTargetDescriptionBase.hxx"

namespace mfront {

  /*!
   * \brief This structure is used to build the compilation
   * rules for a shared library or a module.
   */
  struct MFRONT_VISIBILITY_EXPORT LibraryDescription
      : public CompiledTargetDescriptionBase {
    /*!
     * Types of library supported.
     * This difference is only relevant on Mac Os
     */
    enum LibraryType {
      SHARED_LIBRARY,  //<! a shared library meant to be used for linking
      MODULE           //<! a module is meant to be used as a plugin
    };
    /*!
     * \return the default library prefix for the given target system
     * and library type
     * \param[in] s : target system
     * \param[in] t : library type
     */
    static const char* getDefaultLibraryPrefix(const TargetSystem,
                                               const LibraryType) noexcept;
    /*!
     * \return the default library suffix for the given target system
     * and library type
     * \param[in] s : target system
     * \param[in] t : library type
     */
    static const char* getDefaultLibrarySuffix(const TargetSystem,
                                               const LibraryType) noexcept;
    /*!
     * Constructor
     * \param[in] n : name   of the library
     * \param[in] p : prefix of the library
     * \param[in] s : suffix of the library
     * \param[in] t : type   of the library
     */
    LibraryDescription(const std::string&,
                       const std::string&,
                       const std::string&,
                       const LibraryType);
    //! \brief copy constructor
    LibraryDescription(const LibraryDescription&);
    //! \brief move constructor
    LibraryDescription(LibraryDescription&&);
    //! \brief standard assignement (deleted)
    LibraryDescription& operator=(const LibraryDescription&) = delete;
    //! \brief move assignement (deleted)
    LibraryDescription& operator=(LibraryDescription&&) = delete;
    //! \brief destructor
    ~LibraryDescription();
    //! \brief libray type
    const LibraryType type;
    //! \brief generated entry points
    std::vector<std::string> epts;
  };  // end of struct LibraryDescription

  /*!
   * \return a string describing the library type
   * \param[in] t : library type
   */
  MFRONT_VISIBILITY_EXPORT std::string convert(
      const LibraryDescription::LibraryType);
  /*!
   * \brief merge two library description
   * \param[out] d : destination
   * \param[in]  s : source
   */
  MFRONT_VISIBILITY_EXPORT void mergeLibraryDescription(
      LibraryDescription&, const LibraryDescription&);
  /*!
   * \brief write a library description to a stream
   * \param[out] os : output stream
   * \param[in]  t  : library description
   */
  MFRONT_VISIBILITY_EXPORT std::ostream& operator<<(std::ostream&,
                                                    const LibraryDescription&);
  /*!
   * \brief read a LibraryDescription from a stream created by the
   * CxxTokenizer class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the library description read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT LibraryDescription
  read(tfel::utilities::CxxTokenizer::const_iterator&,
       const tfel::utilities::CxxTokenizer::const_iterator);

}  // end of namespace mfront

#endif /* LIB_MFRONT_LIBRARYDESCRIPTION_H */
