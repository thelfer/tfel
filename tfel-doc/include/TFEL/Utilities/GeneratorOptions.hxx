/*!
 * \file   tfel-doc/include/TFEL/Utilities/GeneratorOptions.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_GENERATOROPTIONS_HXX
#define LIB_TFEL_UTILITIES_GENERATOROPTIONS_HXX

namespace tfel {

  namespace utilities {

    /*!
     * a structure holding generator options
     */
    struct GeneratorOptions {
      //! install path (i.e. where to find the tests files)
      std::string prefix;
      //! basename where individual files shall be created (used if
      //! split is true)
      std::string outputDirectory;
      //! output language
      std::string lang;
      //! build a stand-alone document
      bool standAlone = true;
      //! generate one output file by category
      bool split = false;
    };  // end of struct GeneratorOptions

  }  // end of namespace utilities

}  // end of namespace tfel

#endif /* LIB_TFEL_UTILITIES_GENERATOROPTIONS_HXX */
