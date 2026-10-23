/*!
 * \file   Microstructure.cxx
 * \brief
 * \author Antoine MARTIN
 * \date 23/06/2025
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/MicrostructureDescription.hxx"

template <unsigned short int N, tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareInclusion(pybind11::module_& m,
                                                     const char* const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<N, LengthType>;
  pybind11::class_<I, std::shared_ptr<I>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a N (N=2 or N=3) dimensional inclusion (its "
      "childs are only geometrical ellipsoids, which can be particular like "
      "spheres, disks...)")
      .def_readwrite("semi_lengths", &I::semiLengths);
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareEllipse(pybind11::module_& m,
                                                   const char* const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<2u, LengthType>;
  using Elli = tfel::material::homogenization::elasticity::Ellipse<LengthType>;
  pybind11::class_<Elli, I, std::shared_ptr<Elli>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents an ellipse as a child of Inclusion<2>")
      .def(pybind11::init<const Elli&>())
      .def(pybind11::init<const LengthType&, const LengthType&>(),
           pybind11::arg("a"), pybind11::arg("b"))
      .def("a",
           [](Elli& elli) {
             const auto arr = elli.semiLengths;
             return arr[0];
           })
      .def("b", [](Elli& elli) {
        const auto arr = elli.semiLengths;
        return arr[1];
      });
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareDisk(pybind11::module_& m,
                                                const char* const n) {
  using Elli = tfel::material::homogenization::elasticity::Ellipse<LengthType>;
  using Di = tfel::material::homogenization::elasticity::Disk<LengthType>;
  pybind11::class_<Di, Elli, std::shared_ptr<Di>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a 2d disk as a child of Ellipse")
      .def(pybind11::init<const Di&>())
      .def(pybind11::init<>());
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareEllipsoid(pybind11::module_& m,
                                                     const char* const n) {
  using I =
      tfel::material::homogenization::elasticity::Inclusion<3u, LengthType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  pybind11::class_<Ell, I, std::shared_ptr<Ell>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents an ellipsoid as a child of Inclusion<3>")
      .def(pybind11::init<const Ell&>())
      .def(pybind11::init<const LengthType&, const LengthType&,
                          const LengthType&>(),
           pybind11::arg("a"), pybind11::arg("b"), pybind11::arg("c"));
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareSpheroid(pybind11::module_& m,
                                                    const char* const n) {
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  pybind11::class_<Sphe, Ell, std::shared_ptr<Sphe>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a 3d spheroid as a child of Ellipsoid. The "
      "second semi-length is shared by two axes")
      .def(pybind11::init<const Sphe&>())
      .def(pybind11::init<const LengthType&, const LengthType&>(),
           pybind11::arg("a"), pybind11::arg("b"))
      .def("axis_length",
           [](Sphe& sphe) {
             const auto arr = sphe.semiLengths;
             return arr[0];
           })
      .def("transverse_length", [](Sphe& sphe) {
        const auto arr = sphe.semiLengths;
        return arr[1];
      });
}

template <tfel::math::ScalarConcept LengthType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Length,
         LengthType>()) static void declareSphere(pybind11::module_& m,
                                                  const char* const n) {
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using Sph = tfel::material::homogenization::elasticity::Sphere<LengthType>;
  pybind11::class_<Sph, Sphe, std::shared_ptr<Sph>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a 3d sphere as a child of Spheroid")
      .def(pybind11::init<const Sph&>())
      .def(pybind11::init<>());
}

template <unsigned short int N, tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareInclusionDistribution(pybind11::
                                                                    module_& m,
                                                                const char* const
                                                                    n) {
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      N, StressType>;
  pybind11::class_<ID, std::shared_ptr<ID>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a distribution of inclusions as an abstract "
      "class, child of class Phase. It contains an inclusion and a pure "
      "virtual method computeMeanLocalisator which returns the mean "
      "localisator of the distribution of inclusions, for a given matrix "
      "elasticity in parameter.");
}

template <unsigned short int N, tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declarePhase(pybind11::module_& m,
                                                 const char* const n) {
  using Ph = tfel::material::homogenization::elasticity::Phase<N, StressType>;
  pybind11::class_<Ph, std::shared_ptr<Ph>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a phase. A phase has a volume fraction.");
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareSphereDistribution(pybind11::module_&
                                                                  m,
                                                              const char* const
                                                                  n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Sph = tfel::material::homogenization::elasticity::Sphere<LengthType>;
  using SD = tfel::material::homogenization::elasticity::SphereDistribution<
      StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<SD, ID, std::shared_ptr<SD>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a distribution of spheres, which is a child of\n"
      "InclusionDistribution<3u>, with inclusion of type Sphere.")
      .def(pybind11::init<const SD&>())
      .def(pybind11::init<const Sph&, real,
                          const tfel::math::st2tost2<3u, StressType>&>(),
           pybind11::arg("sph"), pybind11::arg("frac"), pybind11::arg("Ci"))
      .def(pybind11::init<const Sph&, real,
                          const tfel::material::IsotropicModuli<StressType>&>(),
           pybind11::arg("sph"), pybind11::arg("frac"), pybind11::arg("IMi"))
      .def_readwrite("inclusion", &SD::inclusion)
      .def_readwrite("fraction", &SD::fraction)
      .def("getElasticityOfPhase", &SD::getElasticityOfPhase)
      .def("changeElasticityOfPhase",
           [](SD& sd, const tfel::math::st2tost2<3u, StressType>& C) {
             return sd.changeElasticityOfPhase(C);
           },
           pybind11::arg("C"))
      .def("changeElasticityOfPhase",
           [](SD& sd, const tfel::material::IsotropicModuli<StressType>& IM) {
             return sd.changeElasticityOfPhase(IM);
           },
           pybind11::arg("IM"))
      .def("is_isotropic", &SD::isIsotropic)
      .def("isIsotropic", &SD::isIsotropic)
      .def("computeMeanLocalisator",
           [](SD& sd, const tfel::material::IsotropicModuli<StressType>& IM0) {
             return sd.computeMeanLocalisator(IM0);
           },
           pybind11::arg("IM0"))
      .def("computeDerivativesOfMeanLocalisator",
           [](SD& sd, const tfel::material::IsotropicModuli<StressType>& IM0,
              const std::array<real, 4>& dKG) {
             return sd.computeDerivativesOfMeanLocalisator(IM0, dKG);
           },
           pybind11::arg("IM0"), pybind11::arg("dKG"))
      .def(
          "computeMeanLocalisator",
          [](SD& sd, const tfel::math::st2tost2<3u, StressType>& C0,
             int max_iter_anisotropic_integration) {
            return sd.computeMeanLocalisator(C0,
                                             max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareIsotropicDistribution(pybind11::
                                                                    module_& m,
                                                                const char* const
                                                                    n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using IsoD =
      tfel::material::homogenization::elasticity::IsotropicDistribution<
          StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<IsoD, ID, std::shared_ptr<IsoD>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents an isotropic distribution of\n"
      "3d-ellipsoids, 3d-spheroids, as a child of\n"
      "InclusionDistribution<N>")
      .def(pybind11::init<const IsoD&>())
      .def(pybind11::init<const Ell&, real,
                          const tfel::material::IsotropicModuli<StressType>&>(),
           pybind11::arg("ell"), pybind11::arg("frac"), pybind11::arg("IMi"))
      .def(pybind11::init<const Sphe&, real,
                          const tfel::material::IsotropicModuli<StressType>&>(),
           pybind11::arg("sphero"), pybind11::arg("frac"),
           pybind11::arg("IMi"))
      .def_readwrite("inclusion", &IsoD::inclusion)
      .def_readwrite("fraction", &IsoD::fraction)
      .def("getElasticityOfPhase", &IsoD::getElasticityOfPhase)
      .def("changeElasticityOfPhase",
           [](IsoD& isod, const tfel::math::st2tost2<3u, StressType>& C) {
             return isod.changeElasticityOfPhase(C);
           },
           pybind11::arg("C"))
      .def("changeElasticityOfPhase",
           [](IsoD& isod,
              const tfel::material::IsotropicModuli<StressType>& IM) {
             return isod.changeElasticityOfPhase(IM);
           },
           pybind11::arg("IM"))
      .def("is_isotropic", &IsoD::isIsotropic)
      .def("isIsotropic", &IsoD::isIsotropic)
      .def("computeMeanLocalisator",
           [](IsoD& isod,
              const tfel::material::IsotropicModuli<StressType>& IM0) {
             return isod.computeMeanLocalisator(IM0);
           },
           pybind11::arg("IM0"))
      .def(
          "computeDerivativesOfMeanLocalisator",
          [](IsoD& isod, const tfel::material::IsotropicModuli<StressType>& IM0,
             const std::array<real, 4>& dKG) {
            return isod.computeDerivativesOfMeanLocalisator(IM0, dKG);
          },
          pybind11::arg("IM0"), pybind11::arg("dKG"))
      .def(
          "computeMeanLocalisator",
          [](IsoD& isod, const tfel::math::st2tost2<3u, StressType>& C0,
             int max_iter_anisotropic_integration) {
            return isod.computeMeanLocalisator(
                C0, max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareTransverseIsotropicDistribution(pybind11::
                                                                              module_&
                                                                                  m,
                                                                          const char* const
                                                                              n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using TID = tfel::material::homogenization::elasticity::
      TransverseIsotropicDistribution<StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<TID, ID, std::shared_ptr<TID>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a transverse isotropic distribution of "
      "inclusions in 3d, which can be only Spheroids or Ellipsoids. The "
      "vector $n$ is the axis of transverse isotropy. The index corresponds "
      "to the index of the semi-length which corresponds to the axis of "
      "transverse isotropy (axis which does not rotate). This index can "
      "hence be 0, 1 or 2.")
      .def(pybind11::init<const TID&>())
      .def(pybind11::init<
           const Ell&, real, const tfel::material::IsotropicModuli<StressType>&,
           const tfel::math::tvector<3, real>&, unsigned short int&>(),
           pybind11::arg("ell"), pybind11::arg("frac"), pybind11::arg("IM"),
           pybind11::arg("n_"), pybind11::arg("ind"))
      .def(pybind11::init<const Sphe&, real,
                          const tfel::material::IsotropicModuli<StressType>&,
                          const tfel::math::tvector<3, real>&,
                          unsigned short int&>(),
           pybind11::arg("sphero"), pybind11::arg("frac"), pybind11::arg("IM"),
           pybind11::arg("n_"), pybind11::arg("ind"))
      .def_readwrite("inclusion", &TID::inclusion)
      .def_readwrite("fraction", &TID::fraction)
      .def("getElasticityOfPhase", &TID::getElasticityOfPhase)
      .def("changeElasticityOfPhase",
           [](TID& tid, const tfel::math::st2tost2<3u, StressType>& C) {
             return tid.changeElasticityOfPhase(C);
           },
           pybind11::arg("C"))
      .def(
          "changeElasticityOfPhase",
          [](TID& tid, const tfel::material::IsotropicModuli<StressType>& IM) {
            return tid.changeElasticityOfPhase(IM);
          },
          pybind11::arg("IM"))
      .def("is_isotropic", &TID::isIsotropic)
      .def("isIsotropic", &TID::isIsotropic)
      .def(
          "computeMeanLocalisator",
          [](TID& tid, const tfel::material::IsotropicModuli<StressType>& IM0) {
            return tid.computeMeanLocalisator(IM0);
          },
          pybind11::arg("IM0"))
      .def("computeDerivativesOfMeanLocalisator",
           [](TID& tid, const tfel::material::IsotropicModuli<StressType>& IM0,
              const std::array<real, 4>& dKG) {
             return tid.computeDerivativesOfMeanLocalisator(IM0, dKG);
           },
           pybind11::arg("IM0"), pybind11::arg("dKG"))
      .def(
          "computeMeanLocalisator",
          [](TID& tid, const tfel::math::st2tost2<3u, StressType>& C0,
             int max_iter_anisotropic_integration) {
            return tid.computeMeanLocalisator(C0,
                                              max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareOrientedDistribution(pybind11::
                                                                   module_& m,
                                                               const char* const
                                                                   n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using OD = tfel::material::homogenization::elasticity::OrientedDistribution<
      StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<OD, ID, std::shared_ptr<OD>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a distribution of inclusions with a unique\n"
      "orientation in 3d, which can be only Spheroids or Ellipsoids.\n"
      "The vectors $n_a$ and $n_b$ define the orientation.\n"
      "The first semi-length of the inclusion corresponds to $n_a$. The\n"
      "second semi-length corresponds to $n_b$.")
      .def(pybind11::init<const OD&>())
      .def(pybind11::init<const Ell&, real,
                          const tfel::math::st2tost2<3, StressType>&,
                          const tfel::math::tvector<3, real>&,
                          const tfel::math::tvector<3, real>&>(),
           pybind11::arg("ell"), pybind11::arg("frac"), pybind11::arg("C"),
           pybind11::arg("n_a_"), pybind11::arg("n_b_"))
      .def(pybind11::init<const Sphe&, real,
                          const tfel::math::st2tost2<3, StressType>&,
                          const tfel::math::tvector<3, real>&,
                          const tfel::math::tvector<3, real>&>(),
           pybind11::arg("sphero"), pybind11::arg("frac"), pybind11::arg("C"),
           pybind11::arg("n_a_"), pybind11::arg("n_b_"))
      .def(pybind11::init<const Ell&, real,
                          const tfel::material::IsotropicModuli<StressType>&,
                          const tfel::math::tvector<3, real>&,
                          const tfel::math::tvector<3, real>&>(),
           pybind11::arg("ell"), pybind11::arg("frac"), pybind11::arg("IM"),
           pybind11::arg("n_a_"), pybind11::arg("n_b_"))
      .def(pybind11::init<const Sphe&, real,
                          const tfel::material::IsotropicModuli<StressType>&,
                          const tfel::math::tvector<3, real>&,
                          const tfel::math::tvector<3, real>&>(),
           pybind11::arg("sphero"), pybind11::arg("frac"), pybind11::arg("IM"),
           pybind11::arg("n_a_"), pybind11::arg("n_b_"))
      .def_readwrite("inclusion", &OD::inclusion)
      .def_readwrite("fraction", &OD::fraction)
      .def("getElasticityOfPhase", &OD::getElasticityOfPhase)
      .def("changeElasticityOfPhase",
           [](OD& od, const tfel::math::st2tost2<3u, StressType>& C) {
             return od.changeElasticityOfPhase(C);
           },
           pybind11::arg("C"))
      .def("changeElasticityOfPhase",
           [](OD& od, const tfel::material::IsotropicModuli<StressType>& IM) {
             return od.changeElasticityOfPhase(IM);
           },
           pybind11::arg("IM"))
      .def("is_isotropic", &OD::isIsotropic)
      .def("isIsotropic", &OD::isIsotropic)
      .def("computeMeanLocalisator",
           [](OD& od, const tfel::material::IsotropicModuli<StressType>& IM0) {
             return od.computeMeanLocalisator(IM0);
           },
           pybind11::arg("IM0"))
      .def("computeDerivativesOfMeanLocalisator",
           [](OD& od, const tfel::material::IsotropicModuli<StressType>& IM0,
              const std::array<real, 4>& dKG) {
             return od.computeDerivativesOfMeanLocalisator(IM0, dKG);
           },
           pybind11::arg("IM0"), pybind11::arg("dKG"))
      .def(
          "computeMeanLocalisator",
          [](OD& od, const tfel::math::st2tost2<3u, StressType>& C0,
             int max_iter_anisotropic_integration) {
            return od.computeMeanLocalisator(C0,
                                             max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareUserDefinedDistributionOfSpheroids(pybind11::
                                                                                 module_&
                                                                                     m,
                                                                             const char* const
                                                                                 n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Sphe = tfel::material::homogenization::elasticity::Spheroid<LengthType>;
  using UDDS = tfel::material::homogenization::elasticity::
      UserDefinedDistributionOfSpheroids<StressType>;
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      3u, StressType>;

  pybind11::class_<UDDS, ID, std::shared_ptr<UDDS>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a distribution of spheroidal inclusions\n"
      "whose orientation tensors A2 and A4 are given.\n"
      "A2 is the average of n^n and A4 the average of n^n^n^n.")
      .def(pybind11::init<const UDDS&>())
      .def(pybind11::init<const Sphe&, real,
                          const tfel::material::IsotropicModuli<StressType>&,
                          const tfel::math::stensor<3, real>&,
                          const tfel::math::st2tost2<3, real>&>(),
           pybind11::arg("sphero"), pybind11::arg("frac"), pybind11::arg("IMi"),
           pybind11::arg("A2_"), pybind11::arg("A4_"))
      .def_readwrite("inclusion", &UDDS::inclusion)
      .def_readwrite("fraction", &UDDS::fraction)
      .def("getElasticityOfPhase", &UDDS::getElasticityOfPhase)
      .def("changeElasticityOfPhase",
           [](UDDS& udds, const tfel::math::st2tost2<3u, StressType>& C) {
             return udds.changeElasticityOfPhase(C);
           },
           pybind11::arg("C"))
      .def("changeElasticityOfPhase",
           [](UDDS& udds,
              const tfel::material::IsotropicModuli<StressType>& IM) {
             return udds.changeElasticityOfPhase(IM);
           },
           pybind11::arg("IM"))
      .def("is_isotropic", &UDDS::is_isotropic)
      .def("isIsotropic", &UDDS::isIsotropic)
      .def("computeMeanLocalisator",
           [](UDDS& udds,
              const tfel::material::IsotropicModuli<StressType>& IM0) {
             return udds.computeMeanLocalisator(IM0);
           },
           pybind11::arg("IM0"))
      .def(
          "computeDerivativesOfMeanLocalisator",
          [](UDDS& udds, const tfel::material::IsotropicModuli<StressType>& IM0,
             const std::array<real, 4>& dKG) {
            return udds.computeDerivativesOfMeanLocalisator(IM0, dKG);
          },
          pybind11::arg("IM0"), pybind11::arg("dKG"))
      .def(
          "computeMeanLocalisator",
          [](UDDS& udds, const tfel::math::st2tost2<3u, StressType>& C0,
             int max_iter_anisotropic_integration) {
            return udds.computeMeanLocalisator(
                C0, max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declareGrain(pybind11::module_& m,
                                                 const char* const n) {
  using real = tfel::types::real<StressType>;
  using LengthType = tfel::types::length<StressType>;
  using Ell = tfel::material::homogenization::elasticity::Ellipsoid<LengthType>;

  using OD = tfel::material::homogenization::elasticity::OrientedDistribution<
      StressType>;
  using Gr = tfel::material::homogenization::elasticity::Grain<StressType>;

  pybind11::class_<Gr, OD, std::shared_ptr<Gr>>(
      m, n, pybind11::buffer_protocol(),
      "This struct represents a grain of a polycrystal,\n"
      "as a child of class OrientedDistribution.")
      .def(pybind11::init<const Gr&>())
      .def(pybind11::init<const Ell&, real,
                          const tfel::material::IsotropicModuli<StressType>&,
                          const tfel::math::tvector<3, real>&,
                          const tfel::math::tvector<3, real>&>(),
           pybind11::arg("ell"), pybind11::arg("frac"), pybind11::arg("IM"),
           pybind11::arg("n_a_"), pybind11::arg("n_b_"))
      .def(pybind11::init<const Ell&, real,
                          const tfel::math::st2tost2<3u, StressType>&,
                          const tfel::math::tvector<3, real>&,
                          const tfel::math::tvector<3, real>&>(),
           pybind11::arg("ell"), pybind11::arg("frac"), pybind11::arg("C"),
           pybind11::arg("n_a_"), pybind11::arg("n_b_"))
      .def_readwrite("inclusion", &Gr::inclusion)
      .def_readwrite("n_a", &Gr::n_a)
      .def_readwrite("n_b", &Gr::n_b)
      .def_readwrite("fraction", &Gr::fraction)
      .def("getElasticityOfPhase", &Gr::getElasticityOfPhase)
      .def("changeElasticityOfPhase",
           [](Gr& gr, const tfel::math::st2tost2<3u, StressType>& C) {
             return gr.changeElasticityOfPhase(C);
           },
           pybind11::arg("C"))
      .def("changeElasticityOfPhase",
           [](Gr& gr, const tfel::material::IsotropicModuli<StressType>& IM) {
             return gr.changeElasticityOfPhase(IM);
           },
           pybind11::arg("IM"))
      .def("isIsotropic", &Gr::isIsotropic)
      .def("computeMeanLocalisator",
           [](Gr& gr, const tfel::material::IsotropicModuli<StressType>& IM0) {
             return gr.computeMeanLocalisator(IM0);
           },
           pybind11::arg("IM0"))
      .def("computeDerivativesOfMeanLocalisator",
           [](Gr& gr, const tfel::material::IsotropicModuli<StressType>& IM0,
              const std::array<real, 4>& dKG) {
             return gr.computeDerivativesOfMeanLocalisator(IM0, dKG);
           },
           pybind11::arg("IM0"), pybind11::arg("dKG"))
      .def(
          "computeMeanLocalisator",
          [](Gr& gr, const tfel::math::st2tost2<3u, StressType>& C0,
             int max_iter_anisotropic_integration) {
            return gr.computeMeanLocalisator(C0,
                                             max_iter_anisotropic_integration);
          },
          pybind11::arg("C0"),
          pybind11::arg("max_iter_anisotropic_integration") = 12);
}

template <unsigned short int N, tfel::math::ScalarConcept StressType>
requires(
    tfel::math::checkUnitCompatibility<
        tfel::math::unit::Stress,
        StressType>()) static void declareParticulateMicrostructure(pybind11::
                                                                        module_&
                                                                            m,
                                                                    const char* const
                                                                        n) {
  using ID = tfel::material::homogenization::elasticity::InclusionDistribution<
      N, StressType>;

  using PM =
      tfel::material::homogenization::elasticity::ParticulateMicrostructure<
          N, StressType>;

  pybind11::class_<PM>(
      m, n, pybind11::buffer_protocol(),
      "This struct defines a matrix-inclusion microstructure (or particulate\n"
      "microstructure) as a child of Microstructure. This type of\n"
      "microstructure is viewed as an array of InclusionDistribution, with a\n"
      "matrix_phase.")
      .def(pybind11::init<const tfel::math::st2tost2<N, StressType>&>(),
           pybind11::arg("C0"))
      .def(pybind11::init<const tfel::material::IsotropicModuli<StressType>&>(),
           pybind11::arg("IM0"))
      .def("addInclusionPhase", &PM::addInclusionPhase,
           pybind11::arg("inclusionPhase"))
      .def("removeInclusionPhase", &PM::removeInclusionPhase,
           pybind11::arg("i"))
      .def("changeElasticityOfMatrixPhase",
           [](PM& pm, const tfel::math::st2tost2<N, StressType>& C0) {
             return pm.changeElasticityOfMatrixPhase(C0);
           },
           pybind11::arg("C0"))
      .def("changeElasticityOfMatrixPhase",
           [](PM& pm, const tfel::material::IsotropicModuli<StressType>& IM0) {
             return pm.changeElasticityOfMatrixPhase(IM0);
           },
           pybind11::arg("IM0"))
      .def("changeElasticityOfInclusionPhase",
           [](PM& pm, unsigned int i,
              const tfel::math::st2tost2<N, StressType>& C) {
             return pm.changeElasticityOfInclusionPhase(i, C);
           },
           pybind11::arg("i"), pybind11::arg("C"))
      .def("changeElasticityOfInclusionPhase",
           [](PM& pm, unsigned int i,
              const tfel::material::IsotropicModuli<StressType>& IM) {
             return pm.changeElasticityOfInclusionPhase(i, IM);
           },
           pybind11::arg("i"), pybind11::arg("IM"))
      .def("changeFractionOfInclusionPhase",
           &PM::changeFractionOfInclusionPhase, pybind11::arg("i"),
           pybind11::arg("f"))
      .def("get_number_of_phases", &PM::getNumberOfPhases)
      .def("getNumberOfPhases", &PM::getNumberOfPhases)
      .def("get_matrix_fraction", &PM::getMatrixFraction)
      .def("getMatrixFraction", &PM::getMatrixFraction)
      .def("get_matrix_elasticity", &PM::getMatrixElasticity)
      .def("getMatrixElasticity", &PM::getMatrixElasticity)
      .def("is_isotropic_matrix", &PM::isIsotropicMatrix)
      .def("isIsotropicMatrix", &PM::isIsotropicMatrix)
      .def("get_inclusionPhase",
           [](PM& pm, unsigned int i) {
             return std::shared_ptr<ID>(pm.getInclusionPhase(i));
           },
           pybind11::arg("i"))
      .def("getInclusionPhase",
           [](PM& pm, unsigned int i) {
             return std::shared_ptr<ID>(pm.getInclusionPhase(i));
           },
           pybind11::arg("i"));
}

template <tfel::math::ScalarConcept StressType>
requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress,
         StressType>()) static void declarePolycrystal(pybind11::module_& m,
                                                       const char* const n) {
  using Gr = tfel::material::homogenization::elasticity::Grain<StressType>;

  using Po =
      tfel::material::homogenization::elasticity::Polycrystal<StressType>;

  pybind11::class_<Po>(
      m, n, pybind11::buffer_protocol(),
      "This struct defines a polycrystalline microstructure\n"
      "as a child of Microstructure. This type of microstructure\n"
      "is viewed as an array of Grains.")
      .def(pybind11::init<>())
      .def("addGrain", &Po::addGrain, pybind11::arg("grain"))
      .def("removeGrain", &Po::removeGrain, pybind11::arg("i"))
      .def("changeElasticityOfGrain",
           [](Po& po, unsigned int i,
              const tfel::math::st2tost2<3u, StressType>& C) {
             return po.changeElasticityOfGrain(i, C);
           },
           pybind11::arg("i"), pybind11::arg("C"))
      .def("changeElasticityOfGrain",
           [](Po& po, unsigned int i,
              const tfel::material::IsotropicModuli<StressType>& IM) {
             return po.changeElasticityOfGrain(i, IM);
           },
           pybind11::arg("i"), pybind11::arg("IM"))
      .def("changeFractionOfGrain", &Po::changeFractionOfGrain,
           pybind11::arg("i"), pybind11::arg("f"))
      .def("getNumberOfGrains", &Po::getNumberOfGrains)
      .def("getTotalFraction", &Po::getTotalFraction)
      .def("getGrain",
           [](Po& po, unsigned int i) {
             return std::shared_ptr<Gr>(po.getGrain(i));
           },
           pybind11::arg("i"));
}

void declareMicrostructure(pybind11::module_&);

void declareMicrostructure(pybind11::module_& m) {
  declareInclusion<3u, double>(m, "Inclusion3D");
  declareInclusion<2u, double>(m, "Inclusion2D");
  declareEllipse<double>(m, "Ellipse");
  declareDisk<double>(m, "Disk");
  declareEllipsoid<double>(m, "Ellipsoid");
  declareSpheroid<double>(m, "Spheroid");
  declareSphere<double>(m, "Sphere");
  declareInclusionDistribution<3u, double>(m, "InclusionDistribution");
  declareSphereDistribution<double>(m, "SphereDistribution");
  declareIsotropicDistribution<double>(m, "IsotropicDistribution");
  declareTransverseIsotropicDistribution<double>(
      m, "TransverseIsotropicDistribution");
  declareOrientedDistribution<double>(m, "OrientedDistribution");
  declareUserDefinedDistributionOfSpheroids<double>(
      m, "UserDefinedDistributionOfSpheroids");
  declarePhase<3u, double>(m, "Phase");
  declareGrain<double>(m, "Grain");
  declareParticulateMicrostructure<3, double>(m, "ParticulateMicrostructure");
  declarePolycrystal<double>(m, "Polycrystal");
}
