/*!
 * \file  ExecutableDescription.hxx
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

#ifndef LIB_MFRONT_EXECUTABLEDESCRIPTION_HXX
#define LIB_MFRONT_EXECUTABLEDESCRIPTION_HXX

#include <string>
#include <vector>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/CompiledTargetDescriptionBase.hxx"

namespace mfront {

  /*!
   * \brief This structure is used to build the compilation
   * rules for an executable.
   */
  struct MFRONT_VISIBILITY_EXPORT ExecutableDescription
      : public CompiledTargetDescriptionBase {
    /*!
     * \return the default executable prefix for the given target system
     * and executable type
     * \param[in] s : target system
     */
    static const char* getDefaultExecutablePrefix(const TargetSystem) noexcept;
    /*!
     * \return the default executable suffix for the given target system
     * and executable type
     * \param[in] s : target system
     */
    static const char* getDefaultExecutableSuffix(const TargetSystem) noexcept;
    /*!
     * Constructor
     * \param[in] n : name   of the executable
     * \param[in] p : prefix of the executable
     * \param[in] s : suffix of the executable
     */
    ExecutableDescription(const std::string&,
                          const std::string&,
                          const std::string&);
    //! \brief copy constructor
    ExecutableDescription(const ExecutableDescription&);
    //! \brief move constructor
    ExecutableDescription(ExecutableDescription&&);
    //! \brief standard assignement (deleted)
    ExecutableDescription& operator=(const ExecutableDescription&) = delete;
    //! \brief move assignement (deleted)
    ExecutableDescription& operator=(ExecutableDescription&&) = delete;
    //! \brief destructor
    ~ExecutableDescription();
  };  // end of struct ExecutableDescription

  /*!
   * \brief merge two executable description
   * \param[out] d : destination
   * \param[in]  s : source
   */
  MFRONT_VISIBILITY_EXPORT void mergeExecutableDescription(
      ExecutableDescription&, const ExecutableDescription&);
  /*!
   * \brief write a executable description to a stream
   * \param[out] os : output stream
   * \param[in]  t  : executable description
   */
  MFRONT_VISIBILITY_EXPORT std::ostream& operator<<(
      std::ostream&, const ExecutableDescription&);
  /*!
   * \brief read a ExecutableDescription from a stream created by the
   * CxxTokenizer class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the executable description read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT ExecutableDescription
  read(tfel::utilities::CxxTokenizer::const_iterator&,
       const tfel::utilities::CxxTokenizer::const_iterator);

}  // end of namespace mfront

#endif /* LIB_MFRONT_EXECUTABLEDESCRIPTION_HXX */
