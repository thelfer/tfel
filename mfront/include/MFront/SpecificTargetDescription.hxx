/*!
 * \file   mfront/include/MFront/SpecificTargetDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date 13/06/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_SPECIFICTARGETDESCRIPTION_HXX
#define LIB_MFRONT_SPECIFICTARGETDESCRIPTION_HXX

#include <string>
#include <vector>
#include "MFront/MFrontConfig.hxx"
#include "MFront/DSLUtilities.hxx"

namespace mfront {

  /*!
   * A specific target will define the following Makefile rule:
   * \code{.txt}
   * target : dep1 dep2 src1 src1...
   *    cmd1
   *    cmd2
   *    ....
   * \endcode
   */
  struct MFRONT_VISIBILITY_EXPORT SpecificTargetDescription {
    //! \brief default constructor
    SpecificTargetDescription();
    //! \brief copy constructor
    SpecificTargetDescription(const SpecificTargetDescription&);
    //! \brief move constructor
    SpecificTargetDescription(SpecificTargetDescription&&) noexcept;
    //! \brief standard assignement (deleted)
    SpecificTargetDescription& operator=(const SpecificTargetDescription&);
    //! \brief move assignement (deleted)
    SpecificTargetDescription& operator=(SpecificTargetDescription&&) noexcept;
    //! \brief destructor
    ~SpecificTargetDescription();
    //! \brief sources
    std::vector<std::string> sources;
    //!\ brief dependencies to other targets
    std::vector<std::string> deps;
    //!\ brief dependencies to MFront generated libraries
    std::vector<std::string> libraries;
    //! \brief commands
    std::vector<std::string> cmds;
  };  // end of struct SpecificTargetDescription

}  // end of namespace mfront

#endif /* LIB_MFRONT_SPECIFICTARGETDESCRIPTION_HXX */
