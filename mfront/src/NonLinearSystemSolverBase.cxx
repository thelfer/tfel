/*!
 * \file  mfront/src/NonLinearSystemSolverBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/NonLinearSystemSolverBase.hxx"

namespace mfront {

  std::vector<std::string> NonLinearSystemSolverBase::getReservedNames() const {
    return {"jacobian_error", "error", "iter", "iterMax", "converged"};
  }  // end of NonLinearSystemSolverBase::getReservedNames

  std::string NonLinearSystemSolverBase::getJacobianPart(
      const BehaviourDescription& mb,
      const VariableDescription& v1,
      const VariableDescription& v2,
      const SupportedTypes::TypeSize& n,
      const SupportedTypes::TypeSize& n2,
      const SupportedTypes::TypeSize& n3,
      const std::string& j,
      const std::string& p) {
    auto lthrow = [](const std::string& m) {
      tfel::raise("NonLinearSystemSolverBase::getJacobianPart: " + m);
    };
    std::ostringstream d;
    if (mb.getTypeFlag(v1.type) == SupportedTypes::Stensor) {
      if (mb.getTypeFlag(v2.type) == SupportedTypes::Stensor) {
        d << "typename tfel::math::ST2toST2FromTinyMatrixView<N," << n2 << ","
          << n2 << ",\n"
          << n << "," << n3 << ",real>::type " + p + "df" << v1.name << "_dd"
          << v2.name << "(" + j + ");\n";
      } else if (mb.getTypeFlag(v2.type) == SupportedTypes::Scalar) {
        d << "typename tfel::math::StensorFromTinyMatrixColumnView<N," << n2
          << "," << n2 << ",\n"
          << n << "," << n3 << ",real>::type " + p + "df" << v1.name << "_dd"
          << v2.name << "(" + j + ");\n";
      } else {
        lthrow("unsupported type for integration variable '" + v2.name + "'");
      }
    } else if (mb.getTypeFlag(v1.type) == SupportedTypes::Scalar) {
      if (mb.getTypeFlag(v2.type) == SupportedTypes::Stensor) {
        d << "typename tfel::math::StensorFromTinyMatrixRowView<N," << n2 << ","
          << n2 << ",\n"
          << n << "," << n3 << ",real>::type " + p + "df" << v1.name << "_dd"
          << v2.name << "(" + j + ");\n";
      } else if (mb.getTypeFlag(v2.type) == SupportedTypes::Scalar) {
        d << "real& " + p + "df" << v1.name << "_dd" << v2.name
          << " = " + j + "(" << n << "," << n3 << ");\n";
      } else {
        lthrow("unsupported type for integration variable '" + v2.name + "'");
      }
    } else {
      lthrow("unsupported type for integration variable '" + v2.name + "'");
    }
    return d.str();
  }  // end of NonLinearSystemSolverBase::getJacobianPart

  void NonLinearSystemSolverBase::writeEvaluateNumericallyComputedBlocks(
      std::ostream& out, const BehaviourDescription& mb, const Hypothesis h) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "NonLinearSystemSolverBase::"
                     "writeEvaluateNumericallyComputedBlocks: " +
                         m);
    };
    const auto& d = mb.getBehaviourData(h);
    if (!d.hasAttribute(BehaviourData::numericallyComputedJacobianBlocks)) {
      return;
    }
    // First sort blocks by column
    const auto blocs = [&d, throw_if] {
      auto r = std::map<std::string, std::vector<std::string>>{};
      const auto& jbs = d.getAttribute<std::vector<std::string>>(
          BehaviourData::numericallyComputedJacobianBlocks);
      for (const auto& jb : jbs) {
        const auto nd = [throw_if,
                         &jb]() -> std::pair<std::string, std::string> {
          throw_if(jb.empty(), "empty jacobian block");
          throw_if(jb.size() < 6, "invalid jacobian block '" + jb + "'");
          throw_if(jb[0] != 'd', "invalid jacobian block '" + jb + "'");
          throw_if(jb[1] != 'f', "invalid jacobian block '" + jb + "'");
          const auto p = jb.find('_');
          throw_if(p == std::string::npos,
                   "invalid jacobian block '" + jb + "'");
          throw_if(p + 2 >= jb.size(), "invalid jacobian block '" + jb + "'");
          throw_if(jb[p + 1] != 'd', "invalid jacobian block '" + jb + "'");
          throw_if(jb[p + 2] != 'd', "invalid jacobian block '" + jb + "'");
          const auto nn = jb.substr(2, p - 2);
          const auto dn = jb.substr(p + 3);
          throw_if(nn.empty(), "invalid jacobian block '" + jb + "'");
          throw_if(dn.empty(), "invalid jacobian block '" + jb + "'");
          return {nn, dn};
        }();
        throw_if(!d.isIntegrationVariableName(nd.first),
                 "invalid jacobian block '" + jb +
                     "', "
                     "'" +
                     nd.first + "' is not an integration variable");
        throw_if(!d.isIntegrationVariableName(nd.second),
                 "invalid jacobian block '" + jb +
                     "', "
                     "'" +
                     nd.second + "' is not an integration variable");
        r[nd.second].push_back(nd.first);
      }
      return r;
    }();
    const auto& ivs = d.getIntegrationVariables();
    const auto n = ivs.getTypeSize();
    out << "tmatrix<" << n << "," << n << ",real> tjacobian(this->jacobian);\n"
        << "tvector<" << n << ",real> tfzeros(this->fzeros);\n";
    bool first = true;
    for (const auto& b : blocs) {
      auto getPositionAndSize = [&ivs, throw_if](const std::string& v)
          -> std::pair<SupportedTypes::TypeSize, SupportedTypes::TypeSize> {
        auto ns = SupportedTypes::TypeSize{};
        for (const auto& iv : ivs) {
          const auto s = SupportedTypes::getTypeSize(iv.type, iv.arraySize);
          if (iv.name == v) {
            return {ns, s};
          }
          ns += s;
        }
        throw_if(true, "no integration variable named '" + v + "'");
      };
      const auto pd = getPositionAndSize(b.first);
      auto update_jacobian = [&out, &b,
                              getPositionAndSize](const std::string& j) {
        // fzeros contain the new jacobian value
        for (const auto& v : b.second) {
          const auto pn = getPositionAndSize(v);
          if (pn.second.isOne()) {
            out << "tjacobian(" << pn.first << "," << j << ") = "
                << "this->fzeros[" << pn.first << "];\n";
          } else {
            const auto i = [&pn]() -> std::string {
              if (!pn.first.isNull()) {
                return to_string(pn.first) + "+idx2";
              }
              return "idx2";
            }();
            out << "for(unsigned short idx2 = 0; idx2!= " << pn.second
                << ";++idx2){\n"
                << "tjacobian(" << i << "," << j << ") = "
                << "this->fzeros[" << i << "];\n"
                << "}\n";
          }
        }
      };
      auto compute_perturbation = [&out, &update_jacobian, &d,
                                   &n](const std::string& j) {
        out << "this->zeros(" << j
            << ") -= this->numerical_jacobian_epsilon;\n";
        if (d.hasCode(BehaviourData::ComputeStress)) {
          out << "this->computeStress();\n";
        }
        out << "this->computeFdF(true);\n"
            << "this->zeros = this->zeros_1;\n"
            << "tvector<" << n << ",real> tfzeros2(this->fzeros);\n"
            << "this->zeros(" << j
            << ") += this->numerical_jacobian_epsilon;\n";
        if (d.hasCode(BehaviourData::ComputeStress)) {
          out << "this->computeStress();\n";
        }
        out << "this->computeFdF(true);\n"
            << "this->zeros  = this->zeros_1;\n"
            << "this->fzeros = "
               "(this->fzeros-tfzeros2)/"
               "(2*(this->numerical_jacobian_epsilon));\n"
            << "// update jacobian\n";
        update_jacobian(j);
      };
      if (!first) {
        out << "// restore residual values\n"
            << "this->fzeros = tfzeros;\n";
      }
      if (pd.second.isOne()) {
        out << "{\n";
        compute_perturbation(to_string(pd.first));
        out << "}\n";
      } else {
        out << "for(unsigned short idx = 0; idx!= " << pd.second
            << ";++idx){\n";
        if (!pd.first.isNull()) {
          compute_perturbation(to_string(pd.first) + "+idx");
        } else {
          compute_perturbation("idx");
        }
        out << "}\n";
      }
      first = false;
    }
    out << "// update jacobian\n"
        << "this->jacobian = tjacobian;\n"
        << "// restore residual values\n"
        << "this->fzeros = tfzeros;\n";
  }  // end of
     // NonLinearSystemSolverBase::writeEvaluateNumericallyComputedBlocks

  void NonLinearSystemSolverBase::writeComparisonToNumericalJacobian(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h,
      const std::string& nj) {
    if (!mb.getAttribute(h, BehaviourData::compareToNumericalJacobian, false)) {
      return;
    }
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n3;
    const auto n2 = d.getIntegrationVariables().getTypeSize();
    out << "auto jacobian_error = real{};"
        << "this->computeNumericalJacobian(" << nj << ");\n";
    n = SupportedTypes::TypeSize();
    for (const auto& v : d.getIntegrationVariables()) {
      if (v.arraySize == 1) {
        n3 = SupportedTypes::TypeSize();
        for (const auto& v2 : d.getIntegrationVariables()) {
          if ((v.arraySize == 1) && (v2.arraySize == 1)) {
            out << "// derivative of variable f" << v.name << " by variable "
                << v2.name << "\n"
                << NonLinearSystemSolverBase::getJacobianPart(mb, v, v2, n, n2,
                                                              n3)
                << "// numerical derivative of variable f" << v.name
                << " by variable " << v2.name << "\n"
                << NonLinearSystemSolverBase::getJacobianPart(mb, v, v2, n, n2,
                                                              n3, nj, "n");
            n3 += mb.getTypeSize(v2.type, v2.arraySize);
          }
        }
      }
      n += mb.getTypeSize(v.type, v.arraySize);
    }
    for (const auto& v1 : d.getIntegrationVariables()) {
      for (const auto& v2 : d.getIntegrationVariables()) {
        const auto nv1 = mb.getTypeSize(v1.type, 1u);
        const auto nv2 = mb.getTypeSize(v2.type, 1u);
        out << "jacobian_error=" << nv1 << "*" << nv2 << "*"
            << "(this->jacobianComparisonCriterion)"
            << ";\n";
        if ((v1.arraySize == 1u) && (v2.arraySize == 1u)) {
          out << "if(abs("
              << "df" << v1.name << "_dd" << v2.name << "-"
              << "ndf" << v1.name << "_dd" << v2.name << ") > jacobian_error)\n"
              << "{\n"
              << "cout << abs("
              << "df" << v1.name << "_dd" << v2.name << "-"
              << "ndf" << v1.name << "_dd" << v2.name
              << ") << \" \" << jacobian_error << endl;\n"
              << "cout << \"df" << v1.name << "_dd" << v2.name << " :\\n\" << "
              << "df" << v1.name << "_dd" << v2.name << " << endl;\n"
              << "cout << \"ndf" << v1.name << "_dd" << v2.name << " :\\n\" << "
              << "ndf" << v1.name << "_dd" << v2.name << " << endl;\n"
              << "cout << \"df" << v1.name << "_dd" << v2.name << " - ndf"
              << v1.name << "_dd" << v2.name << " :\\n\" << "
              << "df" << v1.name << "_dd" << v2.name << "-"
              << "ndf" << v1.name << "_dd" << v2.name << " << endl;\n"
              << "cout << endl;\n"
              << "}\n";
        } else if (((v1.arraySize != 1u) && (v2.arraySize == 1u)) ||
                   ((v2.arraySize != 1u) && (v1.arraySize == 1u))) {
          const auto asize = (v1.arraySize != 1u) ? v1.arraySize : v2.arraySize;
          out << "for(unsigned short idx=0;idx!=" << asize << ";++idx){\n"
              << "if(abs("
              << "df" << v1.name << "_dd" << v2.name << "(idx)-"
              << "df" << v1.name << "_dd" << v2.name << "(" << nj
              << ",idx)) > jacobian_error)\n"
              << "{\n"
              << "cout << abs("
              << "df" << v1.name << "_dd" << v2.name << "(idx)-"
              << "df" << v1.name << "_dd" << v2.name << "(" << nj
              << ",idx)) << \" \" << jacobian_error << endl;\n"
              << "cout << \"df" << v1.name << "_dd" << v2.name
              << "(\" << idx << \") :\\n\" << "
              << "df" << v1.name << "_dd" << v2.name << "(idx) << endl;\n"
              << "cout << \"ndf" << v1.name << "_dd" << v2.name
              << "(\" << idx << \") :\\n\" << "
              << "df" << v1.name << "_dd" << v2.name << "(" << nj
              << ",idx) << endl;\n"
              << "cout << \"df" << v1.name << "_dd" << v2.name << " - ndf"
              << v1.name << "_dd" << v2.name << "(\" << idx << \") :\\n\" << "
              << "df" << v1.name << "_dd" << v2.name << "(idx) -"
              << "df" << v1.name << "_dd" << v2.name << "(" << nj
              << ",idx) << endl;\n"
              << "cout << endl;\n"
              << "}\n"
              << "}\n";
        } else {
          out << "for(unsigned short idx=0;idx!=" << v1.arraySize
              << ";++idx){\n"
              << "for(unsigned short idx2=0;idx2!=" << v2.arraySize
              << ";++idx2){\n"
              << "if(abs("
              << "df" << v1.name << "_dd" << v2.name << "(idx,idx2)-"
              << "df" << v1.name << "_dd" << v2.name << "(" << nj
              << ",idx,idx2)) > jacobian_error)\n"
              << "{\n"
              << "cout << abs("
              << "df" << v1.name << "_dd" << v2.name << "(idx,idx2)-"
              << "df" << v1.name << "_dd" << v2.name << "(" << nj
              << ",idx,idx2)) << \" \" << jacobian_error << endl;\n"
              << "cout << \"df" << v1.name << "_dd" << v2.name
              << "(\" << idx << \",\" << idx2 << \") :\\n\" << "
              << "df" << v1.name << "_dd" << v2.name << "(idx,idx2) << endl;\n"
              << "cout << \"ndf" << v1.name << "_dd" << v2.name
              << "(\" << idx << \",\" << idx2 << \") :\\n\" << "
              << "df" << v1.name << "_dd" << v2.name << "(" << nj
              << ",idx,idx2) << endl;\n"
              << "cout << \"df" << v1.name << "_dd" << v2.name << " - ndf"
              << v1.name << "_dd" << v2.name
              << "(\" << idx << \",\" << idx2 << \") :\\n\" << "
              << "df" << v1.name << "_dd" << v2.name << "(idx,idx2) -"
              << "df" << v1.name << "_dd" << v2.name << "(" << nj
              << ",idx,idx2) << endl;\n"
              << "cout << endl;\n"
              << "}\n"
              << "}\n"
              << "}\n";
        }
      }
    }
  }  // end of
     // NonLinearSystemSolverBase::writeComparisonToNumericalJacobian

  void NonLinearSystemSolverBase::writeLimitsOnIncrementValues(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h,
      const std::string& v) {
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    for (const auto& iv : d.getIntegrationVariables()) {
      const auto nv = mb.getTypeSize(iv.type, iv.arraySize);
      if (mb.hasParameter(h,
                          iv.name + "_maximum_increment_value_per_iteration")) {
        out << "for(unsigned short idx = 0; idx!=" << nv << ";++idx){\n";
        if (mb.hasAttribute(h, iv.name + "_normalisation_factor")) {
          const auto& nf = mb.getAttribute<std::string>(
              h, iv.name + "_normalisation_factor");
          out << "if(std::abs(" << v << "[" << n << "+idx])>" << nf
              << "*(this->" << iv.name
              << "_maximum_increment_value_per_iteration)){\n";
        } else {
          out << "if(std::abs(" << v << "[" << n << "+idx])>this->" << iv.name
              << "_maximum_increment_value_per_iteration){\n";
        }
        out << "if(" << v << "[" << n << "+idx]<0){\n";
        if (mb.hasAttribute(h, iv.name + "_normalisation_factor")) {
          const auto& nf = mb.getAttribute<std::string>(
              h, iv.name + "_normalisation_factor");
          out << "" << v << "[" << n << "+idx] = -" << nf << "*(this->"
              << iv.name << "_maximum_increment_value_per_iteration);\n";
        } else {
          out << "" << v << "[" << n << "+idx] = -this->" << iv.name
              << "_maximum_increment_value_per_iteration;\n";
        }
        out << "} else {\n";
        if (mb.hasAttribute(h, iv.name + "_normalisation_factor")) {
          const auto& nf = mb.getAttribute<std::string>(
              h, iv.name + "_normalisation_factor");
          out << "" << v << "[" << n << "+idx] = " << nf << "*(this->"
              << iv.name << "_maximum_increment_value_per_iteration);\n";
        } else {
          out << "" << v << "[" << n << "+idx] =  this->" << iv.name
              << "_maximum_increment_value_per_iteration;\n";
        }
        out << "}\n";
        out << "}\n";
        out << "}\n";
      } else if (mb.hasParameter(h, "maximum_increment_value_per_iteration")) {
        out << "for(unsigned short idx = 0; idx!=" << nv << ";++idx){\n";
        out << "if(std::abs(" << v << "[" << n
            << "+idx])>this->maximum_increment_value_per_iteration){\n";
        out << "if(" << v << "[" << n << "+idx]<0){\n";
        out << "" << v << "[" << n
            << "+idx] = "
               "-this->maximum_increment_value_"
               "per_iteration;\n";
        out << "} else {\n";
        out << "" << v << "[" << n
            << "+idx] =  "
               "this->maximum_increment_value_"
               "per_iteration;\n";
        out << "}\n";
        out << "}\n";
        out << "}\n";
      }
      n += nv;
    }
  }  // end of NonLinearSystemSolverBase::writeLimitsOnIncrementValues

  void NonLinearSystemSolverBase::
      writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(
          std::ostream& out,
          const BehaviourDescription& mb,
          const Hypothesis h) {
    const auto& d = mb.getBehaviourData(h);
    auto n = SupportedTypes::TypeSize{};
    for (const auto& v : d.getStateVariables()) {
      if (!v.hasPhysicalBounds()) {
        n += mb.getTypeSize(v.type, v.arraySize);
        continue;
      }
      const auto& b = v.getPhysicalBounds();
      // treating lower bounds
      if ((b.boundsType == VariableBoundsDescription::LOWER) ||
          (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
        if ((mb.getTypeFlag(v.type) == SupportedTypes::Scalar) &&
            (v.arraySize == 1u)) {
          if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
            const auto& nf = mb.getAttribute<std::string>(
                h, v.name + "_normalisation_factor");
            out << "if(this->" << v.name << "+ " << nf << "*(this->zeros[" << n
                << "]) <" << b.lowerBound << "){\n";
          } else {
            out << "if(this->" << v.name << "+this->zeros[" << n << "]<"
                << b.lowerBound << "){\n";
          }
          if (std::abs(b.lowerBound) >
              std::numeric_limits<long double>::min()) {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "this->zeros[" << n << "] = (" << b.lowerBound
                  << "- (this->" << v.name << "))/(" << nf << ");\n";
            } else {
              out << "this->zeros[" << n << "] = " << b.lowerBound
                  << "- (this->" << v.name << ");\n";
            }
          } else {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "this->zeros[" << n << "] = - (this->" << v.name << ")/("
                  << nf << ");\n";
            } else {
              out << "this->zeros[" << n << "] = - (this->" << v.name << ");\n";
            }
          }
          out << "}\n";
        }
        if ((mb.getTypeFlag(v.type) == SupportedTypes::Scalar) &&
            (v.arraySize != 1u)) {
          out << "for(unsigned short idx=0;idx!=" << v.arraySize
              << ";++idx){\n";
          if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
            const auto& nf = mb.getAttribute<std::string>(
                h, v.name + "_normalisation_factor");
            out << "if(this->" << v.name << "[idx]+(" << nf << ")*(this->zeros["
                << n << "+idx])<" << b.lowerBound << "){\n";
          } else {
            out << "if(this->" << v.name << "[idx]+this->zeros[" << n
                << "+idx]<" << b.lowerBound << "){\n";
          }
          if (std::abs(b.lowerBound) >
              std::numeric_limits<long double>::min()) {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "this->zeros[" << n << "+idx] = (" << b.lowerBound
                  << "- (this->" << v.name << "[idx]))/(" << nf << ");\n";
            } else {
              out << "this->zeros[" << n << "+idx] = " << b.lowerBound
                  << "- (this->" << v.name << "[idx]);\n";
            }
          } else {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "this->zeros[" << n << "+idx] = - (this->" << v.name
                  << "[idx])/(" << nf << ");\n";
            } else {
              out << "this->zeros[" << n << "+idx] = - (this->" << v.name
                  << "[idx]);\n";
            }
          }
          out << "}\n"
              << "}\n";
        }
        if ((mb.getTypeFlag(v.type) != SupportedTypes::Scalar) &&
            (v.arraySize == 1u)) {
          if (b.component == -1) {
            const auto n2 = mb.getTypeSize(v.type, 1u);
            out << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){\n";
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "if(this->" << v.name << "[idx]+(" << nf
                  << ")*(this->zeros[" << n << "+idx])<" << b.lowerBound
                  << "){\n";
            } else {
              out << "if(this->" << v.name << "[idx]+this->zeros[" << n
                  << "+idx]<" << b.lowerBound << "){\n";
            }
            if (std::abs(b.lowerBound) >
                std::numeric_limits<long double>::min()) {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+idx] = (" << b.lowerBound
                    << "- (this->" << v.name << "[idx]))/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+idx] = " << b.lowerBound
                    << "- (this->" << v.name << "[idx]);\n";
              }
            } else {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+idx] = - (this->" << v.name
                    << "[idx])/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+idx] = - (this->" << v.name
                    << "[idx]);\n";
              }
            }
            out << "}\n"
                << "}\n";
          } else {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "if(this->" << v.name << "[" << b.component << "]+(" << nf
                  << ")*(this->zeros[" << n << "+" << b.component << "])<"
                  << b.lowerBound << "){\n";
            } else {
              out << "if(this->" << v.name << "[" << b.component
                  << "]+this->zeros[" << n << "+" << b.component << "]<"
                  << b.lowerBound << "){\n";
            }
            if (std::abs(b.lowerBound) >
                std::numeric_limits<long double>::min()) {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << b.component << "] = ("
                    << b.lowerBound << "- (this->" << v.name << "["
                    << b.component << "]))/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << b.component
                    << "] = " << b.lowerBound << "- (this->" << v.name << "["
                    << b.component << "]);\n";
              }
            } else {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << b.component
                    << "] = - (this->" << v.name << "[" << b.component << "])/("
                    << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << b.component
                    << "] = - (this->" << v.name << "[" << b.component
                    << "]);\n";
              }
            }
            out << "}\n";
          }
        }
        if ((mb.getTypeFlag(v.type) != SupportedTypes::Scalar) &&
            (v.arraySize != 1u)) {
          const auto n2 = mb.getTypeSize(v.type, 1u);
          out << "for(unsigned short idx=0;idx!=" << v.arraySize
              << ";++idx){\n";
          if (b.component == -1) {
            out << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){\n";
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "if(this->" << v.name << "[idx][idx2]+(" << nf
                  << ")*(this->zeros[" << n << "+" << n2 << "*idx+idx2])<"
                  << b.lowerBound << "){\n";
            } else {
              out << "if(this->" << v.name << "[idx][idx2]+this->zeros[" << n
                  << "+" << n2 << "*idx+idx2]<" << b.lowerBound << "){\n";
            }
            if (std::abs(b.lowerBound) >
                std::numeric_limits<long double>::min()) {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = ("
                    << b.lowerBound << "- (this->" << v.name
                    << "[idx][idx2]))/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << n2
                    << "*idx+idx2] = " << b.lowerBound << "- (this->" << v.name
                    << "[idx][idx2]);\n";
              }
            } else {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << n2
                    << "*idx+idx2] = - (this->" << v.name << "[idx][idx2])/("
                    << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << n2
                    << "*idx+idx2] = - (this->" << v.name << "[idx][idx2]);\n";
              }
            }
            out << "}\n"
                << "}\n";
          } else {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "if(this->" << v.name << "[idx][" << b.component << "]+("
                  << nf << ")*(this->zeros[" << n << "+" << n2 << "*idx+"
                  << b.component << "])<" << b.lowerBound << "){\n";
            } else {
              out << "if(this->" << v.name << "[idx][" << b.component
                  << "]+this->zeros[" << n << "+" << n2 << "*idx+"
                  << b.component << "]<" << b.lowerBound << "){\n";
            }
            if (std::abs(b.lowerBound) >
                std::numeric_limits<long double>::min()) {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << n2 << "*idx+"
                    << b.component << "] = (" << b.lowerBound << "- (this->"
                    << v.name << "[idx][" << b.component << "]))/(" << nf
                    << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << n2 << "*idx+"
                    << b.component << "] = " << b.lowerBound << "- (this->"
                    << v.name << "[idx][" << b.component << "]);\n";
              }
            } else {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << n2 << "*idx+"
                    << b.component << "] = - (this->" << v.name << "[idx]["
                    << b.component << "])/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << n2 << "*idx+"
                    << b.component << "] = - (this->" << v.name << "[idx]["
                    << b.component << "]);\n";
              }
            }
            out << "}\n";
          }
          out << "}\n";
        }
      }
      // treating upper bounds
      if ((b.boundsType == VariableBoundsDescription::UPPER) ||
          (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
        if ((mb.getTypeFlag(v.type) == SupportedTypes::Scalar) &&
            (v.arraySize == 1u)) {
          if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
            const auto& nf = mb.getAttribute<std::string>(
                h, v.name + "_normalisation_factor");
            out << "if(this->" << v.name << "+ " << nf << "*(this->zeros[" << n
                << "]) >" << b.upperBound << "){\n";
          } else {
            out << "if(this->" << v.name << "+this->zeros[" << n << "]>"
                << b.upperBound << "){\n";
          }
          if (std::abs(b.upperBound) >
              std::numeric_limits<long double>::min()) {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "this->zeros[" << n << "] = (" << b.upperBound
                  << "- (this->" << v.name << "))/(" << nf << ");\n";
            } else {
              out << "this->zeros[" << n << "] = " << b.upperBound
                  << "- (this->" << v.name << ");\n";
            }
          } else {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "this->zeros[" << n << "] = - (this->" << v.name << ")/("
                  << nf << ");\n";
            } else {
              out << "this->zeros[" << n << "] = - (this->" << v.name << ");\n";
            }
          }
          out << "}\n";
        }
        if ((mb.getTypeFlag(v.type) == SupportedTypes::Scalar) &&
            (v.arraySize != 1u)) {
          out << "for(unsigned short idx=0;idx!=" << v.arraySize
              << ";++idx){\n";
          if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
            const auto& nf = mb.getAttribute<std::string>(
                h, v.name + "_normalisation_factor");
            out << "if(this->" << v.name << "[idx]+(" << nf << ")*(this->zeros["
                << n << "+idx])>" << b.upperBound << "){\n";
          } else {
            out << "if(this->" << v.name << "[idx]+this->zeros[" << n
                << "+idx]>" << b.upperBound << "){\n";
          }
          if (std::abs(b.upperBound) >
              std::numeric_limits<long double>::min()) {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "this->zeros[" << n << "+idx] = (" << b.upperBound
                  << "- (this->" << v.name << "[idx]))/(" << nf << ");\n";
            } else {
              out << "this->zeros[" << n << "+idx] = " << b.upperBound
                  << "- (this->" << v.name << "[idx]);\n";
            }
          } else {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "this->zeros[" << n << "+idx] = - (this->" << v.name
                  << "[idx])/(" << nf << ");\n";
            } else {
              out << "this->zeros[" << n << "+idx] = - (this->" << v.name
                  << "[idx]);\n";
            }
          }
          out << "}\n";
          out << "}\n";
        }
        if ((mb.getTypeFlag(v.type) != SupportedTypes::Scalar) &&
            (v.arraySize == 1u)) {
          if (b.component == -1) {
            const auto n2 = mb.getTypeSize(v.type, 1u);
            out << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){\n";
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "if(this->" << v.name << "[idx]+(" << nf
                  << ")*(this->zeros[" << n << "+idx])>" << b.upperBound
                  << "){\n";
            } else {
              out << "if(this->" << v.name << "[idx]+this->zeros[" << n
                  << "+idx]>" << b.upperBound << "){\n";
            }
            if (std::abs(b.upperBound) >
                std::numeric_limits<long double>::min()) {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+idx] = (" << b.upperBound
                    << "- (this->" << v.name << "[idx]))/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+idx] = " << b.upperBound
                    << "- (this->" << v.name << "[idx]);\n";
              }
            } else {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+idx] = - (this->" << v.name
                    << "[idx])/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+idx] = - (this->" << v.name
                    << "[idx]);\n";
              }
            }
            out << "}\n"
                << "}\n";
          } else {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "if(this->" << v.name << "[" << b.component << "]+(" << nf
                  << ")*(this->zeros[" << n << "+" << b.component << "])>"
                  << b.upperBound << "){\n";
            } else {
              out << "if(this->" << v.name << "[" << b.component
                  << "]+this->zeros[" << n << "+" << b.component << "]>"
                  << b.upperBound << "){\n";
            }
            if (std::abs(b.upperBound) >
                std::numeric_limits<long double>::min()) {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << b.component << "] = ("
                    << b.upperBound << "- (this->" << v.name << "["
                    << b.component << "]))/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << b.component
                    << "] = " << b.upperBound << "- (this->" << v.name << "["
                    << b.component << "]);\n";
              }
            } else {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << b.component
                    << "] = - (this->" << v.name << "[" << b.component << "])/("
                    << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << b.component
                    << "] = - (this->" << v.name << "[" << b.component
                    << "]);\n";
              }
            }
            out << "}\n";
          }
        }
        if ((mb.getTypeFlag(v.type) != SupportedTypes::Scalar) &&
            (v.arraySize != 1u)) {
          const auto n2 = mb.getTypeSize(v.type, 1u);
          out << "for(unsigned short idx=0;idx!=" << v.arraySize
              << ";++idx){\n";
          if (b.component == -1) {
            out << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){\n";
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "if(this->" << v.name << "[idx][idx2]+(" << nf
                  << ")*(this->zeros[" << n << "+" << n2 << "*idx+idx2])>"
                  << b.upperBound << "){\n";
            } else {
              out << "if(this->" << v.name << "[idx][idx2]+this->zeros[" << n
                  << "+" << n2 << "*idx+idx2]>" << b.upperBound << "){\n";
            }
            if (std::abs(b.upperBound) >
                std::numeric_limits<long double>::min()) {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = ("
                    << b.upperBound << "- (this->" << v.name
                    << "[idx][idx2]))/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << n2
                    << "*idx+idx2] = " << b.upperBound << "- (this->" << v.name
                    << "[idx][idx2]);\n";
              }
            } else {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << n2
                    << "*idx+idx2] = - (this->" << v.name << "[idx][idx2])/("
                    << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << n2
                    << "*idx+idx2] = - (this->" << v.name << "[idx][idx2]);\n";
              }
            }
            out << "}\n"
                << "}\n";
          } else {
            if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              out << "if(this->" << v.name << "[idx][" << b.component << "]+("
                  << nf << ")*(this->zeros[" << n << "+" << n2 << "*idx+"
                  << b.component << "])>" << b.upperBound << "){\n";
            } else {
              out << "if(this->" << v.name << "[idx][" << b.component
                  << "]+this->zeros[" << n << "+" << n2 << "*idx+"
                  << b.component << "]>" << b.upperBound << "){\n";
            }
            if (std::abs(b.upperBound) >
                std::numeric_limits<long double>::min()) {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << n2 << "*idx+"
                    << b.component << "] = (" << b.upperBound << "- (this->"
                    << v.name << "[idx][" << b.component << "]))/(" << nf
                    << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << n2 << "*idx+"
                    << b.component << "] = " << b.upperBound << "- (this->"
                    << v.name << "[idx][" << b.component << "]);\n";
              }
            } else {
              if (mb.hasAttribute(h, v.name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<std::string>(
                    h, v.name + "_normalisation_factor");
                out << "this->zeros[" << n << "+" << n2 << "*idx+"
                    << b.component << "] = - (this->" << v.name << "[idx]["
                    << b.component << "])/(" << nf << ");\n";
              } else {
                out << "this->zeros[" << n << "+" << n2 << "*idx+"
                    << b.component << "] = - (this->" << v.name << "[idx]["
                    << b.component << "]);\n";
              }
            }
            out << "}\n";
          }
          out << "}\n";
        }
      }
      n += mb.getTypeSize(v.type, v.arraySize);
    }
  }  // end of
     // NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds

  void NonLinearSystemSolverBase::
      writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(
          std::ostream&, const BehaviourDescription&, const Hypothesis) {
  }  // end of
     // NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds

  NonLinearSystemSolverBase::~NonLinearSystemSolverBase() = default;

}  // end of namespace mfront
