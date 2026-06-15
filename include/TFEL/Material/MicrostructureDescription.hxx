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
#include "TFEL/Material/OrientationAverages.hxx"
#include "TFEL/Material/LocalisationTensor.hxx"
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"
#include <stdexcept>
#include <memory>

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
      Inclusion(std::array<LengthType, N> semiL) : semiLengths(semiL) {}
    };

    /*!
     * This struct represents an ellipsoid as a child of Inclusion<3>
     * \tparam unsigned short int: dimension
     * \tparam LengthType: type of the lengths
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Ellipsoid
        : public Inclusion<3u, LengthType> {
      Ellipsoid(LengthType a, LengthType b, LengthType c)
          : Inclusion<3u, LengthType>(std::array<LengthType, 3u>({a, b, c})) {}
    };

    /*!
     * This struct represents a 3d spheroid as a child of Ellipsoid.
     * The second semi-length is shared by two axes
     * \tparam LengthType: type of the lengths
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Spheroid
        : public Ellipsoid<LengthType> {
      Spheroid(LengthType a, LengthType b) : Ellipsoid<LengthType>(a, b, b) {}
    };

    /*!
     * This struct represents a 3d sphere as a child of Spheroid
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Sphere
        : public Spheroid<LengthType> {
      Sphere() : Spheroid<LengthType>(LengthType(1), LengthType(1)) {}
    };

    /*!
     * This struct represents an ellipse as a child of Inclusion<2>
     * \tparam unsigned short int: dimension
     * \tparam LengthType: type of the lengths
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Ellipse
        : public Inclusion<2u, LengthType> {
      Ellipse(LengthType a, LengthType b)
          : Inclusion<2u, LengthType>(std::array<LengthType, 2u>({a, b})) {}
    };

    /*!
     * This struct represents a 2d disk as a child of Ellipse
     * \tparam LengthType: type of the radius
     */
    template <tfel::math::ScalarConcept LengthType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Length,
                                                LengthType>()) struct Disk
        : public Ellipse<LengthType> {
      Disk() : Ellipse<LengthType>(LengthType(1), LengthType(1)) {}
    };

    ////Definition of 'Phase' objects

    /*!
     * This struct represents a phase. A phase has a volume fraction.
     * \tparam unsigned short int: dimension
     * \tparam StressType: type of the elastic constants
     */
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                                StressType>()) struct Phase {
      using real = tfel::types::real<StressType>;
      real fraction;
      Phase(real f, const tfel::math::st2tost2<N, StressType> &C)
          : fraction(f), stiffness(C), isotropic(false) {}
      Phase(real f, const IsotropicModuli<StressType> &IM)
          : fraction(f),
            stiffness(computeIsotropicStiffnessTensor<StressType>(IM)),
            isotropic(true) {}
      
      Phase() = default ;
      
      tfel::math::st2tost2<N, StressType> getElasticityOfPhase() const { return this->stiffness;}
      
      void changeElasticityOfPhase(const tfel::math::st2tost2<N, StressType> &C) { this->stiffness = C; this->isotropic = false;}
      void changeElasticityOfPhase(const IsotropicModuli<StressType> &IM) { this->stiffness = computeIsotropicStiffnessTensor<StressType>(IM); this->isotropic = true;}
      
      bool is_isotropic() const { return this->isotropic; }
      bool isIsotropic() const { return this->isotropic; }

     private:
      tfel::math::st2tost2<N, StressType> stiffness;
      bool isotropic;
    };

    /////Definition of 'InclusionDistribution' objects

    /*!
     * This struct represents a distribution of inclusions as an abstract
     * class, child of class Phase. It contains an inclusion and a pure virtual
     * method computeMeanLocalisator which returns the mean localisator of the
     * distribution of inclusions, for a given matrix elasticity in parameter.
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
          : Phase<N, StressType>(frac, C), inclusion(inc) {}

      InclusionDistribution(const Inclusion<N, LengthType> &inc,
                            real frac,
                            const IsotropicModuli<StressType> &IM)
          : Phase<N, StressType>(frac, IM), inclusion(inc) {}

      InclusionDistribution(const InclusionDistribution<N, StressType> &ID) =
          default;
      using Phase<N, StressType>::operator=;
      
      virtual std::unique_ptr<InclusionDistribution<N, StressType>> clone() const = 0;

      virtual tfel::math::st2tost2<N, real> computeMeanLocalisator(
          const tfel::math::st2tost2<N, StressType> &C0,
          int max_iter_anisotropic_integration) = 0;
      virtual tfel::math::st2tost2<N, real> computeMeanLocalisator(
          const IsotropicModuli<StressType> &IM0) = 0;
      virtual ~InclusionDistribution() = default;
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
                         const IsotropicModuli<StressType> &IMi)
          : InclusionDistribution<3u, StressType>(sph, frac, IMi) {}

      SphereDistribution(const Sphere<LengthType> &sph,
                         real frac,
                         const tfel::math::st2tost2<3u, StressType> &Ci)
          : InclusionDistribution<3u, StressType>(sph, frac, Ci) {}

      std::unique_ptr<InclusionDistribution<3u,StressType>> clone() const override {
        return std::make_unique<SphereDistribution<StressType>>(*this);
      }
      
      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          int max_iter_anisotropic_integration = 12) override {
        tfel::math::tvector<3u, real> n_a = {1., 0., 0.};
        tfel::math::tvector<3u, real> n_b = {0., 1., 0.};
        auto Ci = this->getElasticityOfPhase();
        return computeAnisotropicLocalisationTensor<StressType>(
            C0, Ci, n_a, LengthType(1), n_b, LengthType(1), LengthType(1),
            max_iter_anisotropic_integration);
      }

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const IsotropicModuli<StressType> &IM0) override {
        auto Ci = this->getElasticityOfPhase();
        if (not(this->isIsotropic())) {
          tfel::math::tvector<3u, real> n_x = {1., 0., 0.};
          tfel::math::tvector<3u, real> n_y = {0., 1., 0.};
          return computeEllipsoidLocalisationTensor<StressType>(
              IM0, Ci, n_x, LengthType(1), n_y, LengthType(1), LengthType(1));
        } else {
          const auto KGi = computeKGModuli<StressType>(Ci);
          return computeSphereLocalisationTensor<StressType>(IM0, KGi);
        }
      }
    };  // End of SphereDistribution

    /*!
     * This struct represents an isotropic distribution of
     * 3d-ellipsoids, 3d-spheroids, as a child of
     * InclusionDistribution<N>
     */
    template <tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct IsotropicDistribution
        : public InclusionDistribution<3u, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      IsotropicDistribution(const Ellipsoid<LengthType> &ell,
                            real frac,
                            const IsotropicModuli<StressType> &IMi)
          : InclusionDistribution<3u, StressType>(ell, frac, IMi) {}
      IsotropicDistribution(const Spheroid<LengthType> &sphero,
                            real frac,
                            const IsotropicModuli<StressType> &IMi)
          : InclusionDistribution<3u, StressType>(sphero, frac, IMi) {}
      
      std::unique_ptr<InclusionDistribution<3u,StressType>> clone() const override {
        return std::make_unique<IsotropicDistribution<StressType>>(*this);
      }

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const IsotropicModuli<StressType> &IM0) override {
        tfel::math::st2tost2<3u, StressType> Ci = this->getElasticityOfPhase();
        const auto KGi = computeKGModuli<StressType>(Ci);
        auto semiL = (this->inclusion).semiLengths;
        return EllipsoidMeanLocalisator<3u, StressType>::Isotropic(IM0, KGi,
                                                                   semiL);
      }

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          int max_iter_anisotropic_integration = 12) override {
        tfel::reportContractViolation(
            "I cannot compute the mean localisator for a distribution of non "
            "oriented anisotropic inclusions in an anisotropic matrix. Try to "
            "use an IsotropicModuli for the matrix");
        auto C1 = 1 / StressType(max_iter_anisotropic_integration) * C0;
        return C1;
      }
    };  // End of IsotropicDistribution

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

      TransverseIsotropicDistribution(const Ellipsoid<LengthType> &ell,
                                      real frac,
                                      const IsotropicModuli<StressType> &IM,
                                      const tfel::math::tvector<3u, real> &n_,
                                      unsigned short int &ind)
          : InclusionDistribution<3u, StressType>(ell, frac, IM),
            n(n_),
            index(ind) {
        if ((ind != 0) and (ind != 1) and (ind != 2)) {
          tfel::reportContractViolation(
              "The index can only be 0, 1 or 2 and is the axis of the "
              "ellipsoid which does not rotate");
        }
      }

      TransverseIsotropicDistribution(const Spheroid<LengthType> &sphero,
                                      real frac,
                                      const IsotropicModuli<StressType> &IM,
                                      const tfel::math::tvector<3u, real> &n_,
                                      unsigned short int &ind)
          : InclusionDistribution<3u, StressType>(sphero, frac, IM),
            n(n_),
            index(ind) {
        if ((ind != 0) and (ind != 1) and (ind != 2)) {
          tfel::reportContractViolation(
              "The index can only be 0, 1 or 2 and is the axis of the "
              "ellipsoid which does not rotate");
        }
      }
      
      std::unique_ptr<InclusionDistribution<3u,StressType>> clone() const override {
        return std::make_unique<TransverseIsotropicDistribution<StressType>>(*this);
      }

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const IsotropicModuli<StressType> &IM0) override {
        tfel::math::st2tost2<3u, StressType> Ci = this->getElasticityOfPhase();
        const auto KGi = computeKGModuli<StressType>(Ci);
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
            IM0, KGi, this->n, ai, bi, ci);
      }

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          int max_iter_anisotropic_integration = 12) override {
        tfel::reportContractViolation(
            "I cannot compute the mean localisator for a distribution of non "
            "oriented anisotropic inclusions in an anisotropic matrix. Try to "
            "use an IsotropicModuli for the matrix");
        auto C1 = 1 / StressType(max_iter_anisotropic_integration) * C0;
        return C1;
      }
    }; // End of TransverseIsotropicDistribution

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
      }

      OrientedDistribution(const Ellipsoid<LengthType> &ell,
                           real frac,
                           const IsotropicModuli<StressType> &IM,
                           const tfel::math::tvector<3u, real> &n_a_,
                           const tfel::math::tvector<3u, real> &n_b_)
          : InclusionDistribution<3u, StressType>(ell, frac, IM),
            n_a(n_a_),
            n_b(n_b_) {
        if (not(tfel::math::ieee754::fpclassify(n_a | n_b) == FP_ZERO)) {
          tfel::reportContractViolation("n_a and n_b not normals");
        }
      }

      OrientedDistribution(const Spheroid<LengthType> &sphero,
                           real frac,
                           const tfel::math::st2tost2<3u, StressType> &C,
                           const tfel::math::tvector<3u, real> &n_a_,
                           const tfel::math::tvector<3u, real> &n_b_)
          : InclusionDistribution<3u, StressType>(sphero, frac, C),
            n_a(n_a_),
            n_b(n_b_) {
        if (not(tfel::math::ieee754::fpclassify(n_a | n_b) == FP_ZERO)) {
          tfel::reportContractViolation("n_a and n_b not normals");
        }
      }

      OrientedDistribution(const Spheroid<LengthType> &sphero,
                           real frac,
                           const IsotropicModuli<StressType> &IM,
                           const tfel::math::tvector<3u, real> &n_a_,
                           const tfel::math::tvector<3u, real> &n_b_)
          : InclusionDistribution<3u, StressType>(sphero, frac, IM),
            n_a(n_a_),
            n_b(n_b_) {
        if (not(tfel::math::ieee754::fpclassify(n_a | n_b) == FP_ZERO)) {
          tfel::reportContractViolation("n_a and n_b not normals");
        }
      }
      
      std::unique_ptr<InclusionDistribution<3u,StressType>> clone() const override {
        return std::make_unique<OrientedDistribution<StressType>>(*this);
      }
      
      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          int max_iter_anisotropic_integration = 12) override {
        auto Ci = this->getElasticityOfPhase();
        auto semiL = (this->inclusion).semiLengths;
        auto n_a_i = this->n_a;
        auto n_b_i = this->n_b;
        return computeGeneralAnisotropicLocalisationTensor<3u, StressType>(
            C0, Ci, n_a_i, n_b_i, semiL, max_iter_anisotropic_integration);
      }

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const IsotropicModuli<StressType> &IM0) override {
        auto Ci = this->getElasticityOfPhase();
        auto semiL = (this->inclusion).semiLengths;
        auto n_a_i = this->n_a;
        auto n_b_i = this->n_b;
        return computeIsotropicLocalisationTensor<3u, StressType>(
            IM0, Ci, n_a_i, n_b_i, semiL);
      }
    }; // End of OrientedDistribution
    
    /*!
     * This struct represents a distribution of spheroidal inclusions
     * whose orientation tensors A2 and A4 are given.
     * A2 is the average of n^n and A4 the average of n^n^n^n.
     */
    template <tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct UserDefinedDistributionOfSpheroids
        : public InclusionDistribution<3u, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      tfel::math::stensor<3u,real> A2;
      tfel::math::st2tost2<3u,real> A4;
      UserDefinedDistributionOfSpheroids(const Spheroid<LengthType> &sphero,
                           real frac,
                           const IsotropicModuli<StressType> &IMi,
                           const tfel::math::stensor<3u,real> &A2_,
                           const tfel::math::st2tost2<3u,real> &A4_)
          : InclusionDistribution<3u, StressType>(sphero, frac, IMi),
            A2(A2_),
            A4(A4_) {}
      
      std::unique_ptr<InclusionDistribution<3u,StressType>> clone() const override {
        return std::make_unique<UserDefinedDistributionOfSpheroids<StressType>>(*this);
      }

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<3u, StressType> &C0,
          int max_iter_anisotropic_integration = 12) override {
        tfel::reportContractViolation(
            "I cannot compute the mean localisator for a distribution of non "
            "oriented anisotropic inclusions in an anisotropic matrix. Try to "
            "use an IsotropicModuli for the matrix");
        auto C1 = 1 / StressType(max_iter_anisotropic_integration) * C0;
        return C1;
      }

      virtual tfel::math::st2tost2<3u, real> computeMeanLocalisator(
          const IsotropicModuli<StressType> &IM0) override {
        auto Ci = this->getElasticityOfPhase();
        const auto KGi = computeKGModuli<StressType>(Ci);
        auto semiL = (this->inclusion).semiLengths;
        return EllipsoidMeanLocalisator<3u,StressType>::UserDefinedDistributionOfSpheroids(IM0,KGi,semiL[0],semiL[1],A2,A4);
      }
    }; // End of UserDefinedDistributionOfSpheroids
     
   
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
      Microstructure() {}
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
      
      ParticulateMicrostructure(const tfel::math::st2tost2<N, StressType> &C0)
          : Microstructure<N, StressType>(),
            number_of_phases(1),
            inclusion_phases{},
            matrix_phase(Phase<N, StressType>(real(1), C0)) {}

      ParticulateMicrostructure(const IsotropicModuli<StressType> &IM0)
          : Microstructure<N, StressType>(),
            number_of_phases(1),
            inclusion_phases{},
            matrix_phase(Phase<N, StressType>(real(1), IM0)) {}
      
      void changeElasticityOfMatrixPhase(const tfel::math::st2tost2<N, StressType> &C0) {
        (this->matrix_phase).changeElasticityOfPhase(C0);
      }

      void changeElasticityOfMatrixPhase(const IsotropicModuli<StressType> &IM0) {
        (this->matrix_phase).changeElasticityOfPhase(IM0);
      }
      
      tfel::math::st2tost2<N, StressType> get_matrix_elasticity() {
        return (this->matrix_phase.getElasticityOfPhase());
      }
      
      // this method has the right convention name
      tfel::math::st2tost2<N, StressType> getMatrixElasticity() {
        return (this->matrix_phase.getElasticityOfPhase());
      }
      
      bool is_isotropic_matrix() const { return (this->matrix_phase.isIsotropic()); }
      bool isIsotropicMatrix() const { return (this->matrix_phase.isIsotropic()); }
      
      real get_matrix_fraction() { return (this->matrix_phase.fraction); }
      real getMatrixFraction() { return (this->matrix_phase.fraction); }

      int addInclusionPhase(
          InclusionDistribution<N, StressType> &inclusionPhase) {
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
      
      std::unique_ptr<InclusionDistribution<N, StressType>> get_inclusionPhase(unsigned int i) {
        if ((this->number_of_phases) < i + 2) {
          tfel::reportContractViolation(
              "there are less phases than what you think !");
        }
        return ((this->inclusion_phases)[i])->clone();
      }
      
      // the method with the right convention name
      std::unique_ptr<InclusionDistribution<N, StressType>> getInclusionPhase(unsigned int i) {
        if ((this->number_of_phases) < i + 2) {
          tfel::reportContractViolation(
              "there are less phases than what you think !");
        }
        return ((this->inclusion_phases)[i])->clone();
      }

      int removeInclusionPhase(unsigned int i) {
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
      
      int changeElasticityOfInclusionPhase(unsigned int i,const tfel::math::st2tost2<N, StressType> &C) {
        if ((this->number_of_phases) == 1) {
          return 0;
        } else if ((this->number_of_phases) < i + 2) {
          return 0;
        } else {
          (*((this->inclusion_phases)[i])).changeElasticityOfPhase(C);
          return 1;
        }
      }
      
      int changeElasticityOfInclusionPhase(unsigned int i,const IsotropicModuli<StressType> &IM) {
        if ((this->number_of_phases) == 1) {
          return 0;
        } else if ((this->number_of_phases) < i + 2) {
          return 0;
        } else {
          (*((this->inclusion_phases)[i])).changeElasticityOfPhase(IM);
          return 1;
        }
      }
      
      int changeFractionOfInclusionPhase(unsigned int i,const real f) {
        if ((this->number_of_phases) == 1) {
          return 0;
        } else if ((this->number_of_phases) < i + 2) {
          return 0;
        } else if (this->matrix_phase.fraction + (*(this->inclusion_phases[i])).fraction - f < real(0)) {
          return 0;
        }
        else {
          (this->matrix_phase.fraction) += (*(this->inclusion_phases[i])).fraction;
          (this->matrix_phase.fraction) -= f;
          (*((this->inclusion_phases)[i])).fraction=f;
          return 1;
        }
      }

      unsigned int get_number_of_phases() { return (this->number_of_phases); }
      unsigned int getNumberOfPhases() { return (this->number_of_phases); }

     private:
      unsigned int number_of_phases;
      std::vector<std::unique_ptr<InclusionDistribution<N, StressType>>> inclusion_phases;
      Phase<N, StressType> matrix_phase;
    };

  }  // namespace homogenization::elasticity
}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX */
