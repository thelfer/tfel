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
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"
#include <stdexcept>

namespace tfel::material {

  namespace homogenization::elasticity {

    ////Definition of 'Inclusion' objects

    /*!
     * This struct represents a N (N=2 or N=3) dimensional inclusion (its childs
     * are only geometrical ellipsoids, which can be particular like spheres,
     * disks...)
     */
    template <unsigned short int N, tfel::math::ScalarConcept LengthType>
    requires(
        tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                           LengthType>()) struct Inclusion {
      std::array<LengthType, N> semiLengths;
      Inclusion(std::array<LengthType, N> semiL) : semiLengths(semiL){};
      virtual ~Inclusion(){};
    };

    /*!
     * This struct represents an ellipsoid.
     * \tparam unsigned short int: dimension
     * \tparam LengthType: type of the lengths
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Ellipsoid
        : public Inclusion<3u, LengthType> {
      Ellipsoid(LengthType a, LengthType b, LengthType c)
          : Inclusion<3u, LengthType>(std::array<LengthType, 3u>({a, b, c})){};

      virtual ~Ellipsoid(){};
    };

    /*!
     * This struct represents a 3d spheroid as a child of Inclusion<3>.
     * The second semi-length is shared by two axes
     * \tparam LengthType: type of the lengths
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Spheroid
        : public Inclusion<3u, LengthType> {
      Spheroid(LengthType a, LengthType b)
          : Inclusion<3u, LengthType>(std::array<LengthType, 3u>({a, b, b})){};

      virtual ~Spheroid(){};
    };

    /*!
     * This struct represents a 3d sphere as a child of Inclusion<3>
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Sphere
        : public Inclusion<3u, LengthType> {
      Sphere()
          : Inclusion<3u, LengthType>(std::array<LengthType, 3u>(
                {LengthType(1), LengthType(1), LengthType(1)})){};

      virtual ~Sphere(){};
    };

    /*!
     * This struct represents a 2d disk as a child of Inclusion<2>
     * \tparam LengthType: type of the radius
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Disk
        : public Inclusion<2u, LengthType> {
      Disk()
          : Inclusion<2u, LengthType>(
                std::array<LengthType, 2u>({LengthType(1), LengthType(1)})){};

      virtual ~Disk(){};
    };

    /*!
     * This struct represents an ellipse
     * \tparam unsigned short int: dimension
     * \tparam LengthType: type of the lengths
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Ellipse
        : public Inclusion<2u, LengthType> {
      Ellipse(LengthType a, LengthType b)
          : Inclusion<2u, LengthType>(std::array<LengthType, 2u>({a, b})){};

      virtual ~Ellipse(){};
    };

    ////Definition of 'Phase' objects

    /*!
     * This struct represents a phase. A phase is defined
     * by physical information (stiffness, volume fraction).
     * \tparam unsigned short int: dimension
     * \tparam StressType: type of the elastic constants
     */
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                                StressType>()) struct Phase {
      using real = tfel::types::real<StressType>;
      real fraction;
      tfel::math::st2tost2<N, StressType> stiffness;
      Phase(real f, const tfel::math::st2tost2<N, StressType> &C)
          : fraction(f), stiffness(C){};
      virtual ~Phase(){};
    };


    /////Definition of 'InclusionDistribution' objects

    /*!
     * This struct represents a distribution of inclusions as an abstract
     * class, child of class Phase. It contains an inclusion and a pure virtual
     * method computeMeanLocalisator which returns the mean localisator of the
     * distribution of inclusions, for a given matrix (Phase) in parameter.
     */
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct InclusionDistribution
        : Phase<N, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      Inclusion<N, LengthType> inclusion;
      InclusionDistribution(const Inclusion<N, LengthType> &inc,
                            real frac,
                            const tfel::math::st2tost2<N, StressType> &C)
          : Phase<N, StressType>(frac, C), inclusion(inc){};
      virtual tfel::math::st2tost2<N, real> computeMeanLocalisator(
          const tfel::math::st2tost2<N, StressType> &C0,
          bool isotropic_matrix,
          int max_iter_anisotropic_integration) = 0;
      virtual ~InclusionDistribution(){};
    };

    /*!
     * This struct represents a distribution of spheres, which is
     * a child of InclusionDistribution<3u>, with inclusion of type
     * Sphere.
     */
    template <tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct SphereDistribution
        : public InclusionDistribution<3u, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      SphereDistribution(const Sphere<LengthType> &sph,
                         real frac,
                         const tfel::math::st2tost2<3u, StressType> &Ci)
          : InclusionDistribution<3u, StressType>(sph, frac, Ci){};

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          bool isotropic_matrix,
          int max_iter_anisotropic_integration = 12) override {
        auto Ci = this->stiffness;
        if (not(isIsotropic<StressType>(Ci))) {
          tfel::reportContractViolation(
              "I cannot make computation on distribution of anisotropic "
              "inclusions");
        }
        auto pairi = computeKappaMu<StressType>(Ci);
        const auto kappai = std::get<0>(pairi);
        const auto mui = std::get<1>(pairi);
        const auto KGi = KGModuli<StressType>(kappai, mui);
        if (isotropic_matrix) {
          auto pair0 = computeKappaMu<StressType>(C0);
          const auto kappa0 = std::get<0>(pair0);
          const auto mu0 = std::get<1>(pair0);
          const auto KG0 = KGModuli<StressType>(kappa0, mu0);
          return computeSphereLocalisationTensor<StressType>(KG0, KGi);
        } else {
          tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
          tfel::math::tvector<3u, real> n_b = {0., 1., 0.};
          return computeAnisotropicLocalisationTensor<StressType>(
              C0, Ci, n_a, real(1), n_b, real(1), real(1),
              max_iter_anisotropic_integration);
        }
      };

      virtual ~SphereDistribution(){};
    };


    /*!
     * This struct represents an isotropic distribution of
     * 3d-ellipsoids, 3d-spheroids, as a child of
     * InclusionDistribution<N>
     */
    template <tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct IsotropicDistribution
        : public InclusionDistribution<3u,StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      IsotropicDistribution(const Ellipsoid<LengthType> &ell,
                            real frac,
                            const tfel::math::st2tost2<3u, StressType> &Ci)
          : InclusionDistribution<3u, StressType>(ell, frac, Ci){};
      IsotropicDistribution(const Spheroid<LengthType> &sphero,
                            real frac,
                            const tfel::math::st2tost2<3u, StressType> &Ci)
          : InclusionDistribution<3u, StressType>(sphero, frac, Ci){};

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          bool isotropic_matrix = true,
          int max_iter_anisotropic_integration = 12) override {
        tfel::math::st2tost2<3u, StressType> Ci = this->stiffness;
        if (not(isIsotropic<StressType>(Ci))) {
          tfel::reportContractViolation(
              "I cannot make computation on distribution of anisotropic "
              "inclusions");
        }
        auto pairi = computeKappaMu<StressType>(Ci);
        const auto kappai = std::get<0>(pairi);
        const auto mui = std::get<1>(pairi);
        const auto KGi = KGModuli<StressType>(kappai, mui);
        if (not(isIsotropic<StressType>(C0))) {
          //           std::cout << "warning: your matrix is not isotropic, and
          //           it will be "
          //                        "made isotropic for computing the average
          //                        localisator " "in the distribution"
          //                     << std::endl;
        }
        auto pair0 = computeKappaMu<StressType>(C0);
        const auto kappa0 = std::get<0>(pair0);
        const auto mu0 = std::get<1>(pair0);
        const auto KG0 = KGModuli<StressType>(kappa0, mu0);
        auto semiL = (this->inclusion).semiLengths;
        return EllipsoidMeanLocalisator<3u, StressType>::Isotropic(KG0, KGi,
                                                                     semiL);
      };

      virtual ~IsotropicDistribution(){};
    };

    /*!
     * This struct represents a transverse isotropic distribution of inclusions
     * in 3d, which can be only Spheroids or Ellipsoids. The vector \f$n\f$
     * is the axis of transverse isotropy. The index corresponds to the index of
     * the semi-length which corresponds to the axis of transverse isotropy
     * (axis which does not rotate). This index can hence be 0, 1 or 2.
     */
    template <tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct TransverseIsotropicDistribution
        : public InclusionDistribution<3u, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      tfel::math::tvector<3u, real> n;
      unsigned short int index;

      TransverseIsotropicDistribution(
          const Ellipsoid<LengthType> &ell,
          real frac,
          const tfel::math::st2tost2<3u, StressType> &C,
          const tfel::math::tvector<3u, real> &n_,
          unsigned short int &ind)
          : InclusionDistribution<3u, StressType>(ell, frac, C),
            n(n_),
            index(ind) {
        if ((ind != 0) and (ind != 1) and (ind != 2)) {
          tfel::reportContractViolation(
              "The index can only be 0, 1 or 2 and is the axis of the "
              "ellipsoid which does not rotate");
        }
      };

      TransverseIsotropicDistribution(
          const Spheroid<LengthType> &sphero,
          real frac,
          const tfel::math::st2tost2<3u, StressType> &C,
          const tfel::math::tvector<3u, real> &n_,
          unsigned short int &ind)
          : InclusionDistribution<3u, StressType>(sphero, frac, C),
            n(n_),
            index(ind) {
        if ((ind != 0) and (ind != 1) and (ind != 2)) {
          tfel::reportContractViolation(
              "The index can only be 0, 1 or 2 and is the axis of the "
              "ellipsoid which does not rotate");
        }
      };

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          bool isotropic_matrix,
          int max_iter_anisotropic_integration = 12) override {
        tfel::math::st2tost2<3u, StressType> Ci = this->stiffness;
        if (not(isIsotropic<StressType>(Ci))) {
          tfel::reportContractViolation(
              "I cannot make computation on distribution of anisotropic "
              "inclusions");
        }
        auto pairi = computeKappaMu<StressType>(Ci);
        const auto kappai = std::get<0>(pairi);
        const auto mui = std::get<1>(pairi);
        const auto KGi = KGModuli<StressType>(kappai, mui);
        if (not(isIsotropic<StressType>(C0))) {
          //           std::cout << "warning: your matrix is not isotropic, and
          //           it will be "
          //                        "made isotropic for computing the average
          //                        localisator " "in the distribution"
          //                     << std::endl;
        }
        auto pair0 = computeKappaMu<StressType>(C0);
        const auto kappa0 = std::get<0>(pair0);
        const auto mu0 = std::get<1>(pair0);
        const auto KG0 = KGModuli<StressType>(kappa0, mu0);
        auto semiL = (this->inclusion).semiLengths;
        auto ind = this->index;
        auto ai = semiL[ind];
        LengthType bi;
        LengthType ci;
        if (ind == 0) {
          bi = semiL[1];
          ci = semiL[2];
        }
        if (ind == 1) {
          bi = semiL[0];
          ci = semiL[2];
        }
        if (ind == 2) {
          bi = semiL[0];
          ci = semiL[1];
        }
        return EllipsoidMeanLocalisator<3u, StressType>::TransverseIsotropic(
            KG0, KGi, this->n, ai, bi, ci);
      };

      virtual ~TransverseIsotropicDistribution(){};
    };

    /*!
     * This struct represents a distribution of inclusions with a unique
     * orientation in 3d, which can be only Spheroids or Ellipsoids.
     * The vectors \f$n_a\f$ and \f$n_b\f$ define the orientation.
     * The first semi-length of the inclusion corresponds to \f$n_a\f$. The
     * second semi-length corresponds to \f$n_b\f$.
     */
    template <tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct OrientedDistribution
        : public InclusionDistribution<3u, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      tfel::math::tvector<3u, real> n_a;
      tfel::math::tvector<3u, real> n_b;

      OrientedDistribution(const Ellipsoid<LengthType> &ell,
                           real frac,
                           const tfel::math::st2tost2<3u, StressType> &C,
                           const tfel::math::tvector<3u, real> &n_a_,
                           const tfel::math::tvector<3u, real> &n_b_)
          : InclusionDistribution<3u, StressType>(ell, frac, C),
            n_a(n_a_),
            n_b(n_b_) {
        if (not(tfel::math::ieee754::fpclassify(n_a | n_b) == FP_ZERO)) {
          tfel::reportContractViolation("n_a and n_b not normals");
        }
      };


      OrientedDistribution(const Spheroid<LengthType> &sphero,
                           real frac,
                           tfel::math::st2tost2<3u, StressType> &C,
                           const tfel::math::tvector<3u, real> &n_a_,
                           const tfel::math::tvector<3u, real> &n_b_)
          : InclusionDistribution<3u, StressType>(sphero, frac, C),
            n_a(n_a_),
            n_b(n_b_) {
        if (not(tfel::math::ieee754::fpclassify(n_a | n_b) == FP_ZERO)) {
          tfel::reportContractViolation("n_a and n_b not normals");
        }
      };

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          bool isotropic_matrix,
          int max_iter_anisotropic_integration = 12) override {
        tfel::math::st2tost2<3u, StressType> Ci = this->stiffness;
        auto semiL = (this->inclusion).semiLengths;
        auto n_a_i = this->n_a;
        auto n_b_i = this->n_b;
        if (isotropic_matrix) {
          auto pair0 = computeKappaMu<StressType>(C0);
          const auto kappa0 = std::get<0>(pair0);
          const auto mu0 = std::get<1>(pair0);
          const auto KG0 = KGModuli<StressType>(kappa0, mu0);
          return computeIsotropicLocalisationTensor<3u,StressType>(KG0, Ci, n_a_i, n_b_i, semiL);
        } else {
            return computeGeneralAnisotropicLocalisationTensor<3u,StressType>(
                C0, Ci, n_a_i, n_b_i, semiL,
                max_iter_anisotropic_integration);
        }
      };

      virtual ~OrientedDistribution(){};
    };

    ////Definition of 'Microstructure' objects

    /*!
     * This struct represents a microstructure.
     * \tparam unsigned short int: dimension
     * \tparam StressType: type of the elastic constants related to the
     * inclusion
     */
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct Microstructure {
      Microstructure(){};
      virtual ~Microstructure(){};
    };

    /*!
     * This struct defines a matrix-inclusion microstructure (or particulate
     * microstructure) as a child of Microstructure. This type of microstructure
     * is viewed as an array of InclusionDistribution, with a matrixPhase.
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

      ParticulateMicrostructure(tfel::math::st2tost2<N, StressType> &C0)
          : Microstructure<N, StressType>(),
            number_of_phases(1),
            inclusionPhases(
                std::vector<InclusionDistribution<N, StressType> *>({})),
            matrixPhase(Phase<N, StressType>(real(1), C0)){};

      virtual ~ParticulateMicrostructure(){};

      int addInclusionPhase(
          InclusionDistribution<N, StressType> &inclusionPhase) {
        if (this->matrixPhase.fraction - inclusionPhase.fraction < real(0)) {
          //           std::cout << "the volume fraction of inclusions is too
          //           high !"
          //                     << std::endl;
          return 0;
        } else {
          (this->number_of_phases)++;
          (this->matrixPhase.fraction) -= inclusionPhase.fraction;
          (this->inclusionPhases).push_back(&inclusionPhase);
          return 1;
        };
      };

      int removeInclusionPhase(unsigned int i) {
        if ((this->number_of_phases) == 1) {
//          std::cout << "there are no more inclusions !" << std::endl;
          return 0;
        } else if ((this->number_of_phases) < i + 2) {
          //           std::cout << "there are less phases than what you think
          //           !"
          //                     << std::endl;
          return 0;
        } else {
          if ((this->number_of_phases) == 2) {
            //             std::cout << "you have removed the last inclusion
            //             phase !"
            //                       << std::endl;
          }
          (this->number_of_phases)--;
          (this->matrixPhase.fraction) -= *(this->inclusionPhases[i]).fraction;
          (this->inclusionPhases).erase((this->inclusionPhases).begin() + i);
          return 1;
        };
      };

      unsigned int get_number_of_phases() { return (this->number_of_phases); };

      real get_matrix_fraction() { return (this->matrixPhase.fraction); };

      tfel::math::st2tost2<N, StressType> get_matrix_elasticity() {
        return (this->matrixPhase.stiffness);
      };

      InclusionDistribution<N, StressType> *get_inclusionPhase(unsigned int i) {
        if ((this->number_of_phases) < i + 2) {
          tfel::reportContractViolation(
              "there are less phases than what you think !");
        }
        return (this->inclusionPhases)[i];
      };

     private:
      unsigned int number_of_phases;
      std::vector<InclusionDistribution<N, StressType> *> inclusionPhases;
      Phase<N, StressType> matrixPhase;
    };

  }  // namespace homogenization::elasticity
}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX */
