/*!
 * \file   GasEquationOfState.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   19 oct. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <limits>
#include <iostream>
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Math/Parser/ConstantExternalFunction.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/GasEquationOfState.hxx"

namespace mtest {

  struct GasEquationOfState::GasEquationOfStateMembers {
    //! a simple alias
    using ExternalFunctionManager =
        std::shared_ptr<tfel::math::parser::ExternalFunctionManager>;
    /*!
     * \param[in] e: equation of state
     */
    GasEquationOfStateMembers(const std::string& e, const EvolutionManager& evm)
        : m(std::make_shared<tfel::math::parser::ExternalFunctionManager>()),
          s({"P", "V", "n", "T"}, e, this->m) {
      using Constant = tfel::math::parser::ConstantExternalFunction;
      auto throw_if = [](const bool c, const std::string& msg) {
        if (c) {
          throw(
              std::runtime_error("GasEquationOfStateMembers::"
                                 "GasEquationOfStateMembers: " +
                                 msg));
        }
      };
      std::set<std::string> params;
      this->s.getParametersNames(params);
      for (const auto& p : params) {
        const auto pev = evm.find(p);
        throw_if(pev == evm.end(), "undefined variable '" + p + "'");
        const auto& ev = *(pev->second);
        throw_if(!ev.isConstant(), "variable '" + p +
                                       "' "
                                       "must reference a constant evolution");
        this->m->insert({p, std::make_shared<Constant>(ev(0))});
      }
      this->ds_dn = std::dynamic_pointer_cast<tfel::math::Evaluator>(
          s.differentiate("n"));
      this->ds_dV = std::dynamic_pointer_cast<tfel::math::Evaluator>(
          s.differentiate("V"));
      this->ds_dP = std::dynamic_pointer_cast<tfel::math::Evaluator>(
          s.differentiate("P"));
    }  // end of GasEquationOfStateMembers
       /*!
        * \param[in] P: pressure
        * \param[in] V: volume
        * \param[in] T: temperature
        */
    double computeNumberOfMoles(const double P,
                                const double V,
                                const double T) {
      constexpr const double R = 8.3144598;
      constexpr const unsigned short imax = 100;
      auto iter = [this, P, V, T](double& n, double& dn) {
        dn = -eval(this->s, P, V, n, T) / eval(*(this->ds_dn), P, V, n, T);
        n += dn;
      };
      const auto nref = P * V / (R * T);
      const auto eps = nref * 100 * std::numeric_limits<double>::epsilon();
      auto n = nref;
      auto dn = double();
      iter(n, dn);
      unsigned short i = 0;
      while ((std::abs(dn) > eps) && (i != imax)) {
        iter(n, dn);
        ++i;
      }
      if (i == imax) {
        throw(std::runtime_error(
            "GasEquationOfStateMembers::computeNumberOfMoles: "
            "can't determine number of moles, "
            "maximum number of iterations reached"));
      }
      return n;
    }
    /*!
     * \param[in] V: volume
     * \param[in] n: number of moles
     * \param[in] T: temperature
     */
    double computePressure(const double V, const double n, const double T) {
      constexpr const double R = 8.3144598;
      constexpr const unsigned short imax = 100;
      auto iter = [this, n, V, T](double& P, double& dP) {
        dP = -eval(this->s, P, V, n, T) / eval(*(this->ds_dP), P, V, n, T);
        P += dP;
      };
      const auto Pref = n * R * T / V;
      const auto eps = Pref * 100 * std::numeric_limits<double>::epsilon();
      auto P = Pref;
      auto dP = double();
      iter(P, dP);
      unsigned short i = 0;
      while ((std::abs(dP) > eps) && (i != imax)) {
        iter(P, dP);
        ++i;
      }
      if (i == imax) {
        throw(
            std::runtime_error("GasEquationOfStateMembers::computePressure: "
                               "can't determine pressure, "
                               "maximum number of iterations reached"));
      }
      return P;
    }
    /*!
     * \param[in] V: volume
     * \param[in] n: number of moles
     * \param[in] T: temperature
     */
    double computeIsothermalBulkModulus(const double V,
                                        const double n,
                                        const double T) {
      const auto P = this->computePressure(V, n, T);
      // ds_dp*dP=-ds_dV*dV => dP_dV = - (ds_dV/ds_dP)
      const auto dP_dV =
          -eval(*(this->ds_dV), P, V, n, T) / eval(*(this->ds_dP), P, V, n, T);
      // K=-V*dP_dV
      return -V * dP_dV;
    }

   private:
    static double eval(tfel::math::Evaluator& e,
                       const double P,
                       const double V,
                       const double n,
                       const double T) {
      e.setVariableValue("P", P);
      e.setVariableValue("V", V);
      e.setVariableValue("n", n);
      e.setVariableValue("T", T);
      return e.getValue();
    }  // end of eval
    //! handling of external constants
    ExternalFunctionManager m;
    //! equation of state
    tfel::math::Evaluator s;
    //! derivative with respect to the number of moles
    std::shared_ptr<tfel::math::Evaluator> ds_dn;
    //! derivative with respect to the volume
    std::shared_ptr<tfel::math::Evaluator> ds_dV;
    //! derivative with respect to the pressure
    std::shared_ptr<tfel::math::Evaluator> ds_dP;
  };

  GasEquationOfState::GasEquationOfState(const std::string& e,
                                         const EvolutionManager& evm)
      : m(std::unique_ptr<GasEquationOfStateMembers>(
            new GasEquationOfStateMembers(e, evm))) {
  }  // end of GasEquationOfState::GasEquationOfState

  double GasEquationOfState::computeNumberOfMoles(const double P,
                                                  const double V,
                                                  const double T) {
    return this->m->computeNumberOfMoles(P, V, T);
  }  // end of GasEquationOfState::computeNumberOfMoles

  double GasEquationOfState::computePressure(const double V,
                                             const double n,
                                             const double T) {
    return this->m->computePressure(V, n, T);
  }  // end of GasEquationOfState::computePressure

  double GasEquationOfState::computeIsothermalBulkModulus(const double V,
                                                          const double n,
                                                          const double T) {
    return this->m->computeIsothermalBulkModulus(V, n, T);
  }  // end of GasEquationOfState::computeIsothermalBulkModulus

  GasEquationOfState::~GasEquationOfState() = default;

}  // end of namespace mtest
