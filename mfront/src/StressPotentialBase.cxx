/*!
 * \file   mfront/src/StressPotentialBase.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#include "MFront/BehaviourBrick/StressPotentialBase.hxx"

namespace mfront{

  namespace bbrick {

    StressPotentialBase::StressPotentialBase(AbstractBehaviourDSL& dsl_,
                                             BehaviourDescription& bd_)
        : dsl(dsl_),
          bd(bd_) {}  // end of StressPotentialBase::StressPotentialBase

    StressPotentialBase::~StressPotentialBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront