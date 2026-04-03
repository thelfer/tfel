/*!
 * \file   LogarithmicStrainHandler.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27 juin 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Material/LogarithmicStrainHandler.hxx"

namespace tfel::material {

  void LogarithmicStrainHandlerBase::checkLagrangianSetting() const {
    raise_if(this->s != LAGRANGIAN,
             "LogarithmicStrainHandlerBase::"
             "checkLagrangianSetting: "
             "setting is not set to LAGRANGIAN");
  }  // end of LogarithmicStrainHandlerBase::checkLagrangianSetting

  void LogarithmicStrainHandlerBase::checkEulerianSetting() const {
    raise_if(this->s != EULERIAN,
             "LogarithmicStrainHandlerBase::"
             "checkEulerianSetting: "
             "setting is not set to EULERIAN");
  }  // end of LogarithmicStrainHandlerBase::checkEulerianSetting

}  // end of namespace tfel::material
