/*!
 * \file   mfront/mtest/CastemFiniteStrainBehaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  18 november 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Tensor/TensorConceptIO.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"

#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/T2toST2/T2toST2View.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/Castem/Castem.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/BehaviourSymmetryType.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"
#include "MTest/CastemFiniteStrainBehaviour.hxx"

namespace mtest {

  template <unsigned short N>
  static void computeTangentOperator(
      tfel::math::matrix<castem::CastemReal>& K,
      const tfel::math::matrix<castem::CastemReal>& D,
      const castem::CastemReal* const e0,
      const castem::CastemReal* const e1,
      const castem::CastemReal* const s1) {
    using namespace tfel::math;
    using namespace tfel::material;
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    using castem::CastemReal;
    tensor<N, CastemReal> F0(e0);
    tensor<N, CastemReal> F1(e1);
    stensor<N, CastemReal> s(s1);
    st2tost2<N, CastemReal> C;
    UmatNormaliseTangentOperator::exe(&C(0, 0), D, N);
    T2toST2View<N, CastemReal> Kv(&K(0, 0));
    Kv = convert<TangentOperator::DSIG_DF, TangentOperator::C_TRUESDELL>(C, F0,
                                                                         F1, s);
  }  // end of computeTangentOperator<N>

  /*!
   * This structure is in charge of computing a stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   * \param[in] H: modelling hypothesis
   * \param[in] S: material symmetry
   */
  template <tfel::material::ModellingHypothesis::Hypothesis,
            mfront::BehaviourSymmetryType>
  struct MTestCastemComputeFiniteStrainStiffnessTensor;

  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<H, mfront::ISOTROPIC> {
    static constexpr const unsigned short N =
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    static constexpr const tfel::material::
        StiffnessTensorAlterationCharacteristic STAC =
            tfel::material::GetDefaultStiffnessTensorAlterationCharacteristic<
                H>::value;
    static void exe(tfel::math::T2toST2View<N, real>& C,
                    const real* const props) {
      using namespace tfel::material;
      using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
      tfel::math::st2tost2<N, real> D;
      computeIsotropicStiffnessTensorII<N, STAC>(D, props[0], props[1]);
      const auto id = tfel::math::tensor<N, real>::Id();
      const auto s = tfel::math::stensor<N, real>(real(0));
      C = convert<TangentOperator::DSIG_DF, TangentOperator::C_TRUESDELL>(
          D, id, id, s);
    }  // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  //! an helper structure for finite strain orthotropic stiffness tensor
  template <unsigned short N>
  struct MTestCastemComputeFiniteStrainOrhtotropicStiffnessTensorBase;

  template <>
  struct MTestCastemComputeFiniteStrainOrhtotropicStiffnessTensorBase<1u> {
    template <tfel::material::StiffnessTensorAlterationCharacteristic STAC>
    static tfel::math::st2tost2<1u, real> getStiffnessTensor(
        const real* const props) {
      using namespace tfel::material;
      tfel::math::st2tost2<1u, real> C;
      computeOrthotropicStiffnessTensorII<1u, STAC>(
          C, props[0], props[1], props[2], props[3], props[4], props[5],
          props[0], props[1], props[2]);
      return C;
    }  // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template <>
  struct MTestCastemComputeFiniteStrainOrhtotropicStiffnessTensorBase<2u> {
    template <tfel::material::StiffnessTensorAlterationCharacteristic STAC>
    static tfel::math::st2tost2<2u, real> getStiffnessTensor(
        const real* const props) {
      using namespace tfel::material;
      tfel::math::st2tost2<2u, real> C;
      computeOrthotropicStiffnessTensorII<2u, STAC>(
          C, props[0], props[1], props[2], props[3], props[4], props[5],
          props[6], props[6], props[6]);
      return C;
    }  // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template <>
  struct MTestCastemComputeFiniteStrainOrhtotropicStiffnessTensorBase<3u> {
    template <tfel::material::StiffnessTensorAlterationCharacteristic STAC>
    static tfel::math::st2tost2<3u, real> getStiffnessTensor(
        const real* const props) {
      using namespace tfel::material;
      tfel::math::st2tost2<3u, real> C;
      computeOrthotropicStiffnessTensorII<3u, STAC>(
          C, props[0], props[1], props[2], props[3], props[4], props[5],
          props[6], props[7], props[8]);
      return C;
    }  // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  template <tfel::material::ModellingHypothesis::Hypothesis H>
  using MTestCastemComputeFiniteStrainOrhtotropicStiffnessTensorBaseII =
      MTestCastemComputeFiniteStrainOrhtotropicStiffnessTensorBase<
          tfel::material::ModellingHypothesisToSpaceDimension<H>::value>;

  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct MTestCastemComputeFiniteStrainStiffnessTensor<H, mfront::ORTHOTROPIC>
      : public MTestCastemComputeFiniteStrainOrhtotropicStiffnessTensorBaseII<
            H> {
    using Base =
        MTestCastemComputeFiniteStrainOrhtotropicStiffnessTensorBaseII<H>;
    static constexpr const unsigned short N =
        tfel::material::ModellingHypothesisToSpaceDimension<H>::value;
    static constexpr const tfel::material::
        StiffnessTensorAlterationCharacteristic STAC =
            tfel::material::GetDefaultStiffnessTensorAlterationCharacteristic<
                H>::value;
    static void exe(tfel::math::T2toST2View<N, real>& C,
                    const tfel::math::tmatrix<3u, 3u, real>& r,
                    const real* const props) {
      using namespace tfel::material;
      using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
      auto D = tfel::math::change_basis(
          Base::template getStiffnessTensor<STAC>(props),
          tfel::math::transpose(r));
      const auto id = tfel::math::tensor<N, real>::Id();
      const auto s = tfel::math::stensor<N, real>(real(0));
      C = convert<TangentOperator::DSIG_DF, TangentOperator::C_TRUESDELL>(
          D, id, id, s);
    }  // end of struct MTestCastemComputeFiniteStrainStiffnessTensor
  };

  static tfel::material::ModellingHypothesis::Hypothesis
  getEffectiveModellingHypothesis(
      const tfel::material::ModellingHypothesis::Hypothesis& h,
      const std::string& l,
      const std::string& b) {
    using namespace tfel::material;
    using ELM = tfel::system::ExternalLibraryManager;
    if (h == ModellingHypothesis::PLANESTRESS) {
      auto& elm = ELM::getExternalLibraryManager();
      if (elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l, b)) {
        return ModellingHypothesis::GENERALISEDPLANESTRAIN;
      }
    }
    return h;
  }  // end of getEffectiveModellingHypothesis

  CastemFiniteStrainBehaviour::CastemFiniteStrainBehaviour(const Hypothesis h,
                                                           const std::string& l,
                                                           const std::string& b)
      : CastemStandardBehaviour(
            getEffectiveModellingHypothesis(h, l, b), l, b) {
    if (h == ModellingHypothesis::PLANESTRESS) {
      auto& elm =
          tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      if (elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l, b)) {
        this->usesGenericPlaneStressAlgorithm = true;
        //! better name required
        this->ivnames.push_back("AxialStrain");
        this->ivtypes.push_back(0);
      }
    }
  }  // end of CastemFiniteStrainBehaviour::CastemFiniteStrainBehaviour

  CastemFiniteStrainBehaviour::CastemFiniteStrainBehaviour(
      const UmatBehaviourDescription& umb)
      : CastemStandardBehaviour(umb) {
  }  // end of CastemFiniteStrainBehaviour::CastemFiniteStrainBehaviour

  void CastemFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    v[0] = real(1);
    v[1] = real(1);
    v[2] = real(1);
    std::fill(v.begin() + 3, v.end(), real(0));
  }  // end of
     // CastemFiniteStrainBehaviour::setDrivingVariablesDefaultInitialValue

  std::pair<bool, real> CastemFiniteStrainBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    // rotation matrix
    if (ktype == StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES) {
      // compute the stiffness operator from material properties
      const auto rt = transpose(s.r);
      this->computeElasticStiffness(wk.kt, s.mprops1, rt);
      return {true, 1};
    }
    tfel::raise(
        "CastemFiniteStrainBehaviour::computePredictionOperator : "
        "computation of the tangent operator "
        "is not supported");
  }  // end of CastemFiniteStrainBehaviour::computePredictionOperator

  std::pair<bool, real> CastemFiniteStrainBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    using namespace std;
    using namespace tfel::math;
    using namespace castem;
    using tfel::math::vector;
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->usesGenericPlaneStressAlgorithm
                       ? ModellingHypothesis::PLANESTRESS
                       : this->getHypothesis();
    this->buildMaterialProperties(wk, s);
    const auto nprops = static_cast<CastemInt>(wk.mps.size());
    CastemInt ntens, ndi, nstatv;
    if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      ndi = 14;
      ntens = 3;
    } else if (h == ModellingHypothesis::AXISYMMETRICAL) {
      ndi = 0;
      ntens = 4;
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      ndi = -2;
      ntens = 4;
    } else if (h == ModellingHypothesis::PLANESTRAIN) {
      ndi = -1;
      ntens = 4;
    } else if (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) {
      ndi = -3;
      ntens = 4;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      ndi = 2;
      ntens = 6;
    } else {
      tfel::raise(
          "CastemFiniteStrainBehaviour::integrate: "
          "unsupported hypothesis");
    }
    tfel::raise_if((wk.D.getNbRows() != ntens) || (wk.D.getNbCols() != ntens),
                   "CastemFiniteStrainBehaviour::integrate: "
                   "the memory has not been allocated correctly");
    tfel::raise_if(((s.iv0.size() == 0) && (wk.ivs.size() != 1u)) ||
                       ((s.iv0.size() != 0) && (s.iv0.size() != wk.ivs.size())),
                   "CastemFiniteStrainBehaviour::integrate: "
                   "the memory has not been allocated correctly");
    if (s.iv0.size() != 0) {
      std::copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    }
    nstatv = static_cast<CastemInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u, 3u, real> drot = transpose(s.r);
    CastemInt kinc(1);
    tmatrix<3u, 3u, real> uu0(real(0));
    tmatrix<3u, 3u, real> uu1(real(0));
    uu0(0, 0) = s.e0(0);
    uu1(0, 0) = s.e1(0);
    uu0(1, 1) = s.e0(1);
    uu1(1, 1) = s.e1(1);
    uu0(2, 2) = s.e0(2);
    uu1(2, 2) = s.e1(2);
    if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      uu0(1, 0) = 0.;
      uu1(1, 0) = 0.;
      uu0(0, 1) = 0.;
      uu1(0, 1) = 0.;
      uu0(2, 0) = 0.;
      uu1(2, 0) = 0.;
      uu0(0, 2) = 0.;
      uu1(0, 2) = 0.;
      uu0(2, 1) = 0.;
      uu1(2, 1) = 0.;
      uu0(1, 2) = 0.;
      uu1(1, 2) = 0.;
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h == ModellingHypothesis::PLANESTRESS) ||
               (h == ModellingHypothesis::PLANESTRAIN) ||
               (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      // uu0 and uu1 must be built using Fortran notations
      uu0(1, 0) = s.e0(3);
      uu1(1, 0) = s.e1(3);
      uu0(0, 1) = s.e0(4);
      uu1(0, 1) = s.e1(4);
      uu0(2, 0) = 0.;
      uu1(2, 0) = 0.;
      uu0(0, 2) = 0.;
      uu1(0, 2) = 0.;
      uu0(2, 1) = 0.;
      uu1(2, 1) = 0.;
      uu0(1, 2) = 0.;
      uu1(1, 2) = 0.;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      // uu0 and uu1 must be built using Fortran notations
      uu0(1, 0) = s.e0(3);
      uu1(1, 0) = s.e1(3);
      uu0(0, 1) = s.e0(4);
      uu1(0, 1) = s.e1(4);
      uu0(2, 0) = s.e0(5);
      uu1(2, 0) = s.e1(5);
      uu0(0, 2) = s.e0(6);
      uu1(0, 2) = s.e1(6);
      uu0(2, 1) = s.e0(7);
      uu1(2, 1) = s.e1(7);
      uu0(1, 2) = s.e0(8);
      uu1(1, 2) = s.e1(8);
    } else {
      tfel::raise(
          "CastemFiniteStrainBehaviour::integrate: "
          "unsupported hypothesis");
    }
    copy(s.s0.begin(), s.s0.end(), s.s1.begin());
    for (tmatrix<3u, 3u, real>::size_type i = 3;
         i != static_cast<unsigned short>(ntens); ++i) {
      s.s1(i) /= sqrt2;
    }
    fill(wk.D.begin(), wk.D.end(), CastemReal(0));
    // choosing the type of stiffness matrix
    UmatBehaviourBase::initializeTangentOperator(wk.D, ktype, true);
    CastemReal ndt = std::numeric_limits<CastemReal>::max();
    const auto name = this->getBehaviourNameForUMATFunctionCall();
    (this->fct)(&(s.s1(0)), &(wk.ivs(0)), &(wk.D(0, 0)), nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                nullptr, &dt, &(s.esv0(0)), &(s.desv(0)), &(s.esv0(0)) + 1,
                &(s.desv(0)) + 1, name, &ndi, nullptr, &ntens, &nstatv,
                &(wk.mps(0)), &nprops, nullptr, &drot(0, 0), &ndt, nullptr,
                &uu0(0, 0), &uu1(0, 0), nullptr, nullptr, nullptr, nullptr,
                nullptr, &kinc, 0);
    if (kinc != 1) {
      return {false, ndt};
    }
    // saving state variables
    if (!s.iv1.empty()) {
      copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
    }
    // turning things in standard conventions
    for (vector<real>::size_type i = 3; i != static_cast<unsigned short>(ntens);
         ++i) {
      s.s1(i) *= sqrt2;
    }
    // tangent operator (...)
    if (ktype != StiffnessMatrixType::NOSTIFFNESS) {
      if (ktype == StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES) {
        this->computeElasticStiffness(wk.k, s.mprops1, drot);
      } else {
        // from C_TRUESDELL rate to DSIG_DF
        if (ntens == 3) {
          computeTangentOperator<1u>(wk.k, wk.D, &s.e0(0), &s.e1(0), &s.s1(0));
        } else if (ntens == 4) {
          computeTangentOperator<2u>(wk.k, wk.D, &s.e0(0), &s.e1(0), &s.s1(0));
        } else if (ntens == 6) {
          computeTangentOperator<3u>(wk.k, wk.D, &s.e0(0), &s.e1(0), &s.s1(0));
        } else {
          tfel::raise(
              "CastemFiniteStrainBehaviour::integrate: "
              "unsupported dimension");
        }
      }
    }
    return {true, ndt};
  }  // end of CastemFiniteStrainBehaviour::integrate

  void CastemFiniteStrainBehaviour::computeElasticStiffness(
      tfel::math::matrix<real>& Kt,
      const tfel::math::vector<real>& mp,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    using namespace tfel::math;
    const auto h = this->getHypothesis();
    if (this->stype == 0u) {
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        T2toST2View<1u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
            mfront::ISOTROPIC>::exe(De, &mp(0));
      } else if (h == ModellingHypothesis::AXISYMMETRICAL) {
        T2toST2View<2u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::AXISYMMETRICAL,
            mfront::ISOTROPIC>::exe(De, &mp(0));
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        T2toST2View<2u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::PLANESTRESS, mfront::ISOTROPIC>::exe(De,
                                                                      &mp(0));
      } else if (h == ModellingHypothesis::PLANESTRAIN) {
        T2toST2View<2u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::PLANESTRAIN, mfront::ISOTROPIC>::exe(De,
                                                                      &mp(0));
      } else if (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) {
        T2toST2View<2u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::GENERALISEDPLANESTRAIN,
            mfront::ISOTROPIC>::exe(De, &mp(0));
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        T2toST2View<3u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::TRIDIMENSIONAL,
            mfront::ISOTROPIC>::exe(De, &mp(0));
      } else {
        tfel::raise(
            "CastemFiniteStrainBehaviour::computeElasticMatrix: "
            "unsupported hypothesis");
      }
    } else if (this->stype == 1u) {
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        T2toST2View<1u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
            mfront::ORTHOTROPIC>::exe(De, r, &mp(0));
      } else if (h == ModellingHypothesis::AXISYMMETRICAL) {
        T2toST2View<2u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::AXISYMMETRICAL,
            mfront::ORTHOTROPIC>::exe(De, r, &mp(0));
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        T2toST2View<2u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::PLANESTRESS, mfront::ORTHOTROPIC>::exe(De, r,
                                                                        &mp(0));
      } else if (h == ModellingHypothesis::PLANESTRAIN) {
        T2toST2View<2u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::PLANESTRAIN, mfront::ORTHOTROPIC>::exe(De, r,
                                                                        &mp(0));
      } else if (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) {
        T2toST2View<2u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::GENERALISEDPLANESTRAIN,
            mfront::ORTHOTROPIC>::exe(De, r, &mp(0));
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        T2toST2View<3u, real> De(&Kt(0, 0));
        MTestCastemComputeFiniteStrainStiffnessTensor<
            ModellingHypothesis::TRIDIMENSIONAL,
            mfront::ORTHOTROPIC>::exe(De, r, &mp(0));
      } else {
        tfel::raise(
            "CastemFiniteStrainBehaviour::computeElasticMatrix: "
            "unsupported modelling hypothesis");
      }
    } else {
      tfel::raise(
          "CastemFiniteStrainBehaviour::integrate: "
          "invalid behaviour type (neither isotropic or orthotropic)");
    }
  }

  void CastemFiniteStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    CastemStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(mp,
                                                                        evm);
    if (this->stype == 0) {
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion", 0.);
    } else if (this->stype == 1) {
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion1", 0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion2", 0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion3", 0.);
    } else {
      tfel::raise(
          "CastemSmallStrainBehaviour::"
          "setOptionalMaterialPropertiesDefaultValues : "
          "unsupported symmetry type");
    }
  }  // end of
     // MTestCastemFiniteStrainStrainBehaviour::setOptionalMaterialPropertiesDefaultValues

  const char* CastemFiniteStrainBehaviour::getBehaviourNameForUMATFunctionCall()
      const {
    return nullptr;
  }  // end of CastemFiniteStrainBehaviour::getBehaviourNameForUMATFunctionCall

  CastemFiniteStrainBehaviour::~CastemFiniteStrainBehaviour() = default;

}  // end of namespace mtest
