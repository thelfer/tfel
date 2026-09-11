/*!
 * \file   include/TFEL/Material/MicrostructureDescription.hxx
 * \author Antoine Martin
 * \date   25 January 2025
 * \brief  This file defines some objects to describe a microstructure.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX
#define LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/WalpoleBasis.hxx"
#include "TFEL/Material/OrientationAverages.hxx"
#include "TFEL/Material/LocalisationTensor.hxx"
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"
#include "TFEL/Material/PhaseDescription.hxx"
#include <stdexcept>
#include <memory>

namespace tfel::material::homogenization::elasticity {

  ////Definition of 'Microstructure' objects

  /*!
   * This struct represents a microstructure.
   * \tparam unsigned short int: dimension
   * \tparam StressType: type of the elastic constants related to the
   * inclusion
   */
  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct Microstructure {
    Microstructure() = default;
  };

  /*!
   * This struct defines a matrix-inclusion microstructure (or particulate
   * microstructure) as a child of Microstructure. This type of microstructure
   * is viewed as an array of InclusionDistribution, with a matrix_phase.
   * \tparam unsigned short int: dimension
   * \tparam StressType: type of the elastic constants related to the
   * inclusion
   */
  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) struct ParticulateMicrostructure
      : public Microstructure<N, StressType> {
    // attributes below
    using real = tfel::types::real<StressType>;
    using LengthType = tfel::types::length<StressType>;

    ParticulateMicrostructure() = default;

    explicit ParticulateMicrostructure(
        const tfel::math::st2tost2<N, StressType>& C0)
        : Microstructure<N, StressType>(),
          number_of_phases(1),
          inclusion_phases{},
          matrix_phase(Phase<N, StressType>(real(1), C0)) {}

    explicit ParticulateMicrostructure(const IsotropicModuli<StressType>& IM0)
        : Microstructure<N, StressType>(),
          number_of_phases(1),
          inclusion_phases{},
          matrix_phase(Phase<N, StressType>(real(1), IM0)) {}

    void changeElasticityOfMatrixPhase(
        const tfel::math::st2tost2<N, StressType>& C0) {
      (this->matrix_phase).changeElasticityOfPhase(C0);
    }

    void changeElasticityOfMatrixPhase(const IsotropicModuli<StressType>& IM0) {
      (this->matrix_phase).changeElasticityOfPhase(IM0);
    }

    [[nodiscard]] tfel::math::st2tost2<N, StressType> get_matrix_elasticity() {
      return (this->matrix_phase.getElasticityOfPhase());
    }

    // this method has the right convention name
    [[nodiscard]] tfel::math::st2tost2<N, StressType> getMatrixElasticity() {
      return (this->matrix_phase.getElasticityOfPhase());
    }

    [[nodiscard]] bool is_isotropic_matrix() const {
      return (this->matrix_phase.isIsotropic());
    }
    [[nodiscard]] bool isIsotropicMatrix() const {
      return (this->matrix_phase.isIsotropic());
    }
    [[nodiscard]] real get_matrix_fraction() {
      return (this->matrix_phase.fraction);
    }
    [[nodiscard]] real getMatrixFraction() {
      return (this->matrix_phase.fraction);
    }

    [[nodiscard]] int addInclusionPhase(
        InclusionDistribution<N, StressType>& inclusionPhase) {
      if (this->matrix_phase.fraction - inclusionPhase.fraction < real(0)) {
        //           std::cout << "the volume fraction of inclusions is too
        //           high !"
        //                     << std::endl;
        return 0;
      } else {
        (this->number_of_phases)++;
        (this->matrix_phase.fraction) -= inclusionPhase.fraction;
        (this->inclusion_phases).push_back(inclusionPhase.clone());
        return 1;
      }
    }

    [[nodiscard]] std::unique_ptr<InclusionDistribution<N, StressType>>
    get_inclusionPhase(unsigned int i) {
      if ((this->number_of_phases) < i + 2) {
        tfel::reportContractViolation(
            "there are less phases than what you think !");
      }
      return ((this->inclusion_phases)[i])->clone();
    }

    // the method with the right convention name
    [[nodiscard]] std::unique_ptr<InclusionDistribution<N, StressType>>
    getInclusionPhase(unsigned int i) {
      if ((this->number_of_phases) < i + 2) {
        tfel::reportContractViolation(
            "there are less phases than what you think !");
      }
      return ((this->inclusion_phases)[i])->clone();
    }

    [[nodiscard]] int removeInclusionPhase(unsigned int i) {
      if ((this->number_of_phases) == 1) {
        //          std::cout << "there are no more inclusions !" <<
        //          std::endl;
        return 0;
      } else if ((this->number_of_phases) < i + 2) {
        //           std::cout << "there are less phases than what you think
        //           !"
        //                     << std::endl;
        return 0;
      } else {
        // if ((this->number_of_phases) == 2) {
        //              std::cout << "you have removed the last inclusion
        //              phase !"
        //                        << std::endl;
        //}
        (this->number_of_phases)--;
        (this->matrix_phase.fraction) +=
            (*(this->inclusion_phases[i])).fraction;
        (this->inclusion_phases).erase((this->inclusion_phases).begin() + i);
        return 1;
      }
    }

    [[nodiscard]] int changeElasticityOfInclusionPhase(
        unsigned int i, const tfel::math::st2tost2<N, StressType>& C) {
      if ((this->number_of_phases) == 1) {
        return 0;
      } else if ((this->number_of_phases) < i + 2) {
        return 0;
      } else {
        (*((this->inclusion_phases)[i])).changeElasticityOfPhase(C);
        return 1;
      }
    }

    [[nodiscard]] int changeElasticityOfInclusionPhase(
        unsigned int i, const IsotropicModuli<StressType>& IM) {
      if ((this->number_of_phases) == 1) {
        return 0;
      } else if ((this->number_of_phases) < i + 2) {
        return 0;
      } else {
        (*((this->inclusion_phases)[i])).changeElasticityOfPhase(IM);
        return 1;
      }
    }

    [[nodiscard]] int changeFractionOfInclusionPhase(unsigned int i,
                                                     const real f) {
      if ((this->number_of_phases) == 1) {
        return 0;
      } else if ((this->number_of_phases) < i + 2) {
        return 0;
      } else if (this->matrix_phase.fraction +
                     (*(this->inclusion_phases[i])).fraction - f <
                 real(0)) {
        return 0;
      } else {
        (this->matrix_phase.fraction) +=
            (*(this->inclusion_phases[i])).fraction;
        (this->matrix_phase.fraction) -= f;
        (*((this->inclusion_phases)[i])).fraction = f;
        return 1;
      }
    }

    [[nodiscard]] unsigned int get_number_of_phases() {
      return (this->number_of_phases);
    }
    [[nodiscard]] unsigned int getNumberOfPhases() {
      return (this->number_of_phases);
    }

   private:
    unsigned int number_of_phases;
    std::vector<std::unique_ptr<InclusionDistribution<N, StressType>>>
        inclusion_phases;
    Phase<N, StressType> matrix_phase;

  };  // end of ParticulateMicrostructure

  /*!
   * This struct defines a polycrystalline microstructure
   * as a child of Microstructure. This type of microstructure
   * is viewed as an array of Grains.
   * \tparam StressType: type of the elastic constants related to the
   * grains
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) struct Polycrystal
      : public Microstructure<3u, StressType> {
    // attributes below
    using real = tfel::types::real<StressType>;

    Polycrystal()
        : Microstructure<3u, StressType>(), number_of_grains(0), total_fraction(0), grains{} {}

    [[nodiscard]] int addGrain(Grain<StressType>& grain) {
      if (this->total_fraction + grain.fraction > real(1)) {
        return 0;
      } else {
        (this->number_of_grains)++;
        (this->total_fraction) += grain.fraction;
        (this->grains).push_back(grain.clone());
        return 1;
      }
    }

    [[nodiscard]] std::unique_ptr<Grain<StressType>> getGrain(unsigned int i) {
      if ((this->number_of_grains) < i + 1) {
        tfel::reportContractViolation(
            "there are less grains than what you think !");
      }
      return ((this->grains)[i])->clone();
    }

    [[nodiscard]] int removeGrain(unsigned int i) {
      if ((this->number_of_grains) == 0) {
        return 0;
      } else if ((this->number_of_grains) < i + 1) {
        return 0;
      } else {
        (this->number_of_grains)--;
        (this->total_fraction) -= (*(this->grains[i])).fraction;
        (this->grains).erase((this->grains).begin() + i);
        return 1;
      }
    }

    [[nodiscard]] int changeElasticityOfGrain(
        unsigned int i, const IsotropicModuli<StressType>& IM) {
      if ((this->number_of_grains) == 0) {
        return 0;
      } else if ((this->number_of_grains) < i + 1) {
        return 0;
      } else {
        (*((this->grains)[i])).changeElasticityOfPhase(IM);
        return 1;
      }
    }

    [[nodiscard]] int changeFractionOfGrain(unsigned int i, const real f) {
      if ((this->number_of_grains) == 0) {
        return 0;
      } else if ((this->number_of_grains) < i + 1) {
        return 0;
      } else {
        (this->total_fraction) -= (*(this->grains[i])).fraction;
        (this->total_fraction) += f;
        (*((this->grains)[i])).fraction = f;
        return 1;
      }
    }

    [[nodiscard]] unsigned int getTotalFraction() {
      return (this->total_fraction);
    }

    [[nodiscard]] unsigned int getNumberOfGrains() {
      return (this->number_of_grains);
    }

   private:
    unsigned int number_of_grains;
    real total_fraction;
    std::vector<std::unique_ptr<Grain<StressType>>> grains;
  };  // end of Polycrystal

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX */
