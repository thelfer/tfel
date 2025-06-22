/*!
 * \file   mfront/include/MFront/SpecificTargetDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13/06/2019
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
    SpecificTargetDescription(SpecificTargetDescription&&);
    //! \brief standard assignement (deleted)
    SpecificTargetDescription& operator=(const SpecificTargetDescription&);
    //! \brief move assignement (deleted)
    SpecificTargetDescription& operator=(SpecificTargetDescription&&);
    //! \brief destructor
    ~SpecificTargetDescription();
    // //! \brief  name
    // const std::string name;
    //! \brief sources
    std::vector<std::string> sources;
    //!\ brief  dependencies to other targets
    std::vector<std::string> deps;
    //! \brief commands
    std::vector<std::string> cmds;
  };  // end of struct SpecificTargetDescription

  /*!
   * \brief write a library description to a stream
   * \param[out] os : output stream
   * \param[in]  t  : library description
   */
  MFRONT_VISIBILITY_EXPORT std::ostream& operator<<(
      std::ostream&, const SpecificTargetDescription&);
  /*!
   * \brief read a SpecificTargetDescription from a stream created by the
   * CxxTokenizer class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the library description read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT SpecificTargetDescription
  read(tfel::utilities::CxxTokenizer::const_iterator&,
       const tfel::utilities::CxxTokenizer::const_iterator);

}  // end of namespace mfront

#endif /* LIB_MFRONT_SPECIFICTARGETDESCRIPTION_HXX */
