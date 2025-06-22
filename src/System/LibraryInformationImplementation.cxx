/*!
 * \file   LibraryInformationImplementation.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2017
 * \note This file contains code that has been extracted from the
 * boost/dll library version 1.63 and has been originally written by
 * Antony Polukhin, Renato Tegon Forti and Antony Polukhin.
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

namespace tfel {

  namespace system {

    struct LibraryInformation::LibraryInformation::Implementation {
      virtual std::vector<std::string> sections() = 0;
      virtual std::vector<std::string> symbols() = 0;
      virtual std::vector<std::string> symbols(const char*) = 0;
      virtual ~Implementation();
    };  // end of struct LibraryInformation::LibraryInformation::Implementation

  }  // end of namespace system

}  // end of namespace tfel
