/*!
 * \file   mfront/include/MFront/GeneratorOptions.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   16 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERATOROPTIONS_HXX
#define LIB_MFRONT_GENERATOROPTIONS_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * a data structure holding options passed to generators
   */
  struct MFRONT_VISIBILITY_EXPORT GeneratorOptions {
    /*!
     * \brief a simple enumeration describing the optimisation level.
     * \note See the `tfel-config` utility for more details
     */
    enum OptimisationLevel {
      LEVEL0,  //!< no architecture specific optimisations (i.e. no
               //   `-march=native` under gcc). The options used are
               //   the ones returned by `tfel-config --oflags0`.
      LEVEL1,  //!< standard optimisation level. The options used are
               //   the ones returned by `tfel-config --oflags`.
      LEVEL2   //!< add potentially unsafe math optmisations. The
               //   options used are the ones returned by
               //   `tfel-config --oflags --oflags2`.
    };
    //! \brief default constructor
    GeneratorOptions();
    /*!
     * \brief copy constructor
     * \param[in] src: source
     */
    GeneratorOptions(const GeneratorOptions&);
    /*!
     * \brief move constructor
     * \param[in] src: source
     */
    GeneratorOptions(GeneratorOptions&&);
    /*!
     * \brief copy assignement
     * \param[in] src: source
     */
    GeneratorOptions& operator=(const GeneratorOptions&);
    /*!
     * \brief move assignement
     * \param[in] src: source
     */
    GeneratorOptions& operator=(GeneratorOptions&&);
    //! \brief destructor
    ~GeneratorOptions();
    //! targeted operating system
#if ((defined(_WIN32) || defined(_WIN64)) && (!defined(__CYGWIN__)))
    std::string sys = "win32";
#elif defined __CYGWIN__
    std::string sys = "cygwin";
#elif defined __APPLE__
    std::string sys = "apple";
#else
    std::string sys = "default";
#endif /* __CYGWIN__ */
    //! \brief optimisation level
    OptimisationLevel olevel = LEVEL1;
    /*!
     * \brief boolean stating that no output regarding the compilation
     * commands shall be displayed.
     */
    bool silentBuild = true;
#if (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    bool nodeps = true;
#else  /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    bool nodeps = false;
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    bool melt = true;
  };  // end of struct GeneratorOptions

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERATOROPTIONS_HXX */
