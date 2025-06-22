/*!
 * \file
 * include/MFront/BehaviourBrick/PowerLawStressBasedPorosityNucleationModel.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_POWERLAWSTRESSBASEDPOROSITYNUCLEATIONMODEL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_POWERLAWSTRESSBASEDPOROSITYNUCLEATIONMODEL_HXX

#include "MFront/BehaviourBrick/StressBasedPorosityNucleationModelBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief stress version of the power law nucleation model
     */
    struct PowerLawStressBasedPorosityNucleationModel final
        : StressBasedPorosityNucleationModelBase {
      //! \brief destructor
      ~PowerLawStressBasedPorosityNucleationModel() override;

     protected:
      std::string getModelName() const override final;
      std::vector<MaterialCoefficientDescription>
      getMaterialCoefficientDescriptions() const override final;
    };  // end of struct PowerLawStressBasedPorosityNucleationModel

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_POWERLAWSTRESSBASEDPOROSITYNUCLEATIONMODEL_HXX \
        */
