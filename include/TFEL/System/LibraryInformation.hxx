/*!
 * \file   LibraryInformation.hxx
 * \brief A class for querying information about a library.
 * \author Thomas Helfer
 * \date   28 mars 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * \note This file contains code that has been extracted from the
 * boost/dll library version 1.63 and has been originally written by
 * Antony Polukhin, Renato Tegon Forti and Antony Polukhin.
 */

#ifndef LIB_TFEL_SYSTEM_LIBRARYINFORMATION_HXX
#define LIB_TFEL_SYSTEM_LIBRARYINFORMATION_HXX

#include <vector>
#include <string>
#include <memory>
#include <iosfwd>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace system {

    /*!
     * \brief class that is capable of extracting different
     * information from a library or binary file.
     * Currently understands ELF, MACH-O and PE formats on all the
     * platforms.
     *
     * \note this class has been extracted from the boost/dll library
     * version 1.63 and has been originally written by Antony
     * Polukhin, Renato Tegon Forti and Antony Polukhin.
     */
    struct TFELSYSTEM_VISIBILITY_EXPORT LibraryInformation {
      //! structure used to implement the PImpl idiom.
      struct Implementation;
      /*!
       * \brief opens file with specified path and prepares for
       * information extraction.
       * \param[in] f: path to the binary file from which the info
       * must be extracted.
       * \param[in] b: throw an exception if this file format is not
       * supported by OS.
       */
      explicit LibraryInformation(const std::string&, const bool = true);
      //! copy constructor
      LibraryInformation(const LibraryInformation&);
      //! move constructor
      LibraryInformation(LibraryInformation&&);
      //! standard assignement
      LibraryInformation& operator=(const LibraryInformation&);
      //! move constructor
      LibraryInformation& operator=(LibraryInformation&&);
      /*!
       * \return List of sections that exist in binary file.
       */
      std::vector<std::string> sections();
      /*!
       * \return List of all the exportable symbols from all the
       * sections that exist in binary file.
       */
      std::vector<std::string> symbols();
      /*!
       * \param[in] n: name of the section from which symbol names
       * must be returned.
       * \return The list of symbols from the specified section.
       */
      std::vector<std::string> symbols(const char*);
      /*!
       * \param[in] n: name of the section from which symbol names
       * must be returned.
       * \return The list of symbols from the specified section.
       */
      std::vector<std::string> symbols(const std::string&);
      //! \brief destructor
      ~LibraryInformation() noexcept;

     private:
      //! pointer to the internal structure
      std::shared_ptr<Implementation> impl;
    };  // end of struct LibraryInformation

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_TFEL_SYSTEM_LIBRARYINFORMATION_HXX */
