/*!
 * \file  mfront/src/NonLinearSystemSolverBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <limits>
#include <sstream>

#include "MFront/BehaviourDescription.hxx"
#include "MFront/NonLinearSystemSolverBase.hxx"

namespace mfront {

  std::vector<std::string> NonLinearSystemSolverBase::getReservedNames() const {
    return {"error", "iter", "iterMax", "converged"};
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
    using namespace std;
    ostringstream d;
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
        string msg("NonLinearSystemSolverBase::getJacobianPart : ");
        msg += "unsupported type for integration variable ";
        msg += v2.name;
        throw(runtime_error(msg));
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
        string msg("NonLinearSystemSolverBase::getJacobianPart : ");
        msg += "unsupported type for integration variable ";
        msg += v2.name;
        throw(runtime_error(msg));
      }
    } else {
      string msg("NonLinearSystemSolverBase::getJacobianPart : ");
      msg += "unsupported type for integration variable ";
      msg += v1.name;
      throw(runtime_error(msg));
    }
    return d.str();
  }  // end of NonLinearSystemSolverBase::getJacobianPart

  void NonLinearSystemSolverBase::writeComparisonToNumericalJacobian(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h,
      const std::string& nj) {
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    for (p = d.getIntegrationVariables().begin();
         p != d.getIntegrationVariables().end(); ++p) {
      n2 += mb.getTypeSize(p->type, p->arraySize);
    }
    if (mb.getAttribute(h, BehaviourData::compareToNumericalJacobian, false)) {
      out << "this->computeNumericalJacobian(" << nj << ");\n";
      n = SupportedTypes::TypeSize();
      for (p = d.getIntegrationVariables().begin();
           p != d.getIntegrationVariables().end(); ++p) {
        if (p->arraySize == 1) {
          n3 = SupportedTypes::TypeSize();
          for (p2 = d.getIntegrationVariables().begin();
               p2 != d.getIntegrationVariables().end(); ++p2) {
            if ((p->arraySize == 1) && (p2->arraySize == 1)) {
              out << "// derivative of variable f" << p->name << " by variable "
                  << p2->name << "\n";
              out << NonLinearSystemSolverBase::getJacobianPart(mb, *p, *p2, n,
                                                                n2, n3);
              out << "// numerical derivative of variable f" << p->name
                  << " by variable " << p2->name << "\n";
              out << NonLinearSystemSolverBase::getJacobianPart(
                  mb, *p, *p2, n, n2, n3, nj, "n");
              n3 += mb.getTypeSize(p2->type, p2->arraySize);
            }
          }
        }
        n += mb.getTypeSize(p->type, p->arraySize);
      }
      for (p = d.getIntegrationVariables().begin();
           p != d.getIntegrationVariables().end(); ++p) {
        for (p2 = d.getIntegrationVariables().begin();
             p2 != d.getIntegrationVariables().end(); ++p2) {
          const VariableDescription& v1 = *p;
          const VariableDescription& v2 = *p2;
          SupportedTypes::TypeSize nv1 = mb.getTypeSize(v1.type, 1u);
          SupportedTypes::TypeSize nv2 = mb.getTypeSize(v2.type, 1u);
          out << "error=" << nv1 << "*" << nv2 << "*"
              << "(this->jacobianComparisonCriterion)"
              << ";\n";
          if ((v1.arraySize == 1u) && (v2.arraySize == 1u)) {
            out << "if(abs("
                << "df" << v1.name << "_dd" << v2.name << "-"
                << "ndf" << v1.name << "_dd" << v2.name << ") > error)\n"
                << "{\n";
            out << "cout << abs("
                << "df" << v1.name << "_dd" << v2.name << "-"
                << "ndf" << v1.name << "_dd" << v2.name
                << ") << \" \" << error << endl;\n";
            out << "cout << \"df" << v1.name << "_dd" << v2.name
                << " :\\n\" << "
                << "df" << v1.name << "_dd" << v2.name << " << endl;\n";
            out << "cout << \"ndf" << v1.name << "_dd" << v2.name
                << " :\\n\" << "
                << "ndf" << v1.name << "_dd" << v2.name << " << endl;\n";
            out << "cout << \"df" << v1.name << "_dd" << v2.name << " - ndf"
                << v1.name << "_dd" << v2.name << " :\\n\" << "
                << "df" << v1.name << "_dd" << v2.name << "-"
                << "ndf" << v1.name << "_dd" << v2.name << " << endl;\n";
            out << "cout << endl;\n";
            out << "}\n";
          } else if (((v1.arraySize != 1u) && (v2.arraySize == 1u)) ||
                     ((v2.arraySize != 1u) && (v1.arraySize == 1u))) {
            unsigned short asize;
            if (v1.arraySize != 1u) {
              asize = v1.arraySize;
            } else {
              asize = v2.arraySize;
            }
            out << "for(unsigned short idx=0;idx!=" << asize << ";++idx){\n";
            out << "if(abs("
                << "df" << v1.name << "_dd" << v2.name << "(idx)-"
                << "df" << v1.name << "_dd" << v2.name << "(" << nj
                << ",idx)) > error)\n"
                << "{\n";
            out << "cout << abs("
                << "df" << v1.name << "_dd" << v2.name << "(idx)-"
                << "df" << v1.name << "_dd" << v2.name << "(" << nj
                << ",idx)) << \" \" << error << endl;\n";
            out << "cout << \"df" << v1.name << "_dd" << v2.name
                << "(\" << idx << \") :\\n\" << "
                << "df" << v1.name << "_dd" << v2.name << "(idx) << endl;\n";
            out << "cout << \"ndf" << v1.name << "_dd" << v2.name
                << "(\" << idx << \") :\\n\" << "
                << "df" << v1.name << "_dd" << v2.name << "(" << nj
                << ",idx) << endl;\n";
            out << "cout << \"df" << v1.name << "_dd" << v2.name << " - ndf"
                << v1.name << "_dd" << v2.name << "(\" << idx << \") :\\n\" << "
                << "df" << v1.name << "_dd" << v2.name << "(idx) -"
                << "df" << v1.name << "_dd" << v2.name << "(" << nj
                << ",idx) << endl;\n";
            out << "cout << endl;\n";
            out << "}\n";
            out << "}\n";
          } else {
            out << "for(unsigned short idx=0;idx!=" << v1.arraySize
                << ";++idx){\n";
            out << "for(unsigned short idx2=0;idx2!=" << v2.arraySize
                << ";++idx2){\n";
            out << "if(abs("
                << "df" << v1.name << "_dd" << v2.name << "(idx,idx2)-"
                << "df" << v1.name << "_dd" << v2.name << "(" << nj
                << ",idx,idx2)) > error)\n"
                << "{\n";
            out << "cout << abs("
                << "df" << v1.name << "_dd" << v2.name << "(idx,idx2)-"
                << "df" << v1.name << "_dd" << v2.name << "(" << nj
                << ",idx,idx2)) << \" \" << error << endl;\n";
            out << "cout << \"df" << v1.name << "_dd" << v2.name
                << "(\" << idx << \",\" << idx2 << \") :\\n\" << "
                << "df" << v1.name << "_dd" << v2.name
                << "(idx,idx2) << endl;\n";
            out << "cout << \"ndf" << v1.name << "_dd" << v2.name
                << "(\" << idx << \",\" << idx2 << \") :\\n\" << "
                << "df" << v1.name << "_dd" << v2.name << "(" << nj
                << ",idx,idx2) << endl;\n";
            out << "cout << \"df" << v1.name << "_dd" << v2.name << " - ndf"
                << v1.name << "_dd" << v2.name
                << "(\" << idx << \",\" << idx2 << \") :\\n\" << "
                << "df" << v1.name << "_dd" << v2.name << "(idx,idx2) -"
                << "df" << v1.name << "_dd" << v2.name << "(" << nj
                << ",idx,idx2) << endl;\n";
            out << "cout << endl;\n";
            out << "}\n";
            out << "}\n";
            out << "}\n";
          }
        }
      }
    }
  }  // end of NonLinearSystemSolverBase::writeComparisonToNumericalJacobian

  void NonLinearSystemSolverBase::writeLimitsOnIncrementValues(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h,
      const std::string& v) {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for (p = d.getIntegrationVariables().begin();
         p != d.getIntegrationVariables().end(); ++p) {
      SupportedTypes::TypeSize nv = mb.getTypeSize(p->type, p->arraySize);
      if (mb.hasParameter(h,
                          p->name + "_maximum_increment_value_per_iteration")) {
        out << "for(unsigned short idx = 0; idx!=" << nv << ";++idx){\n";
        if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
          const auto& nf =
              mb.getAttribute<string>(h, p->name + "_normalisation_factor");
          out << "if(std::abs(" << v << "[" << n << "+idx])>" << nf
              << "*(this->" << p->name
              << "_maximum_increment_value_per_iteration)){\n";
        } else {
          out << "if(std::abs(" << v << "[" << n << "+idx])>this->" << p->name
              << "_maximum_increment_value_per_iteration){\n";
        }
        out << "if(" << v << "[" << n << "+idx]<0){\n";
        if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
          const auto& nf =
              mb.getAttribute<string>(h, p->name + "_normalisation_factor");
          out << "" << v << "[" << n << "+idx] = -" << nf << "*(this->"
              << p->name << "_maximum_increment_value_per_iteration);\n";
        } else {
          out << "" << v << "[" << n << "+idx] = -this->" << p->name
              << "_maximum_increment_value_per_iteration;\n";
        }
        out << "} else {\n";
        if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
          const auto& nf =
              mb.getAttribute<string>(h, p->name + "_normalisation_factor");
          out << "" << v << "[" << n << "+idx] = " << nf << "*(this->"
              << p->name << "_maximum_increment_value_per_iteration);\n";
        } else {
          out << "" << v << "[" << n << "+idx] =  this->" << p->name
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
            << "+idx] = -this->maximum_increment_value_per_iteration;\n";
        out << "} else {\n";
        out << "" << v << "[" << n
            << "+idx] =  this->maximum_increment_value_per_iteration;\n";
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
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    VariableDescriptionContainer::const_iterator p;
    vector<BoundsDescription>::const_iterator p2;
    for (p = d.getStateVariables().begin(); p != d.getStateVariables().end();
         ++p) {
      for (p2 = d.getBounds().begin(); p2 != d.getBounds().end(); ++p2) {
        if (p2->name == p->name) {
          // treating lower bounds
          if (((p2->boundsType == BoundsDescription::Lower) ||
               (p2->boundsType == BoundsDescription::LowerAndUpper)) &&
              (p2->category == BoundsDescription::Physical)) {
            if ((mb.getTypeFlag(p->type) == SupportedTypes::Scalar) &&
                (p->arraySize == 1u)) {
              if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<string>(
                    h, p->name + "_normalisation_factor");
                out << "if(this->" << p->name << "+ " << nf << "*(this->zeros["
                    << n << "]) <" << p2->lowerBound << "){\n";
              } else {
                out << "if(this->" << p->name << "+this->zeros[" << n << "]<"
                    << p2->lowerBound << "){\n";
              }
              if (abs(p2->lowerBound) > numeric_limits<long double>::min()) {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "this->zeros[" << n << "] = (" << p2->lowerBound
                      << "- (this->" << p->name << "))/(" << nf << ");\n";
                } else {
                  out << "this->zeros[" << n << "] = " << p2->lowerBound
                      << "- (this->" << p->name << ");\n";
                }
              } else {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "this->zeros[" << n << "] = - (this->" << p->name
                      << ")/(" << nf << ");\n";
                } else {
                  out << "this->zeros[" << n << "] = - (this->" << p->name
                      << ");\n";
                }
              }
              out << "}\n";
            }
            if ((mb.getTypeFlag(p->type) == SupportedTypes::Scalar) &&
                (p->arraySize != 1u)) {
              out << "for(unsigned short idx=0;idx!=" << p->arraySize
                  << ";++idx){" << endl;
              if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<string>(
                    h, p->name + "_normalisation_factor");
                out << "if(this->" << p->name << "[idx]+(" << nf
                    << ")*(this->zeros[" << n << "+idx])<" << p2->lowerBound
                    << "){\n";
              } else {
                out << "if(this->" << p->name << "[idx]+this->zeros[" << n
                    << "+idx]<" << p2->lowerBound << "){\n";
              }
              if (abs(p2->lowerBound) > numeric_limits<long double>::min()) {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "this->zeros[" << n << "+idx] = (" << p2->lowerBound
                      << "- (this->" << p->name << "[idx]))/(" << nf << ");\n";
                } else {
                  out << "this->zeros[" << n << "+idx] = " << p2->lowerBound
                      << "- (this->" << p->name << "[idx]);\n";
                }
              } else {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "this->zeros[" << n << "+idx] = - (this->" << p->name
                      << "[idx])/(" << nf << ");\n";
                } else {
                  out << "this->zeros[" << n << "+idx] = - (this->" << p->name
                      << "[idx]);\n";
                }
              }
              out << "}\n";
              out << "}\n";
            }
            if ((mb.getTypeFlag(p->type) != SupportedTypes::Scalar) &&
                (p->arraySize == 1u)) {
              if (p2->component == -1) {
                SupportedTypes::TypeSize n2 = mb.getTypeSize(p->type, 1u);
                out << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){"
                    << endl;
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "if(this->" << p->name << "[idx]+(" << nf
                      << ")*(this->zeros[" << n << "+idx])<" << p2->lowerBound
                      << "){\n";
                } else {
                  out << "if(this->" << p->name << "[idx]+this->zeros[" << n
                      << "+idx]<" << p2->lowerBound << "){\n";
                }
                if (abs(p2->lowerBound) > numeric_limits<long double>::min()) {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+idx] = (" << p2->lowerBound
                        << "- (this->" << p->name << "[idx]))/(" << nf
                        << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+idx] = " << p2->lowerBound
                        << "- (this->" << p->name << "[idx]);\n";
                  }
                } else {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+idx] = - (this->" << p->name
                        << "[idx])/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+idx] = - (this->" << p->name
                        << "[idx]);\n";
                  }
                }
                out << "}\n";
                out << "}\n";
              } else {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "if(this->" << p->name << "[" << p2->component << "]+("
                      << nf << ")*(this->zeros[" << n << "+" << p2->component
                      << "])<" << p2->lowerBound << "){\n";
                } else {
                  out << "if(this->" << p->name << "[" << p2->component
                      << "]+this->zeros[" << n << "+" << p2->component << "]<"
                      << p2->lowerBound << "){\n";
                }
                if (abs(p2->lowerBound) > numeric_limits<long double>::min()) {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << p2->component
                        << "] = (" << p2->lowerBound << "- (this->" << p->name
                        << "[" << p2->component << "]))/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << p2->component
                        << "] = " << p2->lowerBound << "- (this->" << p->name
                        << "[" << p2->component << "]);\n";
                  }
                } else {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << p2->component
                        << "] = - (this->" << p->name << "[" << p2->component
                        << "])/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << p2->component
                        << "] = - (this->" << p->name << "[" << p2->component
                        << "]);\n";
                  }
                }
                out << "}\n";
              }
            }
            if ((mb.getTypeFlag(p->type) != SupportedTypes::Scalar) &&
                (p->arraySize != 1u)) {
              SupportedTypes::TypeSize n2 = mb.getTypeSize(p->type, 1u);
              out << "for(unsigned short idx=0;idx!=" << p->arraySize
                  << ";++idx){" << endl;
              if (p2->component == -1) {
                out << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){"
                    << endl;
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "if(this->" << p->name << "[idx][idx2]+(" << nf
                      << ")*(this->zeros[" << n << "+" << n2 << "*idx+idx2])<"
                      << p2->lowerBound << "){\n";
                } else {
                  out << "if(this->" << p->name << "[idx][idx2]+this->zeros["
                      << n << "+" << n2 << "*idx+idx2]<" << p2->lowerBound
                      << "){\n";
                }
                if (abs(p2->lowerBound) > numeric_limits<long double>::min()) {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = ("
                        << p2->lowerBound << "- (this->" << p->name
                        << "[idx][idx2]))/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << n2
                        << "*idx+idx2] = " << p2->lowerBound << "- (this->"
                        << p->name << "[idx][idx2]);\n";
                  }
                } else {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << n2
                        << "*idx+idx2] = - (this->" << p->name
                        << "[idx][idx2])/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << n2
                        << "*idx+idx2] = - (this->" << p->name
                        << "[idx][idx2]);\n";
                  }
                }
                out << "}\n";
                out << "}\n";
              } else {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "if(this->" << p->name << "[idx][" << p2->component
                      << "]+(" << nf << ")*(this->zeros[" << n << "+" << n2
                      << "*idx+" << p2->component << "])<" << p2->lowerBound
                      << "){\n";
                } else {
                  out << "if(this->" << p->name << "[idx][" << p2->component
                      << "]+this->zeros[" << n << "+" << n2 << "*idx+"
                      << p2->component << "]<" << p2->lowerBound << "){\n";
                }
                if (abs(p2->lowerBound) > numeric_limits<long double>::min()) {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << n2 << "*idx+"
                        << p2->component << "] = (" << p2->lowerBound
                        << "- (this->" << p->name << "[idx][" << p2->component
                        << "]))/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << n2 << "*idx+"
                        << p2->component << "] = " << p2->lowerBound
                        << "- (this->" << p->name << "[idx][" << p2->component
                        << "]);\n";
                  }
                } else {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << n2 << "*idx+"
                        << p2->component << "] = - (this->" << p->name
                        << "[idx][" << p2->component << "])/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << n2 << "*idx+"
                        << p2->component << "] = - (this->" << p->name
                        << "[idx][" << p2->component << "]);\n";
                  }
                }
                out << "}\n";
              }
              out << "}\n";
            }
          }
          // treating upper bounds
          if (((p2->boundsType == BoundsDescription::Upper) ||
               (p2->boundsType == BoundsDescription::LowerAndUpper)) &&
              (p2->category == BoundsDescription::Physical)) {
            if ((mb.getTypeFlag(p->type) == SupportedTypes::Scalar) &&
                (p->arraySize == 1u)) {
              if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<string>(
                    h, p->name + "_normalisation_factor");
                out << "if(this->" << p->name << "+ " << nf << "*(this->zeros["
                    << n << "]) >" << p2->upperBound << "){\n";
              } else {
                out << "if(this->" << p->name << "+this->zeros[" << n << "]>"
                    << p2->upperBound << "){\n";
              }
              if (abs(p2->upperBound) > numeric_limits<long double>::min()) {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "this->zeros[" << n << "] = (" << p2->upperBound
                      << "- (this->" << p->name << "))/(" << nf << ");\n";
                } else {
                  out << "this->zeros[" << n << "] = " << p2->upperBound
                      << "- (this->" << p->name << ");\n";
                }
              } else {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "this->zeros[" << n << "] = - (this->" << p->name
                      << ")/(" << nf << ");\n";
                } else {
                  out << "this->zeros[" << n << "] = - (this->" << p->name
                      << ");\n";
                }
              }
              out << "}\n";
            }
            if ((mb.getTypeFlag(p->type) == SupportedTypes::Scalar) &&
                (p->arraySize != 1u)) {
              out << "for(unsigned short idx=0;idx!=" << p->arraySize
                  << ";++idx){" << endl;
              if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                const auto& nf = mb.getAttribute<string>(
                    h, p->name + "_normalisation_factor");
                out << "if(this->" << p->name << "[idx]+(" << nf
                    << ")*(this->zeros[" << n << "+idx])>" << p2->upperBound
                    << "){\n";
              } else {
                out << "if(this->" << p->name << "[idx]+this->zeros[" << n
                    << "+idx]>" << p2->upperBound << "){\n";
              }
              if (abs(p2->upperBound) > numeric_limits<long double>::min()) {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "this->zeros[" << n << "+idx] = (" << p2->upperBound
                      << "- (this->" << p->name << "[idx]))/(" << nf << ");\n";
                } else {
                  out << "this->zeros[" << n << "+idx] = " << p2->upperBound
                      << "- (this->" << p->name << "[idx]);\n";
                }
              } else {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "this->zeros[" << n << "+idx] = - (this->" << p->name
                      << "[idx])/(" << nf << ");\n";
                } else {
                  out << "this->zeros[" << n << "+idx] = - (this->" << p->name
                      << "[idx]);\n";
                }
              }
              out << "}\n";
              out << "}\n";
            }
            if ((mb.getTypeFlag(p->type) != SupportedTypes::Scalar) &&
                (p->arraySize == 1u)) {
              if (p2->component == -1) {
                SupportedTypes::TypeSize n2 = mb.getTypeSize(p->type, 1u);
                out << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){"
                    << endl;
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "if(this->" << p->name << "[idx]+(" << nf
                      << ")*(this->zeros[" << n << "+idx])>" << p2->upperBound
                      << "){\n";
                } else {
                  out << "if(this->" << p->name << "[idx]+this->zeros[" << n
                      << "+idx]>" << p2->upperBound << "){\n";
                }
                if (abs(p2->upperBound) > numeric_limits<long double>::min()) {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+idx] = (" << p2->upperBound
                        << "- (this->" << p->name << "[idx]))/(" << nf
                        << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+idx] = " << p2->upperBound
                        << "- (this->" << p->name << "[idx]);\n";
                  }
                } else {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+idx] = - (this->" << p->name
                        << "[idx])/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+idx] = - (this->" << p->name
                        << "[idx]);\n";
                  }
                }
                out << "}\n";
                out << "}\n";
              } else {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "if(this->" << p->name << "[" << p2->component << "]+("
                      << nf << ")*(this->zeros[" << n << "+" << p2->component
                      << "])>" << p2->upperBound << "){\n";
                } else {
                  out << "if(this->" << p->name << "[" << p2->component
                      << "]+this->zeros[" << n << "+" << p2->component << "]>"
                      << p2->upperBound << "){\n";
                }
                if (abs(p2->upperBound) > numeric_limits<long double>::min()) {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << p2->component
                        << "] = (" << p2->upperBound << "- (this->" << p->name
                        << "[" << p2->component << "]))/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << p2->component
                        << "] = " << p2->upperBound << "- (this->" << p->name
                        << "[" << p2->component << "]);\n";
                  }
                } else {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << p2->component
                        << "] = - (this->" << p->name << "[" << p2->component
                        << "])/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << p2->component
                        << "] = - (this->" << p->name << "[" << p2->component
                        << "]);\n";
                  }
                }
                out << "}\n";
              }
            }
            if ((mb.getTypeFlag(p->type) != SupportedTypes::Scalar) &&
                (p->arraySize != 1u)) {
              SupportedTypes::TypeSize n2 = mb.getTypeSize(p->type, 1u);
              out << "for(unsigned short idx=0;idx!=" << p->arraySize
                  << ";++idx){" << endl;
              if (p2->component == -1) {
                out << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){"
                    << endl;
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "if(this->" << p->name << "[idx][idx2]+(" << nf
                      << ")*(this->zeros[" << n << "+" << n2 << "*idx+idx2])>"
                      << p2->upperBound << "){\n";
                } else {
                  out << "if(this->" << p->name << "[idx][idx2]+this->zeros["
                      << n << "+" << n2 << "*idx+idx2]>" << p2->upperBound
                      << "){\n";
                }
                if (abs(p2->upperBound) > numeric_limits<long double>::min()) {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << n2 << "*idx+idx2] = ("
                        << p2->upperBound << "- (this->" << p->name
                        << "[idx][idx2]))/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << n2
                        << "*idx+idx2] = " << p2->upperBound << "- (this->"
                        << p->name << "[idx][idx2]);\n";
                  }
                } else {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << n2
                        << "*idx+idx2] = - (this->" << p->name
                        << "[idx][idx2])/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << n2
                        << "*idx+idx2] = - (this->" << p->name
                        << "[idx][idx2]);\n";
                  }
                }
                out << "}\n";
                out << "}\n";
              } else {
                if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                  const auto& nf = mb.getAttribute<string>(
                      h, p->name + "_normalisation_factor");
                  out << "if(this->" << p->name << "[idx][" << p2->component
                      << "]+(" << nf << ")*(this->zeros[" << n << "+" << n2
                      << "*idx+" << p2->component << "])>" << p2->upperBound
                      << "){\n";
                } else {
                  out << "if(this->" << p->name << "[idx][" << p2->component
                      << "]+this->zeros[" << n << "+" << n2 << "*idx+"
                      << p2->component << "]>" << p2->upperBound << "){\n";
                }
                if (abs(p2->upperBound) > numeric_limits<long double>::min()) {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << n2 << "*idx+"
                        << p2->component << "] = (" << p2->upperBound
                        << "- (this->" << p->name << "[idx][" << p2->component
                        << "]))/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << n2 << "*idx+"
                        << p2->component << "] = " << p2->upperBound
                        << "- (this->" << p->name << "[idx][" << p2->component
                        << "]);\n";
                  }
                } else {
                  if (mb.hasAttribute(h, p->name + "_normalisation_factor")) {
                    const auto& nf = mb.getAttribute<string>(
                        h, p->name + "_normalisation_factor");
                    out << "this->zeros[" << n << "+" << n2 << "*idx+"
                        << p2->component << "] = - (this->" << p->name
                        << "[idx][" << p2->component << "])/(" << nf << ");\n";
                  } else {
                    out << "this->zeros[" << n << "+" << n2 << "*idx+"
                        << p2->component << "] = - (this->" << p->name
                        << "[idx][" << p2->component << "]);\n";
                  }
                }
                out << "}\n";
              }
              out << "}\n";
            }
          }
        }
      }
      n += mb.getTypeSize(p->type, p->arraySize);
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
