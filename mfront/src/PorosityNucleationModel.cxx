/*!
 * \file   mfront/src/PorosityNucleationModel.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "MFront/BehaviourBrick/PorosityNucleationModel.hxx"

namespace mfront::bbrick {

  std::string PorosityNucleationModel::getVariableId(const std::string& n,
                                                     const std::string& id) {
    return "pnm_" + n + id;
  }  // end of getVariableId

  PorosityNucleationModel::~PorosityNucleationModel() = default;

}  // end of namespace mfront::bbrick
