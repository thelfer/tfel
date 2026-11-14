/*!
 * \file   mfront/src/PorosityNucleationModel.cxx
 * \brief
 * \author Thomas Helfer
 * \date 15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/PorosityNucleationModel.hxx"

namespace mfront::bbrick {

  std::string PorosityNucleationModel::getVariableId(const std::string& n,
                                                     const std::string& id) {
    return "pnm_" + n + id;
  }  // end of getVariableId

  PorosityNucleationModel::~PorosityNucleationModel() = default;

}  // end of namespace mfront::bbrick
