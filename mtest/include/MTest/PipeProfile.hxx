/*!
 * \file   PipeProfile.hxx
 * \brief
 * \author Thomas Helfer
 * \date   14 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPEPROFILE_HXX
#define LIB_MTEST_PIPEPROFILE_HXX

#include <iosfwd>
#include "MTest/PipeProfile.hxx"

namespace mtest {

  // forward declaration
  struct CurrentState;

  /*!
   * structure in charge of reporting strain, stresses or internal
   * state variables profiles.
   */
  struct PipeProfile {
    /*!
     * put the requested value(s) in the output stream
     * \param[out] os: output stream
     * \param[in]  s:  current state
     */
    virtual void report(std::ostream&, const CurrentState&) const = 0;
    //! destructor
    virtual ~PipeProfile();
  };

  /*!
   * structure in charge of reporting stresses.
   */
  struct PipeStressProfile : public PipeProfile {
    //! default constructor
    PipeStressProfile();
    /*!
     * \brief constructor
     * \param[in] cn: component
     */
    PipeStressProfile(const unsigned short);
    /*!
     * put the requested value(s) in the output stream
     * \param[out] os: output stream
     * \param[in]  s:  current state
     */
    virtual void report(std::ostream&, const CurrentState&) const override;
    //! destructor
    virtual ~PipeStressProfile();

   private:
    bool all;
    unsigned short c;
  };

  /*!
   * structure in charge of reporting strains.
   */
  struct PipeStrainProfile : public PipeProfile {
    //! default constructor
    PipeStrainProfile();
    /*!
     * \brief constructor
     * \param[in] cn: component
     */
    PipeStrainProfile(const unsigned short);
    /*!
     * put the requested value(s) in the output stream
     * \param[out] os: output stream
     * \param[in]  s:  current state
     */
    virtual void report(std::ostream&, const CurrentState&) const override;
    //! destructor
    virtual ~PipeStrainProfile();

   private:
    bool all;
    unsigned short c;
  };

  /*!
   * structure in charge of reporting internal state variables.
   */
  struct PipeInternalStateVariableProfile : public PipeProfile {
    //! default constructor
    PipeInternalStateVariableProfile();
    /*!
     * \brief constructor
     * \param[in] cn: component
     */
    PipeInternalStateVariableProfile(const unsigned short);
    /*!
     * put the requested value(s) in the output stream
     * \param[out] os: output stream
     * \param[in]  s:  current state
     */
    virtual void report(std::ostream&, const CurrentState&) const override;
    //! destructor
    virtual ~PipeInternalStateVariableProfile();

   private:
    bool all;
    unsigned short c;
  };

}  // namespace mtest

#endif /* LIB_MTEST_PIPEPROFILE_HXX */
