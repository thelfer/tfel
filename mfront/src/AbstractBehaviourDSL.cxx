/*!
 * \file   mfront/src/AbstractBehaviourDSL.cxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/AbstractBehaviourDSL.hxx"

namespace mfront {

  AbstractDSL::DSLTarget AbstractBehaviourDSL::getTargetType() const {
    return BEHAVIOURDSL;
  }

  AbstractBehaviourDSL::~AbstractBehaviourDSL() = default;

}  // end of namespace mfront
