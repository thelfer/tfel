/*!
 * \file   NonLinearConstraint.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03 march 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <string>
#include <ostream>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/NonLinearConstraint.hxx"

namespace mtest {

  /*!
   * \brief structure containing
   */
  struct NonLinearConstraint::Constraint {
    /*!
     * \brief a structure containing the data relative of one of the
     * variable defining the contraint.
     */
    struct Variable {
      /*!
       * \param[in] n: name
       * \param[in] pos: variable position
       */
      Variable(const std::string&, const unsigned short);
      //! \brief variable name
      std::string name;
      //! \brief position
      unsigned short p;
    };  // end of struct Variable
    /*!
     * A structure containing the constraint derivative and the second
     * derivative of the constraint
     */
    struct ConstraintDerivative {
      //! variable used to differentiate the constraint
      std::shared_ptr<Variable> v;
      // derivative of the
      std::shared_ptr<tfel::math::Evaluator> d;
      //! second derivative with respect to driving variables
      std::map<std::shared_ptr<Variable>,
               std::shared_ptr<tfel::math::Evaluator>>
          dd_dv;
      //! second derivative with respect to thermodynamic forces
      std::map<std::shared_ptr<Variable>,
               std::shared_ptr<tfel::math::Evaluator>>
          dd_tf;
    };  // end of struct ConstraintDerivative
    //! \brief list of driving variables used to define the constraint
    std::vector<std::shared_ptr<Variable>> dvs;
    //! \brief list of thermodynamic forces used to define the constraint
    std::vector<std::shared_ptr<Variable>> tfs;
    //! constraint
    std::shared_ptr<tfel::math::Evaluator> c;
    //! list of derivatives
    std::vector<ConstraintDerivative> d;
    /*!
     * \brief reference to all variables which are not driving
     * variables or thermodynamic forces
     */
    EvolutionManager evs;
    //! normalisation policy
    NormalisationPolicy np;
  };  // end of struct NonLinearConstraint::Constraint

  NonLinearConstraint::Constraint::Variable::Variable(const std::string& n,
                                                      const unsigned short pos)
      : name(n),
        p(pos) {}  // end of NonLinearConstraint::Constraint::Variable::Variable

  double NonLinearConstraint::eval(tfel::math::Evaluator& ev,
                                   const tfel::math::vector<real>& d,
                                   const tfel::math::vector<real>& s,
                                   const real t,
                                   const real dt) const {
    for (const auto& v : this->c->dvs) {
      ev.setVariableValue(v->name, d[v->p]);
    }
    for (const auto& v : this->c->tfs) {
      ev.setVariableValue(v->name, s[v->p]);
    }
    for (const auto& e : this->c->evs) {
      ev.setVariableValue(e.first, e.second->operator()(t + dt));
    }
    return ev.getValue();
  }  // end of eval

  NonLinearConstraint::NonLinearConstraint(const Behaviour& b_,
                                           const std::string& f,
                                           const EvolutionManager& evm,
                                           const NormalisationPolicy p)
      : b(b_) {
    using namespace tfel::math;
    auto throw_if = [](const bool cond, const std::string& m) {
      tfel::raise_if(cond, "NonLinearConstraint::NonLinearConstraint: " + m);
    };
    auto add_variable =
        [](std::vector<std::shared_ptr<Constraint::Variable>>& cv,
           const std::string& n, const unsigned short pos) {
          cv.push_back(std::make_shared<Constraint::Variable>(n, pos));
        };
    this->c = std::make_shared<Constraint>();
    this->c->c = std::make_shared<Evaluator>(f);
    this->c->np = p;
    // creating all the variables
    const auto all_dvs = this->b.getDrivingVariablesComponents();
    const auto all_tfs = this->b.getThermodynamicForcesComponents();
    for (const auto& v : this->c->c->getVariablesNames()) {
      if (std::find(all_dvs.begin(), all_dvs.end(), v) != all_dvs.end()) {
        add_variable(this->c->dvs, v, b.getDrivingVariableComponentPosition(v));
      } else if (std::find(all_tfs.begin(), all_tfs.end(), v) !=
                 all_tfs.end()) {
        add_variable(this->c->tfs, v,
                     b.getThermodynamicForceComponentPosition(v));
      } else {
        const auto pev = evm.find(v);
        throw_if(pev == evm.end(),
                 "undefined evolution or variable '" + v + "'");
        this->c->evs[v] = pev->second;
      }
    }
    throw_if((this->c->dvs.empty()) && (this->c->tfs.empty()),
             "invalid constraint '" + f +
                 "', "
                 "no thermodynamic force nor driving variable used in its "
                 "definition");
    // computation of the derivative and second derivative
    auto add_constraint_derivative =
        [this](const std::vector<std::shared_ptr<Constraint::Variable>>& cv) {
          for (const auto& v : cv) {
            auto d = Constraint::ConstraintDerivative();
            d.v = v;
            d.d = std::dynamic_pointer_cast<Evaluator>(
                this->c->c->differentiate(v->name));
            for (const auto& v2 : this->c->dvs) {
              auto dd = std::dynamic_pointer_cast<Evaluator>(
                  d.d->differentiate(v2->name));
              d.dd_dv.insert({v2, dd});
            }
            for (const auto& v2 : this->c->tfs) {
              auto dd = std::dynamic_pointer_cast<Evaluator>(
                  d.d->differentiate(v2->name));
              d.dd_tf.insert({v2, dd});
            }
            this->c->d.push_back(std::move(d));
          }
        };
    add_constraint_derivative(this->c->dvs);
    add_constraint_derivative(this->c->tfs);
    // informative messages
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
      auto& log = mfront::getLogStream();
      log << "\nAdding non linear constraint defined by: '" + f + "'\n";
      if (!this->c->dvs.empty()) {
        log << "List of related driving variables:";
        for (const auto& dv : this->c->dvs) {
          log << " '" << dv->name << "'";
        }
        log << '\n';
      }
      if (!this->c->tfs.empty()) {
        log << "List of related thermodynamic forces: ";
        for (const auto& tf : this->c->tfs) {
          log << " '" << tf->name << "'";
        }
        log << '\n';
      }
    }
  }  // end of NonLinearConstraint::NonLinearConstraint

  unsigned short NonLinearConstraint::getNumberOfLagrangeMultipliers() const {
    return 1u;
  }  // end of NonLinearConstraint::getNumberOfLagrangeMultipliers

  void NonLinearConstraint::setValues(tfel::math::matrix<real>& K,
                                      tfel::math::vector<real>& r,
                                      const tfel::math::vector<real>&,
                                      const tfel::math::vector<real>& u1,
                                      const tfel::math::matrix<real>& k,
                                      const tfel::math::vector<real>& s,
                                      const unsigned short pos,
                                      const unsigned short,
                                      const real t,
                                      const real dt,
                                      const real a) const {
    auto find_dv = [this](const std::shared_ptr<Constraint::Variable>& v) {
      return std::find(this->c->dvs.begin(), this->c->dvs.end(), v);
    };
    auto find_tf = [this](const std::shared_ptr<Constraint::Variable>& v) {
      return std::find(this->c->tfs.begin(), this->c->tfs.end(), v);
    };
    // current value of the Lagrange multiplier
    const auto l = u1(pos);
    const auto nf = this->c->np == DRIVINGVARIABLECONSTRAINT ? a : real(1);
    // force associated to the lagrange multiplier
    r(pos) -= nf * (this->eval(*(this->c->c), u1, s, t, dt));
    // derivative of the force associated to the lagrange multiplier
    for (const auto& d : this->c->d) {
      const auto pdv = find_dv(d.v);
      const auto ptf = find_tf(d.v);
      if (pdv != this->c->dvs.end()) {
        const auto dc_dv = this->eval(*(d.d), u1, s, t, dt);
        K(pos, (*(pdv))->p) -= nf * dc_dv;
      }
      if (ptf != this->c->tfs.end()) {
        const auto ndv = this->b.getDrivingVariablesSize();
        const auto ps = (*ptf)->p;
        const auto dc_dtf = this->eval(*(d.d), u1, s, t, dt);
        for (unsigned short i = 0; i != ndv; ++i) {
          K(pos, i) -= nf * dc_dtf * k(ps, i);
        }
      }
    }
    // computation of the forces associated with the constraint and
    // its derivative
    for (const auto& d : this->c->d) {
      const auto pdv = find_dv(d.v);
      const auto ptf = find_tf(d.v);
      if (pdv != this->c->dvs.end()) {
        const auto i = (*pdv)->p;
        const auto dc_dv = this->eval(*(d.d), u1, s, t, dt);
        const auto kv = nf * dc_dv;
        r(i) -= kv * l;
        // derivative of the force
        K(i, pos) -= kv;
        for (const auto& d2 : d.dd_dv) {
          const auto j = d2.first->p;
          const auto d2c_dij = this->eval(*(d2.second), u1, s, t, dt);
          K(i, j) -= nf * d2c_dij * l;
        }
        for (const auto& d2 : d.dd_tf) {
          const auto ndv = this->b.getDrivingVariablesSize();
          const auto ps = d2.first->p;
          const auto d2c_dis = this->eval(*(d2.second), u1, s, t, dt);
          for (unsigned short j = 0; j != ndv; ++j) {
            K(i, j) -= nf * d2c_dis * k(ps, j) * l;
          }
        }
      }
      if (ptf != this->c->tfs.end()) {
        const auto ndv = this->b.getDrivingVariablesSize();
        const auto dc_dtf = this->eval(*(d.d), u1, s, t, dt);
        const auto ps = (*ptf)->p;
        for (unsigned short i = 0; i != ndv; ++i) {
          r(i) -= nf * dc_dtf * k(ps, i) * l;
          K(i, pos) -= nf * dc_dtf * k(ps, i);
          // approximation of the derivative of the force
          for (const auto& d2 : d.dd_dv) {
            const auto j = d2.first->p;
            const auto d2c_dsj = this->eval(*(d2.second), u1, s, t, dt);
            K(i, j) -= nf * d2c_dsj * k(ps, i) * l;  // missing term dk(ps_i)_dj
          }
          for (const auto& d2 : d.dd_tf) {
            const auto ps2 = d2.first->p;
            const auto d2c_dsds2 = this->eval(*(d2.second), u1, s, t, dt);
            for (unsigned short j = 0; j != ndv; ++j) {
              K(i, j) -= nf * d2c_dsds2 * k(ps, i) * k(ps2, j) *
                         l;  // missing term dk(ps_i)_dj
            }
          }
        }
      }
    }
  }  // end of NonLinearConstraint::setValues

  bool NonLinearConstraint::checkConvergence(const tfel::math::vector<real>& e,
                                             const tfel::math::vector<real>& s,
                                             const real eeps,
                                             const real seps,
                                             const real t,
                                             const real dt) const {
    const auto cv = this->eval(*(this->c->c), e, s, t, dt);
    if (this->c->np == DRIVINGVARIABLECONSTRAINT) {
      return std::abs(cv) < eeps;
    }
    return std::abs(cv) < seps;
  }

  std::string NonLinearConstraint::getFailedCriteriaDiagnostic(
      const tfel::math::vector<real>& e,
      const tfel::math::vector<real>& s,
      const real eeps,
      const real seps,
      const real t,
      const real dt) const {
    const auto cv = this->eval(*(this->c->c), e, s, t, dt);
    const auto ev = this->c->np == DRIVINGVARIABLECONSTRAINT ? eeps : seps;
    return "imposed constraint not reached "
           "(constraint value: " +
           std::to_string(cv) +
           ","
           " criteria : " +
           std::to_string(ev) + ")";
  }

  NonLinearConstraint::~NonLinearConstraint() = default;

}  // end of namespace mtest
