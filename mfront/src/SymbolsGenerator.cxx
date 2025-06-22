/*!
 * \file   mfront/src/SymbolsGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetTFELVersion.h"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourMaterialProperty.hxx"
#include "MFront/StandardBehaviourInterface.hxx"
#include "MFront/SymbolsGenerator.hxx"

namespace mfront {

  static int getVariableTypeId(const VariableDescription& v) {
    switch (SupportedTypes::getTypeFlag(v.type)) {
      case SupportedTypes::Scalar:
        return 0;
        break;
      case SupportedTypes::Stensor:
        return 1;
        break;
      case SupportedTypes::TVector:
        return 2;
        break;
      case SupportedTypes::Tensor:
        return 3;
        break;
      default:
        tfel::raise(
            "getVariableTypeId: "
            "internal error, tag unsupported for "
            "variable '" +
            v.name + "'");
    }
  }  // end of getVariableTypeId

  std::string SymbolsGenerator::getSymbolName(
      const StandardBehaviourInterface& i,
      const std::string& n,
      const Hypothesis h) const {
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return i.getFunctionNameBasis(n) + "_" + ModellingHypothesis::toString(h);
    }
    return i.getFunctionNameBasis(n);
  }  // end of SymbolsGenerator::getSymbolName

  void SymbolsGenerator::generateGeneralSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& bd,
      const FileDescription& fd,
      const std::set<Hypothesis>& mhs,
      const std::string& name) const {
    this->writeEntryPointSymbol(out, i, name);
    this->writeTFELVersionSymbol(out, i, name);
    this->writeMaterialSymbol(out, i, bd, name);
    this->writeMaterialKnowledgeTypeSymbol(out, i, name);
    this->writeInterfaceNameSymbols(out, i, bd, fd, name);
    this->writeSourceFileSymbols(out, i, bd, fd, name);
    this->writeSupportedModellingHypothesis(out, i, bd, mhs, name);
    this->writeMainVariablesSymbols(out, i, bd, name);
    this->writeBehaviourTypeSymbols(out, i, bd, name);
    this->writeBehaviourKinematicSymbols(out, i, bd, name);
    this->writeSymmetryTypeSymbols(out, i, bd, name);
    this->writeElasticSymmetryTypeSymbols(out, i, bd, name);
    this->writeSpecificSymbols(out, i, bd, fd, name);
  }

  void SymbolsGenerator::writeEntryPointSymbol(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const std::string& n) const {
    mfront::writeEntryPointSymbol(out, i.getFunctionNameBasis(n));
  }  // end of SymbolsGenerator::writeEntryPointSymbol

  void SymbolsGenerator::writeTFELVersionSymbol(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const std::string& n) const {
    mfront::writeTFELVersionSymbol(out, i.getFunctionNameBasis(n));
  }  // end of SymbolsGenerator::writeTFELVersionSymbol

  void SymbolsGenerator::writeMaterialSymbol(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& n) const {
    mfront::writeMaterialSymbol(out, i.getFunctionNameBasis(n),
                                mb.getMaterialName());
  }  // end of SymbolsGenerator::writeEntryPointSymbol

  void SymbolsGenerator::writeMainVariablesSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    const auto& mvs = bd.getMainVariables();
    std::vector<std::string> dvnames;
    std::vector<int> dvtypes;
    std::vector<std::string> thnames;
    std::vector<int> thtypes;
    auto nmvs = decltype(mvs.size()){};
    for (const auto& mv : mvs) {
      const auto& dv = mv.first;
      const auto& th = mv.second;
      if (dv.arraySize != th.arraySize) {
        tfel::raise(
            "SymbolsGenerator::writeMainVariablesSymbols: "
            "array size for driving variable '" +
            dv.name +
            "' "
            "does not match the array size of the associated "
            "thermodynamic forces '" +
            th.name + "'");
      }
      const auto dvname = dv.getExternalName();
      const auto dvtype = getVariableTypeId(dv);
      const auto thname = th.getExternalName();
      const auto thtype = getVariableTypeId(th);
      if (dvname == "Strain") {
        if (thname != "Stress") {
          tfel::raise(
              "SymbolsGenerator::writeMainVariablesSymbols: "
              "the thermodynamic force associated with the "
              "strain is not the stress");
        }
        if (dvtype != 1) {
          tfel::raise(
              "SymbolsGenerator::writeMainVariablesSymbols: "
              "invalid type for the strain");
        }
        if (thtype != 1) {
          tfel::raise(
              "SymbolsGenerator::writeMainVariablesSymbols: "
              "invalid type for the stress");
        }
        if (dv.arraySize != 1) {
          tfel::raise(
              "SymbolsGenerator::writeMainVariablesSymbols: "
              "the strain shall not be declared as an array");
        }
        if (th.arraySize != 1) {
          tfel::raise(
              "SymbolsGenerator::writeMainVariablesSymbols: "
              "the stress shall not be declared as an array");
        }
        if ((bd.isStrainMeasureDefined()) &&
            (bd.getStrainMeasure() != BehaviourDescription::LINEARISED)) {
          if (this->handleStrainMeasure()) {
            // strain measure is handled by the interface
            dvnames.push_back("DeformationGradient");
            thnames.push_back("Stress");
            dvtypes.push_back(3);
            thtypes.push_back(thtype);
          } else {
            // strain measure is handled by the calling finite element solver
            dvnames.push_back(dvname);
            thnames.push_back(thname);
            dvtypes.push_back(dvtype);
            thtypes.push_back(thtype);
          }
        } else {
          dvnames.push_back(dvname);
          thnames.push_back(thname);
          dvtypes.push_back(dvtype);
          thtypes.push_back(thtype);
        }
      } else {
        if (dv.arraySize == 1u) {
          dvnames.push_back(dvname);
          thnames.push_back(thname);
          dvtypes.push_back(dvtype);
          thtypes.push_back(thtype);
        } else {
          for (unsigned short idx = 0; idx != dv.arraySize; ++idx) {
            std::ostringstream nb;
            nb << '[' << idx << ']';
            dvnames.push_back(dvname + nb.str());
            thnames.push_back(thname + nb.str());
            dvtypes.push_back(dvtype);
            thtypes.push_back(thtype);
          }
        }
      }
      nmvs += dv.arraySize;
    }
    const auto fn = i.getFunctionNameBasis(name);
    out << "MFRONT_SHAREDOBJ unsigned short " << fn
        << "_nMainVariables = " << nmvs << ";\n";
    out << "MFRONT_SHAREDOBJ unsigned short " << fn << "_nGradients = " << nmvs
        << ";\n\n";
    this->writeArrayOfIntsSymbol(out, fn + "_GradientsTypes", dvtypes);
    this->writeArrayOfStringsSymbol(out, fn + "_Gradients", dvnames);
    out << "MFRONT_SHAREDOBJ unsigned short " << fn
        << "_nThermodynamicForces = " << nmvs << ";\n\n";
    this->writeArrayOfIntsSymbol(out, fn + "_ThermodynamicForcesTypes",
                                 thtypes);
    this->writeArrayOfStringsSymbol(out, fn + "_ThermodynamicForces", thnames);
  }  // end of SymbolsGenerator::writeMainVariablesSymbols

  void SymbolsGenerator::writeMaterialKnowledgeTypeSymbol(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const std::string& n) const {
    mfront::writeMaterialKnowledgeTypeSymbol(out, i.getFunctionNameBasis(n),
                                             BEHAVIOUR);
  }  // end of SymbolsGenerator::writeEntryPointSymbol

  void SymbolsGenerator::generateSymbols(std::ostream& out,
                                         const StandardBehaviourInterface& i,
                                         const BehaviourDescription& bd,
                                         const FileDescription& fd,
                                         const std::string& name,
                                         const Hypothesis h) const {
    this->writeIsUsableInPurelyImplicitResolutionSymbols(out, i, bd, name, h);
    this->writeMaterialPropertiesSymbols(out, i, bd, name, h);
    this->writeStateVariablesSymbols(out, i, bd, name, h);
    this->writeExternalStateVariablesSymbols(out, i, bd, name, h);
    this->writeParametersSymbols(out, i, bd, name, h);
    this->writeParameterDefaultValueSymbols(out, i, bd, name, h);
    this->writeBoundsSymbols(out, i, bd, name, h);
    this->writePhysicalBoundsSymbols(out, i, bd, name, h);
    this->writeRequirementsSymbols(out, i, bd, name, h);
    this->writeAdditionalSymbols(out, i, bd, fd, name, h);
  }

  void SymbolsGenerator::writeSpecificSymbols(std::ostream&,
                                              const StandardBehaviourInterface&,
                                              const BehaviourDescription&,
                                              const FileDescription&,
                                              const std::string&) const {
  }  // end of SymbolsGenerator::writeSpecificSymbols

  void SymbolsGenerator::writeSupportedModellingHypothesis(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription&,
      const std::set<Hypothesis>& mhs,
      const std::string& name) const {
    if (mhs.empty()) {
      out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
          << "_nModellingHypotheses = 0u;\n\n"
          << "MFRONT_SHAREDOBJ const char * const * "
          << i.getFunctionNameBasis(name) << "_ModellingHypotheses = 0;\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
          << "_nModellingHypotheses = " << mhs.size() << "u;\n\n"
          << "MFRONT_SHAREDOBJ const char * \n"
          << i.getFunctionNameBasis(name) << "_ModellingHypotheses["
          << mhs.size() << "u] = {";
      for (auto ph = mhs.begin(); ph != mhs.end();) {
        out << "\"" << ModellingHypothesis::toString(*ph) << "\"";
        if (++ph != mhs.end()) {
          out << ",\n";
        }
      }
      out << "};\n\n";
    }
  }  // end of SymbolsGenerator::writeSupportedModellingHypothesis

  void SymbolsGenerator::writeMaterialPropertiesSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    using namespace std;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "SymbolsGenerator::writeMaterialPropertiesSymbols: " + m);
    };
    const auto mprops = i.buildMaterialPropertiesList(mb, h);
    if (mprops.first.empty()) {
      out << "MFRONT_SHAREDOBJ unsigned short "
          << this->getSymbolName(i, name, h) << "_nMaterialProperties = 0u;\n\n"
          << "MFRONT_SHAREDOBJ const char * const *"
          << this->getSymbolName(i, name, h)
          << "_MaterialProperties = nullptr;\n\n";
    } else {
      const auto& last = mprops.first.back();
      SupportedTypes::TypeSize s;
      throw_if((mprops.second.getTensorSize() != 0) ||
                   (mprops.second.getStensorSize() != 0) ||
                   (mprops.second.getTVectorSize() != 0),
               "internal error: the material properties shall all be scalars");
      s = last.offset;
      s += SupportedTypes::getTypeSize(last.type, last.arraySize);
      s -= mprops.second;
      throw_if((s.getTensorSize() != 0) || (s.getStensorSize() != 0) ||
                   (s.getTVectorSize() != 0),
               "internal error: the material properties shall all be scalars");
      throw_if(s.getScalarSize() < 0,
               "internal error: negative number of the material properties");
      vector<BehaviourMaterialProperty>::size_type ib =
          0; /* index of the first element which
              * is not imposed by the material properties */
      bool found = false;
      for (decltype(mprops.first.size()) idx = 0;
           (idx != mprops.first.size()) && (!found); ++idx) {
        if (mprops.first[idx].offset == mprops.second) {
          ib = idx;
          found = true;
        }
      }
      if (!found) {
        throw_if(s.getScalarSize() != 0,
                 "internal error: inconsistent offset declaration");
        out << "MFRONT_SHAREDOBJ unsigned short "
            << this->getSymbolName(i, name, h)
            << "_nMaterialProperties = 0u;\n\n";
        out << "MFRONT_SHAREDOBJ const char * const *"
            << this->getSymbolName(i, name, h)
            << "_MaterialProperties = nullptr;\n\n";
      } else {
        out << "MFRONT_SHAREDOBJ unsigned short "
            << this->getSymbolName(i, name, h)
            << "_nMaterialProperties = " << s.getScalarSize() << "u;\n\n";
        out << "MFRONT_SHAREDOBJ const char *"
            << this->getSymbolName(i, name, h) << "_MaterialProperties["
            << s.getScalarSize() << "u] = {";
        for (auto idx = ib; idx != mprops.first.size();) {
          const auto& m = mprops.first[idx];
          if (m.arraySize == 1u) {
            out << "\"" << m.name << "\"";
          } else {
            for (unsigned short j = 0; j != m.arraySize;) {
              out << "\"" << m.name << "[" << j << "]\"";
              if (++j != m.arraySize) {
                out << ",\n";
              }
            }
          }
          if (++idx != mprops.first.size()) {
            out << ",\n";
          }
        }
        out << "};\n\n";
      }
    }
  }  // end of UMATInterface::writeMaterialPropertiesSymbol

  void SymbolsGenerator::writeStateVariablesSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const unsigned short nStateVariables = static_cast<unsigned short>(
        persistentVarsHolder.getNumberOfVariables());
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(i, name, h)
        << "_nInternalStateVariables = " << nStateVariables << ";\n";
    std::vector<std::string> stateVariablesNames;
    mb.getExternalNames(stateVariablesNames, h, persistentVarsHolder);
    this->writeExternalNames(out, i, name, h, stateVariablesNames,
                             "InternalStateVariables");
    if (!persistentVarsHolder.empty()) {
      out << "MFRONT_SHAREDOBJ int " << this->getSymbolName(i, name, h)
          << "_InternalStateVariablesTypes [] = {";
      for (auto p = persistentVarsHolder.begin();
           p != persistentVarsHolder.end();) {
        const auto t = getVariableTypeId(*p);
        for (unsigned short is = 0; is != p->arraySize;) {
          out << t;
          if (++is != p->arraySize) {
            out << ",";
          }
        }
        if (++p != persistentVarsHolder.end()) {
          out << ",";
        }
      }
      out << "};\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ const int * " << this->getSymbolName(i, name, h)
          << "_InternalStateVariablesTypes = nullptr;\n\n";
    }
  }  // end of SymbolsGenerator::writeStateVariablesSymbols

  void SymbolsGenerator::writeExternalStateVariablesSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    auto evs = d.getExternalStateVariables();
    // removing the temperature
    evs.erase(evs.begin());
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(i, name, h)
        << "_nExternalStateVariables = " << evs.getNumberOfVariables() << ";\n";
    this->writeExternalNames(out, i, name, h, mb.getExternalNames(h, evs),
                             "ExternalStateVariables");
  }  // end of SymbolsGenerator::writeExternalStateVariablesSymbols

  void SymbolsGenerator::writeParametersSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& parameters = d.getParameters();
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(i, name, h)
        << "_nParameters = " << parameters.getNumberOfVariables() << ";\n";
    this->writeExternalNames(out, i, name, h,
                             mb.getExternalNames(h, parameters), "Parameters");
    if (!parameters.empty()) {
      out << "MFRONT_SHAREDOBJ int " << this->getSymbolName(i, name, h)
          << "_ParametersTypes [] = {";
      for (auto p = parameters.begin(); p != parameters.end();) {
        for (unsigned short is = 0; is != p->arraySize;) {
          if (p->type == "int") {
            out << "1";
          } else if (p->type == "ushort") {
            out << "2";
          } else {
            const auto f = SupportedTypes::getTypeFlag(p->type);
            tfel::raise_if(f != SupportedTypes::Scalar,
                           "SymbolsGenerator::writeParametersSymbols: "
                           "internal error, unsupported type "
                           "for parameter '" +
                               p->name + "'");
            out << "0";
          }
          if (++is != p->arraySize) {
            out << ",";
          }
        }
        if (++p != parameters.end()) {
          out << ",";
        }
      }
      out << "};\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ const int * " << this->getSymbolName(i, name, h)
          << "_ParametersTypes = nullptr;\n\n";
    }
  }  // end of SymbolsGenerator::writeParametersSymbols

  void SymbolsGenerator::writeParameterDefaultValueSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "SymbolsGenerator::"
                     "writeParameterDefaultValueSymbols: " +
                         m);
    };
    for (const auto& p : mb.getBehaviourData(h).getParameters()) {
      if (p.type == "int") {
        throw_if(p.arraySize != 1u,
                 "unsupported parameters array of type '" + p.type + "'");
        out << "MFRONT_SHAREDOBJ int " << this->getSymbolName(i, name, h) << "_"
            << p.getExternalName() << "_ParameterDefaultValue  = "
            << mb.getIntegerParameterDefaultValue(h, p.name) << ";\n\n";
      } else if (p.type == "ushort") {
        throw_if(p.arraySize != 1u,
                 "unsupported parameters array of type '" + p.type + "'");
        out << "MFRONT_SHAREDOBJ unsigned short "
            << this->getSymbolName(i, name, h) << "_" << p.getExternalName()
            << "_ParameterDefaultValue  = "
            << mb.getUnsignedShortParameterDefaultValue(h, p.name) << ";\n\n";
      } else {
        const auto f = SupportedTypes::getTypeFlag(p.type);
        throw_if(f != SupportedTypes::Scalar,
                 "unsupported paramaeter type '" + p.type + "'");
        const auto prec = out.precision();
        out.precision(14);
        if (p.arraySize == 1u) {
          out << "MFRONT_SHAREDOBJ double " << this->getSymbolName(i, name, h)
              << "_" << p.getExternalName() << "_ParameterDefaultValue"
              << " = " << mb.getFloattingPointParameterDefaultValue(h, p.name)
              << ";\n\n";
        } else {
          for (unsigned short is = 0; is != p.arraySize; ++is) {
            out << "MFRONT_SHAREDOBJ double " << this->getSymbolName(i, name, h)
                << "_" << p.getExternalName() << "__" << is
                << "___ParameterDefaultValue = "
                << mb.getFloattingPointParameterDefaultValue(h, p.name, is)
                << ";\n\n";
          }
        }
        out.precision(prec);
      }
    }
  }  // end of SymbolsGenerator::writeParameterDefaultValueSymbols

  static void writeBoundsSymbol(std::ostream& out,
                                const std::string& bn,
                                const std::string& vn,
                                const std::string& bt,
                                const VariableBoundsDescription& b) {
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "MFRONT_SHAREDOBJ long double " << bn << "_" << vn << "_"
          << "Lower" << bt << "Bound = " << b.lowerBound << ";\n\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "MFRONT_SHAREDOBJ long double " << bn << "_" << vn << "_"
          << "Upper" << bt << "Bound = " << b.upperBound << ";\n\n";
    }
  }  // end of writeBoundsSymbol

  void SymbolsGenerator::writeBoundsSymbols(std::ostream& out,
                                            const StandardBehaviourInterface& i,
                                            const BehaviourDescription& mb,
                                            const std::string& name,
                                            const Hypothesis h) const {
    auto write = [this, &out, &name, &i,
                  h](const VariableDescriptionContainer& vc) {
      const auto prec = out.precision();
      out.precision(14);
      for (const auto& v : vc) {
        if (v.arraySize == 1u) {
          if (!v.hasBounds()) {
            continue;
          }
          mfront::writeBoundsSymbol(out, this->getSymbolName(i, name, h),
                                    v.getExternalName(), "", v.getBounds());
        } else {
          for (auto idx = 0; idx != v.arraySize; ++idx) {
            if (!v.hasBounds(idx)) {
              continue;
            }
            mfront::writeBoundsSymbol(
                out, this->getSymbolName(i, name, h),
                v.getExternalName() + "__" + std::to_string(idx) + "__", "",
                v.getBounds(idx));
          }
        }
      }
      out.precision(prec);
    };
    const auto& d = mb.getBehaviourData(h);
    write(d.getMaterialProperties());
    write(d.getPersistentVariables());
    write(d.getExternalStateVariables());
    write(d.getParameters());
  }  // end of SymbolsGenerator::writeBoundsSymbols

  void SymbolsGenerator::writePhysicalBoundsSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    auto write = [this, &out, &i, &name,
                  h](const VariableDescriptionContainer& vc) {
      const auto prec = out.precision();
      out.precision(14);
      for (const auto& v : vc) {
        if (v.arraySize == 1u) {
          if (!v.hasPhysicalBounds()) {
            continue;
          }
          mfront::writeBoundsSymbol(out, this->getSymbolName(i, name, h),
                                    v.getExternalName(), "Physical",
                                    v.getPhysicalBounds());
        } else {
          for (auto idx = 0; idx != v.arraySize; ++idx) {
            if (!v.hasPhysicalBounds(idx)) {
              continue;
            }
            mfront::writeBoundsSymbol(
                out, this->getSymbolName(i, name, h),
                v.getExternalName() + "__" + std::to_string(idx) + "__",
                "Physical", v.getPhysicalBounds(idx));
          }
        }
      }
      out.precision(prec);
    };
    const auto& d = mb.getBehaviourData(h);
    write(d.getMaterialProperties());
    write(d.getPersistentVariables());
    write(d.getExternalStateVariables());
    write(d.getParameters());
  }  // end of SymbolsGenerator::writePhysicalBoundsSymbols

  void SymbolsGenerator::writeRequirementsSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    out << "MFRONT_SHAREDOBJ unsigned short "
        << this->getSymbolName(i, name, h);
    out << "_requiresStiffnessTensor = ";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
    out << "MFRONT_SHAREDOBJ unsigned short "
        << this->getSymbolName(i, name, h);
    out << "_requiresThermalExpansionCoefficientTensor = ";
    if (mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";

  }  // end of SymbolsGenerator::writeRequirementsSymbols

  void SymbolsGenerator::writeIsUsableInPurelyImplicitResolutionSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(i, name, h)
        << "_UsableInPurelyImplicitResolution = ";
    if (d.isUsableInPurelyImplicitResolution()) {
      out << "1;\n\n";
    } else {
      out << "0;\n\n";
    }
  }  // end of
     // SymbolsGenerator::writeIsUsableInPurelyImplicitResolution

  void SymbolsGenerator::writeSymmetryTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_SymmetryType = ";
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "0u;\n\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "1u;\n\n";
    } else {
      tfel::raise(
          "SymbolsGenerator::writeSymmetryTypeSymbols: "
          "unsupported behaviour type.\n"
          "only isotropic or orthotropic behaviours "
          "are supported at this time.");
    }
  }  // end of SymbolsGenerator::writeSymmetryTypeSymbols

  void SymbolsGenerator::writeElasticSymmetryTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_ElasticSymmetryType = ";
    if (mb.getElasticSymmetryType() == mfront::ISOTROPIC) {
      out << "0u;\n\n";
    } else if (mb.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
      out << "1u;\n\n";
    } else {
      tfel::raise(
          "SymbolsGenerator::writeElasticSymmetryTypeSymbols: "
          "unsupported behaviour type.\n"
          "only isotropic or orthotropic behaviours are "
          "supported at this time.");
    }
  }  // end of SymbolsGenerator::writeElasticSymmetryTypeSymbols

  void SymbolsGenerator::writeSourceFileSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription&,
      const mfront::FileDescription& fd,
      const std::string& name) const {
    using namespace tfel::system;
    using namespace tfel::utilities;
    out << "MFRONT_SHAREDOBJ const char *\n"
        << i.getFunctionNameBasis(name) << "_src = \""
        << tokenize(fd.fileName, dirSeparator()).back() << "\";\n\n";
  }

  void SymbolsGenerator::writeInterfaceNameSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription&,
      const mfront::FileDescription&,
      const std::string& name) const {
    writeInterfaceSymbol(out, i.getFunctionNameBasis(name),
                         i.getInterfaceName());
  }

  void SymbolsGenerator::writeExternalNames(std::ostream& f,
                                            const StandardBehaviourInterface& i,
                                            const std::string& name,
                                            const Hypothesis& h,
                                            const std::vector<std::string>& v,
                                            const std::string& t) const {
    this->writeArrayOfStringsSymbol(
        f, this->getSymbolName(i, name, h) + "_" + t, v);
  }  // end of SymbolsGenerator::writeExternalNames

  void SymbolsGenerator::writeArrayOfStringsSymbol(
      std::ostream& f,
      const std::string& s,
      const std::vector<std::string>& v) const {
    if (v.empty()) {
      f << "MFRONT_SHAREDOBJ const char * const * " << s << " = nullptr;\n\n";
    } else {
      auto i = decltype(v.size()){};
      auto p = v.begin();
      f << "MFRONT_SHAREDOBJ const char * " << s << "[" << v.size() << "] = {";
      while (p != v.end()) {
        f << '"' << *p << '"';
        if (++p != v.end()) {
          if (i % 5 == 0) {
            f << ",\n";
          } else {
            f << ",";
          }
        }
        ++i;
      }
      f << "};\n";
    }
  }  // end of SymbolsGenerator::writeArrayOfStringsSymbol

  void SymbolsGenerator::writeArrayOfIntsSymbol(
      std::ostream& f, const std::string& s, const std::vector<int>& v) const {
    if (v.empty()) {
      f << "MFRONT_SHAREDOBJ const int * " << s << " = nullptr;\n\n";
    } else {
      auto i = decltype(v.size()){};
      auto p = v.begin();
      f << "MFRONT_SHAREDOBJ int " << s << "[" << v.size() << "] = {";
      while (p != v.end()) {
        f << *p;
        if (++p != v.end()) {
          if (i % 5 == 0) {
            f << ",\n";
          } else {
            f << ",";
          }
        }
        ++i;
      }
      f << "};\n";
    }
  }  // end of SymbolsGenerator::writeArrayOfIntsSymbol

  SymbolsGenerator::~SymbolsGenerator() = default;

}  // end of namespace mfront
