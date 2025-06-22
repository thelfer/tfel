/*!
 * \file  mfront/src/ModelDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 jun 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/ModelDSL.hxx"
#include "MFront/ModelInterfaceFactory.hxx"

namespace mfront {

  std::string ModelDSL::getName() {
    return "Model";
  }  // end of MaterialPropertyDSL::getName()

  std::string ModelDSL::getDescription() {
    return "this parser is used to define simple material models";
  }  // end of ModelDSL::getDescription

  void ModelDSL::setInterfaces(const std::set<std::string>& f) {
    ModelDSLCommon::setInterfaces(f);
    for (const auto& i : this->interfaces) {
      i.second->declareReservedNames(this->md.getReservedNames());
    }
  }  // end of ModelDSL::setInterfaces

  ModelDescription ModelDSL::getModelDescription() const {
    return this->md;
  }  // end of ModelDSL::getModelDescription

}  // end of namespace mfront
