/*!
 * \file   mtest/include/MTest/PipeFailureCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/09/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPEFAILURECRITERION_HXX
#define LIB_MTEST_PIPEFAILURECRITERION_HXX

#include "MTest/Config.hxx"

namespace mtest {

  //! \brief abstract class for all failure criteria
  struct MTEST_VISIBILITY_EXPORT PipeFailureCriterion {
    //! \brief destructor
    virtual ~PipeFailureCriterion();
  };  // end of PipeFailureCriterion

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEFAILURECRITERION_HXX */
