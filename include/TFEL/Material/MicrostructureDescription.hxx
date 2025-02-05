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
#include <stdexcept>

namespace tfel::material::homogenization::elasticity {

  /*!
   * This struct defines a (2d or 3d) ellipsoid.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths
   */
  template <unsigned short int N, typename real, typename LengthType>
  struct Ellipsoid {
    std::array<LengthType, N> axes_semiLengths;
    std::array<tfel::math::tvector<N, real>, N - 1>
        axes_orientations;  // doivent être perpendiculaires
    Ellipsoid(std::array<LengthType, N> semiLengths,
              std::array<tfel::math::tvector<N, real>, N - 1> orientations)
        : axes_semiLengths(semiLengths),
          axes_orientations(orientations){};
    ~Ellipsoid(){};
  };

  /*!
   * This struct defines a 3d spheroid as a child of 3d Ellipsoid
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths
   */
  template <typename real, typename LengthType>
  struct Spheroid : public Ellipsoid<3u, real, LengthType> {
    // checker que les 2e et 3e éléments de axes_semiLengths sont égaux
  };

  /*!
   * This struct defines a 3d sphere as a child of 3d Ellipsoid
   * \tparam LengthType: type of the radius
   */
  template <typename real, typename LengthType>
  struct Sphere : public Ellipsoid<3u, real, LengthType> {
    // checker que les 3 éléments de axes_semiLengths sont égaux
  };

  /*!
   * This struct defines a 2d disk as a child of a 2d Ellipsoid
   * \tparam LengthType: type of the radius
   */
  template <typename real, typename LengthType>
  struct Disk : public Ellipsoid<2u, real, LengthType> {
    // checker que les 2 éléments de axes_semiLengths sont égaux
  };

  /*!
   * This struct defines a distribution of centers.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   */
  template <unsigned short int N, typename real>
  struct CenterDistribution {};

  /*!
   * This struct defines a distribution of orientations.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   */
  template <unsigned short int N, typename real>
  struct OrientationDistribution {};

  /*!
   * This struct defines a phase. A phase is defined
   * by physical information (stiffness)
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants
   */
  template <unsigned short int N, typename real, typename StressType>
  struct Phase {
    tfel::math::st2tost2<N, StressType> stiffness;
    // check if stiffnessTensor is symetric ?
    Phase(tfel::math::st2tost2<N, StressType> C) : stiffness(C){};
    ~Phase(){};
  };

  /*!
   * This struct defines a matrix phase, which is just an alias of Phase.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related
   * to the inclusion
   */
  template <unsigned short int N, typename real, typename StressType>
  struct MatrixPhase : public Phase<N, real, StressType> {
    MatrixPhase(tfel::math::st2tost2<N, StressType> C)
        : Phase<N, real, StressType>(C){};
    ~MatrixPhase(){};
  };

  /*!
   * This struct defines an inclusion phase as a child of Phase, with a volumic
   * fraction, an object of type Ellipsoid, and information about distribution
   * of centers and orientations.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to
   * the inclusion
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct InclusionPhase : public Phase<N, real, StressType> {
    Ellipsoid<N, real, LengthType> inclusion;
    real fraction;
    // CenterDistribution<N, real> centerDistribution;
    // OrientationDistribution<N, real> orientationDistribution;
    InclusionPhase(tfel::math::st2tost2<N, StressType> C,
                   Ellipsoid<N, real, LengthType> inc,
                   real frac)
        : Phase<N, real, StressType>(C), inclusion(inc), fraction(frac){};
    ~InclusionPhase(){};
  };

  /*!
   * just an alias of InclusionPhase
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct Grain : public InclusionPhase<N, real, LengthType, StressType> {
    Grain(tfel::math::st2tost2<N, StressType> C,
          Ellipsoid<N, real, LengthType> inc,
          real frac)
        : InclusionPhase<N, real, LengthType, StressType>(C, inc, frac){};
    ~Grain(){};
  };

  /*!
   * This struct defines a microstructure.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to the inclusion
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct Microstructure {};

  /*!
   * This struct defines a matrix-inclusion microstructure as a child of
   * Microstructure. This type of microstructure is viewed as an array of
   * InclusionPhase, with a matrixPhase.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to the inclusion
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct MatrixInclusionMicrostructure
      : public Microstructure<N, real, LengthType, StressType> {
    MatrixInclusionMicrostructure(MatrixPhase<N, real, StressType> matrix)
        : Microstructure<N, real, LengthType, StressType>(),
          number_of_phases(1),
          matrix_fraction(1),
          inclusionPhases({}),
          matrixPhase(matrix){};

    ~MatrixInclusionMicrostructure(){};

    int addInclusionPhase(
        InclusionPhase<N, real, LengthType, StressType> inclusionPhase) {
      if (this->matrix_fraction - inclusionPhase.fraction < real(0)) {
        std::cout << "the volume fraction of inclusions is too high !"
                  << std::endl;
        return 0;
      } else {
        (this->number_of_phases)++;
        (this->matrix_fraction) -= inclusionPhase.fraction;
        (this->inclusionPhases).push_back(inclusionPhase);
        return 1;
      };
    };

    int removeInclusionPhase(unsigned int i) {
      if ((this->number_of_phases) == 1) {
        std::cout << "there are no more inclusions !" << std::endl;
        return 0;
      } else if ((this->number_of_phases) < i + 2) {
        std::cout << "there are less phases than what you think !" << std::endl;
        return 0;
      } else {
        if ((this->number_of_phases) == 2) {
          std::cout << "you have removed the last inclusion phase !"
                    << std::endl;
        };
        (this->number_of_phases)--;
        (this->matrix_fraction) -= (this->inclusionPhases[i]).fraction;
        (this->inclusionPhases).erase((this->inclusionPhases).begin() + i);
        return 1;
      };
    };

    unsigned int get_number_of_phases() { return (this->number_of_phases); };

    real get_matrix_fraction() { return (this->matrix_fraction); };

    std::vector<InclusionPhase<N, real, LengthType, StressType>>
    get_inclusionPhases() {
      return (this->inclusionPhases);
    };

    MatrixPhase<N, real, StressType> get_matrixPhase() {
      return (this->matrixPhase);
    };

    InclusionPhase<N, real, LengthType, StressType> get_inclusionPhase(
        unsigned int i) {
      if ((this->number_of_phases) < i + 2) {
        tfel::reportContractViolation(
            "there are less phases than what you think !");
      }
      return ((this->inclusionPhases)[i]);
    };
    // stiffnessDistribution ?

   private:
    unsigned int number_of_phases;
    real matrix_fraction;
    std::vector<InclusionPhase<N, real, LengthType, StressType>>
        inclusionPhases;
    MatrixPhase<N, real, StressType> matrixPhase;
  };

  /*!
   * This struct defines a polycrystal as a child of Microstructure. A
   * polycrystal is viewed as a vector of Grain.
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to the inclusion
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct Polycrystal : public Microstructure<N, real, LengthType, StressType> {
    Polycrystal()
        : Microstructure<N, real, LengthType, StressType>(),
          number_of_grains(0),
          total_fraction(real(0)),
          grains({}){};

    ~Polycrystal(){};

    int addGrain(Grain<N, real, LengthType, StressType> grain) {
      if ((this->total_fraction) + grain.fraction > real(1)) {
        std::cout << "the volume fraction is too high !" << std::endl;
        return 0;
      } else {
        (this->number_of_grains)++;
        (this->total_fraction) += grain.fraction;
        (this->grains).push_back(grain);
        return 1;
      };
    };

    int removeGrain(unsigned int i) {
      if ((this->number_of_grains) == 0) {
        std::cout << "there are no more grains !" << std::endl;
        return 0;
      } else if ((this->number_of_grains) < i + 1) {
        std::cout << "there are less grains than what you think !" << std::endl;
        return 0;
      } else {
        if ((this->number_of_grains) == 1) {
          std::cout << "you have removed the last grain !" << std::endl;
        };
        (this->number_of_grains)--;
        (this->total_fraction) -= (this->grains[i]).fraction;
        (this->grains).erase((this->grains).begin() + i);  // TOOODDOOOO
      };
      return 1;
    };

    unsigned int get_number_of_grains() { return (this->number_of_grains); };

    real get_total_fraction() { return (this->total_fraction); };

    std::vector<Grain<N, real, LengthType, StressType>> get_grains() {
      return (this->grains);
    };

    Grain<N, real, LengthType, StressType> get_grain(unsigned int i) {
      if ((this->number_of_grains) < i + 1) {
        tfel::reportContractViolation(
            "there are less phases than what you think !");
      }
      return ((this->grains)[i]);
    };

   private:
    unsigned int number_of_grains;
    real total_fraction;
    std::vector<Grain<N, real, LengthType, StressType>> grains;
  };
  // conditions sur la centerDistribution et sur orientationDistribution
  // stiffnessDistribution ??

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX */
