/*!
 * \file  CompiledTargetDescriptionBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
linking exception
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
X */

#ifndef LIB_MFRONT_COMPILEDTARGETDESCRIPTIONBASE_HXX
#define LIB_MFRONT_COMPILEDTARGETDESCRIPTIONBASE_HXX

#include <string>
#include <vector>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/MFrontUtilities.hxx"

namespace mfront {

  /*!
   * \brief This structure is used to build the compilation
   * rules for an compiled target.
   * - the name
   * - the generated sources
   * - the required preprocessor flags
   * - the linker flags of the generated executable
   * - the generated entry points (function or class names)
   */
  struct MFRONT_VISIBILITY_EXPORT CompiledTargetDescriptionBase {
    /*!
     * Supported target systems
     */
    enum TargetSystem {
      WINDOWS,  //<! Microsoft windows system
      CYGWIN,   //<! Cywgin environment
      MACOSX,   //<! Apple Mac Os
      UNIX      //<! General unix flavor (Linux, FreeBSD, Solaris, ...)
    };
    /*!
     * Constructor
     * \param[in] n : name   of the executable
     * \param[in] p : prefix of the executable
     * \param[in] s : suffix of the executable
     * \param[in] t : type   of the executable
     */
    CompiledTargetDescriptionBase(const std::string&,
                                  const std::string&,
                                  const std::string&);
    //! \brief copy constructor
    CompiledTargetDescriptionBase(const CompiledTargetDescriptionBase&);
    //! \brief move constructor
    CompiledTargetDescriptionBase(CompiledTargetDescriptionBase&&);
    //! \brief standard assignement (deleted)
    CompiledTargetDescriptionBase& operator=(
        const CompiledTargetDescriptionBase&) = delete;
    //! \brief move assignement (deleted)
    CompiledTargetDescriptionBase& operator=(CompiledTargetDescriptionBase&&) =
        delete;
    //! \brief destructor
    ~CompiledTargetDescriptionBase();
    //! \brief name
    const std::string name;
    //! \brief suffix
    const std::string prefix;
    //! \brief suffix
    const std::string suffix;
    //! \brief list of mfront files used to generate the C++ sources
    std::vector<std::string> mfront_sources;
    //! \brief generated sources
    std::vector<std::string> sources;
    //! \brief additional preprocessor flags
    std::vector<std::string> cppflags;
    //! \brief path to the libraries to be linked
    std::vector<std::string> include_directories;
    //! \brief path to the libraries to be linked
    std::vector<std::string> link_directories;
    //! \brief libraries to be linked
    std::vector<std::string> link_libraries;
    //! \brief dependencies to other generated libraries
    std::vector<std::string> deps;
    //! \brief the linker flags
    std::vector<std::string> ldflags;
    //! \brief installation path
    std::string install_path;
  };  // end of struct CompiledTargetDescriptionBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_COMPILEDTARGETDESCRIPTIONBASE_HXX */
