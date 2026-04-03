/*!
 * \file   mtest/include/MTest/PipeFailureCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/09/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPEFAILURECRITERION_HXX
#define LIB_MTEST_PIPEFAILURECRITERION_HXX

#include <string>
#include "MTest/Types.hxx"
#include "MTest/Config.hxx"

namespace mtest {

  // forward declaration
  struct StudyCurrentState;

  //! \brief abstract class for all failure criteria
  struct MTEST_VISIBILITY_EXPORT PipeFailureCriterion {
    //! \return the name of failure criterion
    virtual std::string getName() const = 0;
    /*!
     * \brief evaluate the failure criterion
     * \return if the pipe is broken
     * \param[in] ti: time at the beginning of the time step
     * \param[in] te: time at the beginning of the end step
     */
    virtual bool execute(const StudyCurrentState&,
                         const real,
                         const real) const = 0;
    //! \brief destructor
    virtual ~PipeFailureCriterion();
  };  // end of PipeFailureCriterion

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEFAILURECRITERION_HXX */
