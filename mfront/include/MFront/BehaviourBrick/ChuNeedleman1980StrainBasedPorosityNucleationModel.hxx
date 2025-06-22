/*!
 * \file
 * include/MFront/BehaviourBrick/ChuNeedleman1980StrainBasedPorosityNucleationModel.hxx
 * \brief
 * \author Thomas Helfer
 * \date   05/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRAINBASEDPOROSITYNUCLEATIONMODEL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRAINBASEDPOROSITYNUCLEATIONMODEL_HXX

#include "MFront/BehaviourBrick/StrainBasedPorosityNucleationModelBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief strain version of the nucleation model
   * proposed by Chu and Needleman.
   *
   * Chu, C. C., and A. Needleman. 1980.
   * “Void Nucleation Effects in Biaxially Stretched Sheets.”
   * Journal of Engineering Materials and Technology 102 (3): 249–56
   */
  struct ChuNeedleman1980StrainBasedPorosityNucleationModel final
      : StrainBasedPorosityNucleationModelBase {
    //! \brief destructor
    ~ChuNeedleman1980StrainBasedPorosityNucleationModel() override;

   protected:
    std::string getModelName() const override;
    bool requiresLimitOnNucleationPorosity() const override;
    std::vector<MaterialCoefficientDescription>
    getMaterialCoefficientDescriptions() const override;
  };  // end of struct ChuNeedleman1980StrainBasedPorosityNucleationModel

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRAINBASEDPOROSITYNUCLEATIONMODEL_HXX \
        */
