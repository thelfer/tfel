/*!
 * \file   mtest/src/HybridHighOrderBehaviourWrapperBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MTest/Behaviour.hxx"
#include "MTest/HybridHighOrder/BehaviourWrapperBase.hxx"

namespace mtest {

  namespace hho {

    static ::hho::element::Variable::Type convertType(const int t) {
      if (t == 0) {
        return ::hho::element::Variable::SCALAR;
      } else if (t == 1) {
        return ::hho::element::Variable::STENSOR;
      } else if (t == 2) {
        return ::hho::element::Variable::VECTOR;
      }
      if (t != 3) {
        tfel::raise("mtest::hho::convertType: invalid type");
      }
      return ::hho::element::Variable::TENSOR;
    }  // end of convertType

    static std::vector<::hho::element::Variable> makeVariables(
        const std::vector<std::string> &names, const std::vector<int> &types) {
      if (names.size() != types.size()) {
        tfel::raise("mtest::hho::makeVariables: inconsistent sizes");
      }
      auto r = std::vector<::hho::element::Variable>{};
      for (std::vector<std::string>::size_type i = 0; i != names.size(); ++i) {
        r.push_back(::hho::element::Variable{names[i], convertType(types[i])});
      }
      return r;
    }  // end of makeVariables

    BehaviourWrapperBase::BehaviourWrapperBase(
        const mtest::Behaviour &b) noexcept : behaviour(b) {
    }  // end of BehaviourWrapperBase::BehaviourWrapperBase

    ::hho::Hypothesis BehaviourWrapperBase::get_hypothesis() const {
      return ::hho::Hypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
    }  // end of BehaviourWrapperBase::get_hypothesis

    ::hho::element::Behaviour::Symmetry BehaviourWrapperBase::get_symmetry()
        const {
      if (this->behaviour.getSymmetryType() == 0) {
        return ::hho::element::Behaviour::ISOTROPIC;
      }
      return ::hho::element::Behaviour::ORTHOTROPIC;
    }  // end of BehaviourWrapperBase::get_symmetry

    std::vector<::hho::element::Variable> BehaviourWrapperBase::get_gradients()
        const {
      return makeVariables(this->behaviour.getGradientsNames(),
                           this->behaviour.getGradientsTypes());
    }  // end of BehaviourWrapperBase::get_gradients

    ::hho::size_type BehaviourWrapperBase::get_gradients_size() const {
      return this->behaviour.getGradientsSize();
    }  // end of BehaviourWrapperBase::get_gradients_size

    std::vector<::hho::element::Variable>
    BehaviourWrapperBase::get_thermodynamic_forces() const {
      return makeVariables(this->behaviour.getThermodynamicForcesNames(),
                           this->behaviour.getThermodynamicForcesTypes());
    }  // end of BehaviourWrapperBase::get_thermodynamic_forces

    ::hho::size_type BehaviourWrapperBase::get_thermodynamic_forces_size()
        const {
      return this->behaviour.getThermodynamicForcesSize();
    }  // end of BehaviourWrapperBase::get_thermodynamic_forces_size

    std::vector<::hho::element::Variable>
    BehaviourWrapperBase::get_material_properties() const {
      std::vector<::hho::element::Variable> r;
      for (const auto &n : this->behaviour.getMaterialPropertiesNames()) {
        r.push_back(::hho::element::Variable{n, ::hho::element::Variable::SCALAR});
      }
      return r;
    }  // end of BehaviourWrapperBase::get_material_properties

    ::hho::size_type BehaviourWrapperBase::get_material_properties_size()
        const {
      return this->behaviour.getThermodynamicForcesSize();
    }  // end of BehaviourWrapperBase::get_material_properties_size

    std::vector<::hho::element::Variable> BehaviourWrapperBase::get_internal_state_variables() const {
      return makeVariables(this->behaviour.getInternalStateVariablesNames(),
                           this->behaviour.getInternalStateVariablesTypes());
    }  // end of BehaviourWrapperBase::get_internal_state_variables

    ::hho::size_type BehaviourWrapperBase::get_internal_state_variables_size()
        const {
      return this->behaviour.getInternalStateVariablesSize();
    }  // end of BehaviourWrapperBase::get_internal_state_variables_size

    std::vector<::hho::element::Variable>
    BehaviourWrapperBase::get_external_state_variables() const {
      std::vector<::hho::element::Variable> r;
      for (const auto &n : this->behaviour.getExternalStateVariablesNames()) {
        r.push_back(::hho::element::Variable{n, ::hho::element::Variable::SCALAR});
      }
      return r;
    }  // end of BehaviourWrapperBase::get_external_state_variables

    ::hho::size_type BehaviourWrapperBase::get_external_state_variables_size()
        const {
      return this->behaviour.getExternalStateVariablesSize();
    }  // end of BehaviourWrapperBase::get_external_state_variables_size

    std::vector<std::pair<::hho::element::Variable, ::hho::element::Variable>>
    BehaviourWrapperBase::get_tangent_operators_blocks() const {
      auto r = std::vector<
          std::pair<::hho::element::Variable, ::hho::element::Variable>>{};
      for (const auto &to : this->behaviour.getTangentOperatorBlocks()) {
        auto find = [this, &to](const std::string &n) {
          auto get_position =
              [this, &n](const std::vector<std::string> &vns) -> ptrdiff_t {
            const auto pg = std::find(vns.begin(), vns.end(), n);
            return (pg != vns.end()) ? pg - vns.begin() : -1;
          };  // end of get_position
          {
            const auto gns = this->behaviour.getGradientsNames();
            const auto pos = get_position(gns);
            if (pos != -1) {
              const auto gtypes = this->behaviour.getGradientsTypes();
              return ::hho::element::Variable{gns.at(pos),
                                              convertType(gtypes.at(pos))};
            }
          }
          {
            const auto thns = this->behaviour.getThermodynamicForcesNames();
            const auto pos = get_position(thns);
            if (pos != -1) {
              const auto thtypes =
                  this->behaviour.getThermodynamicForcesTypes();
              return ::hho::element::Variable{thns.at(pos),
                                              convertType(thtypes.at(pos))};
            }
          }
          {
            const auto isvns = this->behaviour.getInternalStateVariablesNames();
            const auto pos = get_position(isvns);
            if (pos != -1) {
              const auto isvtypes =
                  this->behaviour.getInternalStateVariablesTypes();
              return ::hho::element::Variable{isvns.at(pos),
                                              convertType(isvtypes.at(pos))};
            }
          }
          const auto esvns = this->behaviour.getExternalStateVariablesNames();
          const auto pos = get_position(esvns);
          if (pos == -1) {
            tfel::raise(
                "BehaviourWrapperBase::get_tangent_operators_blocks: "
                "unsupported tangent operator block (" +
                to.first + "/" + to.second + ")");
          }
          return ::hho::element::Variable{esvns.at(pos),
                                          ::hho::element::Variable::SCALAR};
        };
        r.push_back({find(to.first), find(to.second)});
      }
      return r;
    }  // end of BehaviourWrapperBase::get_tangent_operators_blocks

    ::hho::size_type BehaviourWrapperBase::get_tangent_operator_blocks_size()
        const {
      return this->behaviour.getTangentOperatorArraySize();
    }  // end of BehaviourWrapperBase::get_tangent_operator_blocks_size

    void BehaviourWrapperBase::integrate(
        ::hho::element::IntegrationPointState &,
        ::hho::span<::hho::real>,
        ::hho::span<const ::hho::real>,
        ::hho::span<const ::hho::real>,
        const ::hho::element::IntegrationPointState &,
        const ::hho::element::IntegrationType,
        const ::hho::real) const {}  // end of BehaviourWrapperBase::integrate

    BehaviourWrapperBase::~BehaviourWrapperBase() noexcept = default;

  }  // end of namespace hho

}  // end of namespace mtest
