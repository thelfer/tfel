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
#include <stdexcept>

namespace tfel::material {

  /*!
   * This function makes the projection of a `st2tost2`
   * on the subset of isotropic fourth-order tensors.
   * It uses the quadruple contraction of \f$C_i\f$ with \f$J\f$
   * and \f$K\f$. It returns the moduli \f$k\f$ and \f$mu\f$.
   * \tparam T: type of the `st2tost2`
   * \param A : `st2tost2`
   * \return a std::pair<T,T> (kappa,mu)
   */
  template <unsigned short int N, typename T>
  std::pair<T, T> computeKappaMu(const tfel::math::st2tost2<N, T> &A) {
    const auto siz = tfel::math::StensorDimeToSize<N>::value;
    auto J = tfel::math::st2tost2<N, tfel::types::real<T>>::J();
    auto K = tfel::math::st2tost2<N, tfel::types::real<T>>::K();
    const T kappai = tfel::math::quaddot(A, J) / 3;
    const T mui = tfel::math::quaddot(A, K) / (siz - 1) / 2;
    return {kappai, mui};
  }  // end of computeKappaMu

  /*!
   * \brief This function computes the relative error between two `st2tost2`,
   * based on the `tfel::math::norm` function.
   * \tparam real: relative error
   * \tparam N: dimension
   * \tparam T: type of the tensors
   * \param C1,C2 st2tost2 objects
   * \return a real
   */
  template <unsigned short int N, typename real, typename T>
  real relative_error(const tfel::math::st2tost2<N, T> &C1,
                      const tfel::math::st2tost2<N, T> &C2) {
    auto val = tfel::math::norm(C1 - C2) / tfel::math::norm(C2);
    return val;
  }  // end of relative_error

  /*!
   * \brief This function permits to know if a `st2tost2` is Isotropic
   * It first makes the projection on the subset of isotropic fourth-order
   * tensors using computeKappaMu and then compare the projected tensor with the
   * original \f$A_i\f$. \tparam T: type of the `st2tost2` \param Ai :
   * `st2tost2` \return a boolean
   */
  template <unsigned short int N, typename T>
  bool isIsotropic(const tfel::math::st2tost2<N, T> &Ai) {
    static constexpr auto eps = std::numeric_limits<types::real<T>>::epsilon();
    const auto pair = computeKappaMu<N, T>(Ai);
    const auto kappai = std::get<0>(pair);
    const auto mui = std::get<1>(pair);
    auto J = tfel::math::st2tost2<N, tfel::types::real<T>>::J();
    auto K = tfel::math::st2tost2<N, tfel::types::real<T>>::K();
    const auto A_comp = 3 * kappai * J + 2 * mui * K;
    const auto val = relative_error<N, types::real<T>, T>(Ai, A_comp);
    if (val > 100 * eps) {
      return false;
    }
    return true;
  }  // end of isIsotropic

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

    /*!
     * GrainPhase is defined for polycrystal microstructure as a child of Phase.
     * It has an Inclusion<N>, and vectors for its orientation.
     * \f$n_a\f$ corresponds to the first semi-length and \f$n_b\f$ to the
     * second semi-length.
     * The elasticity must be expressed in the basis given by these orientation
     * vectors.
     */
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                                StressType>()) struct GrainPhase
        : public Phase<N, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      Inclusion<N, LengthType> inclusion;
      tfel::math::tvector<N, real> n_a;
      tfel::math::tvector<N, real> n_b;

      GrainPhase(real frac,
                 tfel::math::st2tost2<N, StressType> &C,
                 Inclusion<N, LengthType> &inc,
                 const tfel::math::tvector<N, real> &n_a_,
                 const tfel::math::tvector<N, real> &n_b_)
          : Phase<N, StressType>(frac, C),
            inclusion(inc),
            n_a(n_a_),
            n_b(n_b_) {
        static constexpr auto eps = std::numeric_limits<real>::epsilon();
        if (not((n_a | n_b) < eps)) {
          tfel::reportContractViolation("n_a and n_b not normals");
        }
      };

      tfel::math::st2tost2<N, real> computeMeanLocalisator(
          const tfel::math::st2tost2<N, StressType> &C0,
          bool isotropic_matrix,
          int max_iter_anisotropic_integration = 12) {
        tfel::math::st2tost2<N, StressType> Ci = this->stiffness;
        auto semiL = (this->inclusion).semiLengths;
        auto n_a_i = this->n_a;
        auto n_b_i = this->n_b;
        auto ai = semiL[0];
        auto bi = semiL[1];
        LengthType ci;
        if (N == 3) {
          ci = semiL[2];
        }
        if (isotropic_matrix) {
          auto pair0 = computeKappaMu<N, StressType>(C0);
          const auto kappa0 = std::get<0>(pair0);
          const auto mu0 = std::get<1>(pair0);
          const auto KG0 = KGModuli<StressType>(kappa0, mu0);
          auto pairi = computeKappaMu<N, StressType>(Ci);
          const auto kappai = std::get<0>(pairi);
          const auto mui = std::get<1>(pairi);
          const auto KGi = KGModuli<StressType>(kappai, mui);
          if (N == 3) {
            return computeEllipsoidLocalisationTensor<StressType>(
                KG0, Ci, n_a_i, ai, n_b_i, bi, ci);
          } else if (N == 2) {
            return computeEllipticCylinderLocalisationTensor<StressType>(
                KG0, Ci, n_a_i, ai, bi);
          }
        } else {
          if (N == 3) {
            return computeAnisotropicLocalisationTensor<StressType>(
                C0, Ci, n_a_i, ai, n_b_i, bi, ci,
                max_iter_anisotropic_integration);
          } else if (N == 2) {
            return computePlainStrainAnisotropicLocalisationTensor<StressType>(
                C0, Ci, n_a_i, ai, bi, max_iter_anisotropic_integration);
          }
        }
      };

      virtual ~GrainPhase(){};
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
        if (not(isIsotropic<3u, StressType>(Ci))) {
          tfel::reportContractViolation(
              "I cannot make computation on distribution of anisotropic "
              "inclusions");
        }
        auto pairi = computeKappaMu<3u, StressType>(Ci);
        const auto kappai = std::get<0>(pairi);
        const auto mui = std::get<1>(pairi);
        const auto KGi = KGModuli<StressType>(kappai, mui);
        if (isotropic_matrix) {
          auto pair0 = computeKappaMu<3u, StressType>(C0);
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
     * This struct represents a distribution of disks, which is
     * a child of InclusionDistribution, with inclusion of type
     * Disk.
     */
    template <tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct DiskDistribution
        : public InclusionDistribution<2u, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      DiskDistribution(Disk<LengthType> &disk,
                       real frac,
                       tfel::math::st2tost2<2u, StressType> &Ci)
          : InclusionDistribution<2u, StressType>(disk, frac, Ci){};

      virtual tfel::math::st2tost2<2u, real> computeMeanLocalisator(
          const tfel::math::st2tost2<2u, StressType> &C0,
          bool isotropic_matrix,
          int max_iter_anisotropic_integration = 12) override {
        auto Ci = this->stiffness;
        if (not(isIsotropic<2u, StressType>(Ci))) {
          tfel::reportContractViolation(
              "I cannot make computation on distribution of anisotropic "
              "inclusions");
        }
        if (isotropic_matrix) {
          auto pair0 = computeKappaMu<2u, StressType>(C0);
          const auto kappa0 = std::get<0>(pair0);
          const auto mu0 = std::get<1>(pair0);
          const auto KG0 = KGModuli<StressType>(kappa0, mu0);
          tfel::math::tvector<2u, real> n_a = {1., 0.};
          return computeEllipticCylinderLocalisationTensor<StressType>(
              KG0, Ci, n_a, real(1), real(1))
        } else {
          tfel::math::tvector<2u, real> n_a = {1., 0.};
          return computePlainStrainAnisotropicLocalisationTensor<StressType>(
              C0, Ci, n_a, real(1), real(1), max_iter_anisotropic_integration);
        }
      };

      virtual ~DiskDistribution(){};
    };

    /*!
     * This struct represents an isotropic distribution of
     * 3d-ellipsoids, 3d-spheroids, or 2d-ellipses as a child of
     * InclusionDistribution<N>
     */
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct IsotropicDistribution
        : public InclusionDistribution<N, StressType> {
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
      IsotropicDistribution(const Ellipse<LengthType> &ell,
                            real frac,
                            const tfel::math::st2tost2<2u, StressType> &Ci)
          : InclusionDistribution<2u, StressType>(ell, frac, Ci){};

      virtual tfel::math::st2tost2<N, real> computeMeanLocalisator(
          const tfel::math::st2tost2<N, StressType> &C0,
          bool isotropic_matrix = true,
          int max_iter_anisotropic_integration = 12) override {
        tfel::math::st2tost2<N, StressType> Ci = this->stiffness;
        if (not(isIsotropic<N, StressType>(Ci))) {
          tfel::reportContractViolation(
              "I cannot make computation on distribution of anisotropic "
              "inclusions");
        }
        auto pairi = computeKappaMu<N, StressType>(Ci);
        const auto kappai = std::get<0>(pairi);
        const auto mui = std::get<1>(pairi);
        const auto KGi = KGModuli<StressType>(kappai, mui);
        if (not(isIsotropic<N, StressType>(C0))) {
          std::cout << "warning: your matrix is not isotropic, and it will be "
                       "made isotropic for computing the average localisator "
                       "in the distribution"
                    << std::endl;
        }
        auto pair0 = computeKappaMu<3u, StressType>(C0);
        const auto kappa0 = std::get<0>(pair0);
        const auto mu0 = std::get<1>(pair0);
        const auto KG0 = KGModuli<StressType>(kappa0, mu0);
        auto semiL = (this->inclusion).semiLengths;
        auto ai = semiL[0];
        auto bi = semiL[1];
        if (N == 2) {
          return EllipsoidMeanLocalisator<2u, StressType>::Isotropic(KG0, KGi,
                                                                     ai, bi);
        } else if (N == 3) {
          auto ci = semiL[2];
          auto pair = EllipsoidMeanLocalisator<3u, StressType>::Isotropic(
              KG0, KGi, ai, bi, ci);
          auto kappaA = std::get<0>(pair);
          auto muA = std::get<1>(pair);
          return 3 * kappaA * tfel::math::st2tost2<3u, real>::J() +
                 2 * muA * tfel::math::st2tost2<3u, real>::K();
        }
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
        if (not(isIsotropic<3u, StressType>(Ci))) {
          tfel::reportContractViolation(
              "I cannot make computation on distribution of anisotropic "
              "inclusions");
        }
        auto pairi = computeKappaMu<3u, StressType>(Ci);
        const auto kappai = std::get<0>(pairi);
        const auto mui = std::get<1>(pairi);
        const auto KGi = KGModuli<StressType>(kappai, mui);
        if (not(isIsotropic<3u, StressType>(C0))) {
          std::cout << "warning: your matrix is not isotropic, and it will be "
                       "made isotropic for computing the average localisator "
                       "in the distribution"
                    << std::endl;
        }
        auto pair0 = computeKappaMu<3u, StressType>(C0);
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
     * orientation in 2d or 3d, which can be only Spheroids or Ellipsoids or
     * 2d-Ellipses. The vectors \f$n_a\f$ and \f$n_b\f$ define the orientation.
     * The first semi-length of the inclusion corresponds to \f$n_a\f$. The
     * second semi-length corresponds to \f$n_b\f$.
     */
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) struct OrientedDistribution
        : public InclusionDistribution<N, StressType> {
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      tfel::math::tvector<N, real> n_a;
      tfel::math::tvector<N, real> n_b;

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

      OrientedDistribution(const Ellipse<LengthType> &ell,
                           real frac,
                           const tfel::math::st2tost2<2u, StressType> &C,
                           const tfel::math::tvector<2u, real> &n_a_,
                           const tfel::math::tvector<2u, real> &n_b_)
          : InclusionDistribution<2u, StressType>(ell, frac, C),
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

      virtual tfel::math::st2tost2<N, real> computeMeanLocalisator(
          const tfel::math::st2tost2<N, StressType> &C0,
          bool isotropic_matrix,
          int max_iter_anisotropic_integration = 12) override {
        tfel::math::st2tost2<N, StressType> Ci = this->stiffness;
        auto semiL = (this->inclusion).semiLengths;
        auto n_a_i = this->n_a;
        auto n_b_i = this->n_b;
        auto ai = semiL[0];
        auto bi = semiL[1];
        LengthType ci;
        if (N == 3) {
          ci = semiL[2];
        }
        if (isotropic_matrix) {
          auto pair0 = computeKappaMu<N, StressType>(C0);
          const auto kappa0 = std::get<0>(pair0);
          const auto mu0 = std::get<1>(pair0);
          const auto KG0 = KGModuli<StressType>(kappa0, mu0);
          if (N == 3) {
            return computeEllipsoidLocalisationTensor<StressType>(
                KG0, Ci, n_a_i, ai, n_b_i, bi, ci);
          } else if (N == 2) {
            return computeEllipticCylinderLocalisationTensor<StressType>(
                KG0, Ci, n_a, ai, bi);
          }
        } else {
          if (N == 3) {
            return computeAnisotropicLocalisationTensor<StressType>(
                C0, Ci, n_a_i, ai, n_b_i, bi, ci,
                max_iter_anisotropic_integration);
          } else if (N == 2) {
            return computePlainStrainAnisotropicLocalisationTensor<StressType>(
                C0, Ci, n_a_i, ai, bi, max_iter_anisotropic_integration);
          }
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
          std::cout << "the volume fraction of inclusions is too high !"
                    << std::endl;
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
          std::cout << "there are no more inclusions !" << std::endl;
          return 0;
        } else if ((this->number_of_phases) < i + 2) {
          std::cout << "there are less phases than what you think !"
                    << std::endl;
          return 0;
        } else {
          if ((this->number_of_phases) == 2) {
            std::cout << "you have removed the last inclusion phase !"
                      << std::endl;
          };
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

    /*!
     * This struct defines a polycrystal as a child of Microstructure. A
     * polycrystal is viewed as a vector of GrainPhase.
     * \tparam unsigned short int: dimension
     * \tparam StressType: type of the elastic constants related to the
     * inclusion
     */
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(
        tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                           StressType>()) struct Polycrystal
        : public Microstructure<N, StressType> {
      // attributes below
      using real = tfel::types::real<StressType>;
      using LengthType = tfel::types::length<StressType>;

      Polycrystal()
          : Microstructure<N, StressType>(),
            number_of_grains(0),
            total_fraction(real(0)),
            grains(std::vector<GrainPhase<N, StressType> *>({})){};

      virtual ~Polycrystal(){};

      int addGrain(GrainPhase<N, StressType> &grain) {
        if ((this->total_fraction) + grain.fraction > real(1)) {
          std::cout << "the volume fraction is too high !" << std::endl;
          return 0;
        } else {
          (this->number_of_grains)++;
          (this->total_fraction) += grain.fraction;
          (this->grains).push_back(&grain);
          return 1;
        };
      };

      int removeGrain(unsigned int i) {
        if ((this->number_of_grains) == 0) {
          std::cout << "there are no more grains !" << std::endl;
          return 0;
        } else if ((this->number_of_grains) < i + 1) {
          std::cout << "there are less grains than what you think !"
                    << std::endl;
          return 0;
        } else {
          if ((this->number_of_grains) == 1) {
            std::cout << "you have removed the last grain !" << std::endl;
          };
          (this->number_of_grains)--;
          (this->total_fraction) -= *(this->grains[i]).fraction;
          (this->grains).erase((this->grains).begin() + i);
        };
        return 1;
      };

      unsigned int get_number_of_grains() { return (this->number_of_grains); };

      real get_total_fraction() { return (this->total_fraction); };

      GrainPhase<N, StressType> *get_grain(unsigned int i) {
        if ((this->number_of_grains) < i + 1) {
          tfel::reportContractViolation(
              "there are less grains than what you think !");
        }
        return (this->grains)[i];
      };

     private:
      unsigned int number_of_grains;
      real total_fraction;
      std::vector<GrainPhase<N, StressType> *> grains;
    };

  }  // namespace homogenization::elasticity

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTUREDESCRIPTION_HXX */
