/*!
 * \file
 * include/MFront/BehaviourBrick/ChuNeedleman1980StressBasedPorosityNucleationModel.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRESSBASEDPOROSITYNUCLEATIONMODEL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRESSBASEDPOROSITYNUCLEATIONMODEL_HXX

#include "MFront/BehaviourBrick/StressBasedPorosityNucleationModelBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief strain version of the nucleation model
     * proposed by Chu and Needleman.
     *
     * Chu, C. C., and A. Needleman. 1980.
     * “Void Nucleation Effects in Biaxially Stretched Sheets.”
     * Journal of Engineering Materials and Technology 102 (3): 249–56
     */
    struct ChuNeedleman1980StressBasedPorosityNucleationModel final
        : StressBasedPorosityNucleationModelBase {
      //! \brief destructor
      ~ChuNeedleman1980StressBasedPorosityNucleationModel() override;

     protected:
      std::string getModelName() const override final;
      std::vector<MaterialCoefficientDescription>
      getMaterialCoefficientDescriptions() const override final;
    };  // end of struct ChuNeedleman1980StressBasedPorosityNucleationModel

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_CHUNEEDLEMAN1980STRESSBASEDPOROSITYNUCLEATIONMODEL_HXX \
        */
