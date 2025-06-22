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
#include "MFront/BehaviourInterfaceBase.hxx"
#include "MFront/SymbolsGenerator.hxx"

namespace mfront {

  static bool shallRemoveTemperatureFromExternalStateVariables(
      const BehaviourDescription& bd) {
    return bd.isTemperatureDefinedAsTheFirstExternalStateVariable();
  }  // end of shallRemoveTemperatureFromExternalStateVariables

  std::string SymbolsGenerator::getSymbolName(const BehaviourInterfaceBase& i,
                                              const std::string& n,
                                              const Hypothesis h) const {
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return i.getFunctionNameBasis(n) + "_" + ModellingHypothesis::toString(h);
    }
    return i.getFunctionNameBasis(n);
  }  // end of getSymbolName

  void SymbolsGenerator::generateGeneralSymbols(std::ostream& out,
                                                const BehaviourInterfaceBase& i,
                                                const BehaviourDescription& bd,
                                                const FileDescription& fd,
                                                const std::set<Hypothesis>& mhs,
                                                const std::string& name) const {
    this->writeFileDescriptionSymbols(out, i, fd, name);
    this->writeValidatorSymbol(out, i, bd, name);
    this->writeBuildIdentifierSymbol(out, i, bd, name);
    this->writeEntryPointSymbol(out, i, name);
    this->writeTFELVersionSymbol(out, i, name);
    this->writeUnitSystemSymbol(out, i, name, bd);
    this->writeMaterialSymbol(out, i, bd, name);
    this->writeMaterialKnowledgeTypeSymbol(out, i, name);
    this->writeInterfaceNameSymbols(out, i, bd, fd, name);
    this->writeSourceFileSymbols(out, i, bd, fd, name);
    this->writeSupportedModellingHypothesis(out, i, bd, mhs, name);
    this->writeMainVariablesSymbols(out, i, bd, name);
    this->writeTangentOperatorSymbols(out, i, bd, name);
    this->writeBehaviourTypeSymbols(out, i, bd, name);
    this->writeBehaviourKinematicSymbols(out, i, bd, name);
    this->writeSymmetryTypeSymbols(out, i, bd, name);
    this->writeElasticSymmetryTypeSymbols(out, i, bd, name);
    this->writeSpecificSymbols(out, i, bd, fd, name);
    this->writeTemperatureRemovedFromExternalStateVariablesSymbol(out, i, bd,
                                                                  name);
  }

  void SymbolsGenerator::writeFileDescriptionSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const FileDescription& fd,
      const std::string& n) const {
    mfront::writeFileDescriptionSymbols(out, i.getFunctionNameBasis(n), fd);
  }  // end of writeFileDescriptionSymbols

  void SymbolsGenerator::writeValidatorSymbol(std::ostream& out,
                                              const BehaviourInterfaceBase& i,
                                              const BehaviourDescription& d,
                                              const std::string& n) const {
    mfront::writeValidatorSymbol(out, i.getFunctionNameBasis(n), d);
  }  // end of writeValidatorSymbol

  void SymbolsGenerator::writeBuildIdentifierSymbol(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& d,
      const std::string& n) const {
    mfront::writeBuildIdentifierSymbol(out, i.getFunctionNameBasis(n), d);
  }  // end of writeBuildIdentifierSymbol

  void SymbolsGenerator::writeEntryPointSymbol(std::ostream& out,
                                               const BehaviourInterfaceBase& i,
                                               const std::string& n) const {
    mfront::writeEntryPointSymbol(out, i.getFunctionNameBasis(n));
  }  // end of writeEntryPointSymbol

  void SymbolsGenerator::writeTFELVersionSymbol(std::ostream& out,
                                                const BehaviourInterfaceBase& i,
                                                const std::string& n) const {
    mfront::writeTFELVersionSymbol(out, i.getFunctionNameBasis(n));
  }  // end of writeTFELVersionSymbol

  void SymbolsGenerator::writeUnitSystemSymbol(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const std::string& n,
      const BehaviourDescription& bd) const {
    mfront::writeUnitSystemSymbol(out, i.getFunctionNameBasis(n), bd);
  }  // end of writeUnitSystemSymbol

  void SymbolsGenerator::writeMaterialSymbol(std::ostream& out,
                                             const BehaviourInterfaceBase& i,
                                             const BehaviourDescription& mb,
                                             const std::string& n) const {
    mfront::writeMaterialSymbol(out, i.getFunctionNameBasis(n),
                                mb.getMaterialName());
  }  // end of writeEntryPointSymbol

  void SymbolsGenerator::writeMainVariablesSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
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
      const auto dvtype = dv.getVariableTypeIdentifier();
      const auto thname = th.getExternalName();
      const auto thtype = th.getVariableTypeIdentifier();
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
    exportUnsignedShortSymbol(out, fn + "_nMainVariables", nmvs);
    exportUnsignedShortSymbol(out, fn + "_nGradients", nmvs);
    this->writeArrayOfIntsSymbol(out, fn + "_GradientsTypes", dvtypes);
    this->writeArrayOfStringsSymbol(out, fn + "_Gradients", dvnames);
    exportUnsignedShortSymbol(out, fn + "_nThermodynamicForces", nmvs);
    this->writeArrayOfIntsSymbol(out, fn + "_ThermodynamicForcesTypes",
                                 thtypes);
    this->writeArrayOfStringsSymbol(out, fn + "_ThermodynamicForces", thnames);
  }  // end of writeMainVariablesSymbols

  void SymbolsGenerator::writeTangentOperatorSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    auto write_impl = [this, &out, &i,
                       &name](const std::vector<std::string>& bns) {
      const auto fn = i.getFunctionNameBasis(name);
      exportUnsignedShortSymbol(out, fn + "_nTangentOperatorBlocks",
                                bns.size());
      this->writeArrayOfStringsSymbol(out, fn + "_TangentOperatorBlocks", bns);
    };
    auto default_impl = [&bd, &write_impl] {
      std::vector<std::string> bns;
      for (const auto& b : bd.getTangentOperatorBlocks()) {
        const auto& f = b.first;   // function
        const auto& v = b.second;  // variable
        if ((v.arraySize == 1u) && (f.arraySize == 1u)) {
          bns.push_back(f.getExternalName());
          bns.push_back(v.getExternalName());
        } else if (v.arraySize == 1u) {
          for (unsigned short idx = 0; idx != f.arraySize; ++idx) {
            bns.push_back(f.getExternalName() + '[' + std::to_string(idx) +
                          ']');
            bns.push_back(v.getExternalName());
          }
        } else if (f.arraySize == 1u) {
          for (unsigned short idx = 0; idx != v.arraySize; ++idx) {
            bns.push_back(f.getExternalName());
            bns.push_back(v.getExternalName() + '[' + std::to_string(idx) +
                          ']');
          }
        } else {
          for (unsigned short idx = 0; idx != f.arraySize; ++idx) {
            for (unsigned short jdx = 0; jdx != v.arraySize; ++jdx) {
              bns.push_back(f.getExternalName() + '[' + std::to_string(idx) +
                            ']');
              bns.push_back(v.getExternalName() + '[' + std::to_string(jdx) +
                            ']');
            }
          }
        }
      }
      write_impl(bns);
    };
    auto empty_impl = [this, &i, &name, &out] {
      // strain measure is handled by the interface
      // we don't write any tangent operator here
      // as the consistent tangent operator
      // in finite strain is very dependant of the
      // interface is generally not related
      // the derivative of a flux with respect to
      // a gradient
      const auto fn = i.getFunctionNameBasis(name);
      exportUnsignedShortSymbol(out, fn + "_nTangentOperatorBlocks", 0u);
      this->writeArrayOfStringsSymbol(out, fn + "_TangentOperatorBlocks", {});
    };
    if (bd.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if ((bd.isStrainMeasureDefined()) &&
          (bd.getStrainMeasure() != BehaviourDescription::LINEARISED)) {
        if (this->handleStrainMeasure()) {
          empty_impl();
        } else {
          default_impl();
        }
      } else {
        default_impl();
      }
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      empty_impl();
    } else {
      default_impl();
    }
  }  // end of writeTangentOperatorSymbols

  void SymbolsGenerator::writeMaterialKnowledgeTypeSymbol(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const std::string& n) const {
    mfront::writeMaterialKnowledgeTypeSymbol(out, i.getFunctionNameBasis(n),
                                             MaterialKnowledgeType::BEHAVIOUR);
  }  // end of writeEntryPointSymbol

  void SymbolsGenerator::generateSymbols(std::ostream& out,
                                         const BehaviourInterfaceBase& i,
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
    this->writeInitializeFunctionsSymbols(out, i, bd, name, h);
    this->writePostProcessingsSymbols(out, i, bd, name, h);
    this->writeAdditionalSymbols(out, i, bd, fd, name, h);
    this->writeComputesInternalEnergySymbol(out, i, bd, name, h);
    this->writeComputesDissipatedEnergySymbol(out, i, bd, name, h);
  }

  void SymbolsGenerator::writeComputesInternalEnergySymbol(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name,
      const Hypothesis h) const {
    exportUnsignedShortSymbol(
        out, this->getSymbolName(i, name, h) + "_ComputesInternalEnergy",
        bd.hasCode(h, BehaviourData::ComputeInternalEnergy) ? 1u : 0u);
  }  // end of writeComputesInternalEnergySymbol

  void SymbolsGenerator::writeComputesDissipatedEnergySymbol(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name,
      const Hypothesis h) const {
    exportUnsignedShortSymbol(
        out, this->getSymbolName(i, name, h) + "_ComputesDissipatedEnergy",
        bd.hasCode(h, BehaviourData::ComputeDissipatedEnergy) ? 1u : 0u);
  }  // end of writeComputesDissipatedEnergySymbol

  void
  SymbolsGenerator::writeTemperatureRemovedFromExternalStateVariablesSymbol(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) +
                   "_TemperatureRemovedFromExternalStateVariables";
    exportUnsignedShortSymbol(
        out, s, shallRemoveTemperatureFromExternalStateVariables(bd) ? 1u : 0u);
  }  // end of writeTemperatureRemovedFromExternalStateVariablesSymbol

  void SymbolsGenerator::writeSpecificSymbols(std::ostream&,
                                              const BehaviourInterfaceBase&,
                                              const BehaviourDescription&,
                                              const FileDescription&,
                                              const std::string&) const {
  }  // end of writeSpecificSymbols

  void SymbolsGenerator::writeSupportedModellingHypothesis(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription&,
      const std::set<Hypothesis>& mhs,
      const std::string& name) const {
    exportUnsignedShortSymbol(
        out, i.getFunctionNameBasis(name) + "_nModellingHypotheses",
        mhs.size());
    const auto hypotheses = [&mhs] {
      auto hs = std::vector<std::string>{};
      std::for_each(mhs.begin(), mhs.end(), [&hs](const auto h) {
        hs.push_back(ModellingHypothesis::toString(h));
      });
      return hs;
    }();
    this->writeArrayOfStringsSymbol(
        out, i.getFunctionNameBasis(name) + "_ModellingHypotheses", hypotheses);
  }  // end of writeSupportedModellingHypothesis

  void SymbolsGenerator::writeMaterialPropertiesSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto mprops = i.buildMaterialPropertiesList(mb, h);
    for (const auto& mp : mprops.first) {
      if (SupportedTypes::getTypeFlag(mp.type) != SupportedTypes::SCALAR) {
        tfel::raise(
            "SymbolsGenerator::writeMaterialPropertiesSymbols: "
            "internal error: the material properties shall all be scalars");
      }
    }
    auto empty_case = [this, &out, &i, &name, h] {
      exportUnsignedShortSymbol(
          out, this->getSymbolName(i, name, h) + "_nMaterialProperties", 0u);
      this->writeArrayOfStringsSymbol(
          out, this->getSymbolName(i, name, h) + "_MaterialProperties", {});
    };
    if (mprops.first.empty()) {
      empty_case();
      return;
    }
    const auto& last = mprops.first.back();
    SupportedTypes::TypeSize s;
    s = last.offset;
    s += SupportedTypes::getTypeSize(last.type, last.arraySize);
    s -= mprops.second;
    // index of the first element which is not imposed by the material
    // properties
    auto ib = std::vector<BehaviourMaterialProperty>::size_type{};
    bool found = false;
    for (decltype(mprops.first.size()) idx = 0;
         (idx != mprops.first.size()) && (!found); ++idx) {
      if (mprops.first[idx].offset == mprops.second) {
        ib = idx;
        found = true;
      }
    }
    if (!found) {
      empty_case();
      return;
    }
    const auto mpnames = [&mprops, &ib] {
      auto mps = std::vector<std::string>{};
      for (auto idx = ib; idx != mprops.first.size(); ++idx) {
        const auto& m = mprops.first[idx];
        if (m.arraySize == 1u) {
          mps.push_back(m.getExternalName());
        } else {
          for (unsigned short j = 0; j != m.arraySize; ++j) {
            mps.push_back(m.getExternalName() + '[' + std::to_string(j) + ']');
          }
        }
      }
      return mps;
    }();
    exportUnsignedShortSymbol(
        out, this->getSymbolName(i, name, h) + "_nMaterialProperties",
        static_cast<unsigned short>(s.getValueForDimension(1)));
    this->writeArrayOfStringsSymbol(
        out, this->getSymbolName(i, name, h) + "_MaterialProperties", mpnames);
  }  // end of writeMaterialPropertiesSymbol

  void SymbolsGenerator::writeVariablesTypesSymbol(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const std::string& name,
      const Hypothesis h,
      const VariableDescriptionContainer& variables,
      const std::string& variables_identifier) const {
    if (!variables.empty()) {
      const auto size = [&variables] {
        auto s = std::size_t{};
        for (const auto& p : variables) {
          s += p.arraySize;
        }
        return s;
      }();
      out << "MFRONT_EXPORT_ARRAY_OF_SYMBOLS(int, "
          << this->getSymbolName(i, name, h) << "_" << variables_identifier
          << "Types, " << size << ", MFRONT_EXPORT_ARRAY_ARGUMENTS(";
      for (auto p = variables.begin(); p != variables.end();) {
        const auto t = p->getVariableTypeIdentifier();
        for (unsigned short is = 0; is != p->arraySize;) {
          out << t;
          if (++is != p->arraySize) {
            out << ",";
          }
        }
        if (++p != variables.end()) {
          out << ",";
        }
      }
      out << "));\n\n";
    } else {
      out << "MFRONT_EXPORT_SYMBOL(const int *, "
          << this->getSymbolName(i, name, h) << "_" << variables_identifier
          << "Types, nullptr);\n\n";
    }
  }  // end of writeVariablesTypesSymbol

  void SymbolsGenerator::writeStateVariablesSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const unsigned short nStateVariables = static_cast<unsigned short>(
        persistentVarsHolder.getNumberOfVariables());
    out << "MFRONT_EXPORT_SYMBOL(unsigned short, "
        << this->getSymbolName(i, name, h) << "_nInternalStateVariables, "
        << nStateVariables << "u);\n";
    std::vector<std::string> stateVariablesNames;
    mb.getExternalNames(stateVariablesNames, h, persistentVarsHolder);
    this->writeExternalNames(out, i, name, h, stateVariablesNames,
                             "InternalStateVariables");
    this->writeVariablesTypesSymbol(out, i, name, h, persistentVarsHolder,
                                    "InternalStateVariables");
  }  // end of writeStateVariablesSymbols

  void SymbolsGenerator::writeExternalStateVariablesSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    auto esvs = d.getExternalStateVariables();
    if (shallRemoveTemperatureFromExternalStateVariables(mb)) {
      // removing the temperature
      esvs.erase(esvs.begin());
    }
    out << "MFRONT_EXPORT_SYMBOL(unsigned short, "
        << this->getSymbolName(i, name, h) << "_nExternalStateVariables, "
        << esvs.getNumberOfVariables() << "u);\n";
    this->writeExternalNames(out, i, name, h, mb.getExternalNames(h, esvs),
                             "ExternalStateVariables");
    this->writeVariablesTypesSymbol(out, i, name, h, esvs,
                                    "ExternalStateVariables");
  }  // end of writeExternalStateVariablesSymbols

  void SymbolsGenerator::writeInitializeFunctionsSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const std::string&,
      const Hypothesis) const {}  // end of writeInitializeFunctionsSymbols

  void SymbolsGenerator::writePostProcessingsSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const std::string&,
      const Hypothesis) const {}  // end of writePostProcessingsSymbols

  void SymbolsGenerator::writeParametersSymbols(std::ostream& out,
                                                const BehaviourInterfaceBase& i,
                                                const BehaviourDescription& mb,
                                                const std::string& name,
                                                const Hypothesis h) const {
    if (!areParametersTreatedAsStaticVariables(mb)) {
      mfront::writeParametersDeclarationSymbols(
          out, this->getSymbolName(i, name, h),
          mb.getBehaviourData(h).getParameters());
    } else {
      mfront::writeParametersDeclarationSymbols(
          out, this->getSymbolName(i, name, h), {});
    }
  }  // end of writeParametersSymbols

  void SymbolsGenerator::writeParameterDefaultValueSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    if (areParametersTreatedAsStaticVariables(mb)) {
      return;
    }
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "SymbolsGenerator::writeParameterDefaultValueSymbols: " + m);
    };
    const auto n = this->getSymbolName(i, name, h);
    for (const auto& p : mb.getBehaviourData(h).getParameters()) {
      if (p.type == "int") {
        throw_if(p.arraySize != 1u,
                 "unsupported parameters array of type '" + p.type + "'");
        exportSymbol(os, "int",
                     n + "_" + p.getExternalName() + "_ParameterDefaultValue",
                     mb.getIntegerParameterDefaultValue(h, p.name));
      } else if (p.type == "ushort") {
        throw_if(p.arraySize != 1u,
                 "unsupported parameters array of type '" + p.type + "'");
        exportSymbol(os, "unsigned short",
                     n + "_" + p.getExternalName() + "_ParameterDefaultValue",
                     mb.getUnsignedShortParameterDefaultValue(h, p.name));
      } else {
        const auto f = SupportedTypes::getTypeFlag(p.type);
        throw_if(f != SupportedTypes::SCALAR,
                 "unsupported paramaeter type '" + p.type + "'");
        const auto prec = os.precision();
        os.precision(14);
        if (p.arraySize == 1u) {
          exportSymbol(os, "double",
                       n + "_" + p.getExternalName() + "_ParameterDefaultValue",
                       mb.getFloattingPointParameterDefaultValue(h, p.name));

        } else {
          for (unsigned short is = 0; is != p.arraySize; ++is) {
            exportSymbol(
                os, "double",
                n + "_" + p.getExternalName() + "_mfront_index_" +
                    std::to_string(static_cast<int>(is)) +
                    "_ParameterDefaultValue",
                mb.getFloattingPointParameterDefaultValue(h, p.name, is));
          }
        }
        os.precision(prec);
      }
    }
  }  // end of writeParameterDefaultValueSymbols

  void SymbolsGenerator::writeBoundsSymbols(std::ostream& os,
                                            const BehaviourInterfaceBase& i,
                                            const BehaviourDescription& mb,
                                            const std::string& name,
                                            const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto n = this->getSymbolName(i, name, h);
    mfront::writeBoundsSymbols(os, n, d.getMaterialProperties());
    mfront::writeBoundsSymbols(os, n, d.getPersistentVariables());
    mfront::writeBoundsSymbols(os, n, d.getExternalStateVariables());
    if (!areParametersTreatedAsStaticVariables(mb)) {
      mfront::writeBoundsSymbols(os, n, d.getParameters());
    }
  }  // end of writeBoundsSymbols

  void SymbolsGenerator::writePhysicalBoundsSymbols(
      std::ostream& os,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto n = this->getSymbolName(i, name, h);
    mfront::writePhysicalBoundsSymbols(os, n, d.getMaterialProperties());
    mfront::writePhysicalBoundsSymbols(os, n, d.getPersistentVariables());
    mfront::writePhysicalBoundsSymbols(os, n, d.getExternalStateVariables());
    if (!areParametersTreatedAsStaticVariables(mb)) {
      mfront::writePhysicalBoundsSymbols(os, n, d.getParameters());
    }
  }  // end of writePhysicalBoundsSymbols

  void SymbolsGenerator::writeRequirementsSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto b1 =
        mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false);
    exportUnsignedShortSymbol(
        out, this->getSymbolName(i, name, h) + "_requiresStiffnessTensor",
        b1 ? 1 : 0);
    const auto b2 = mb.getAttribute(
        BehaviourDescription::requiresThermalExpansionCoefficientTensor, false);
    exportUnsignedShortSymbol(out,
                              this->getSymbolName(i, name, h) +
                                  "_requiresThermalExpansionCoefficientTensor",
                              b2 ? 1 : 0);
  }  // end of writeRequirementsSymbols

  void SymbolsGenerator::writeIsUsableInPurelyImplicitResolutionSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto b = d.isUsableInPurelyImplicitResolution();
    const auto s =
        this->getSymbolName(i, name, h) + "_UsableInPurelyImplicitResolution";
    exportUnsignedShortSymbol(out, s, b ? 1 : 0);
  }  // end of writeIsUsableInPurelyImplicitResolution

  void SymbolsGenerator::writeSymmetryTypeSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    const auto st = [&mb]() -> unsigned short {
      if (mb.getSymmetryType() == mfront::ISOTROPIC) {
        return 0u;
      } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
        return 1u;
      }
      tfel::raise(
          "SymbolsGenerator::writeSymmetryTypeSymbols: "
          "unsupported behaviour type.\n"
          "only isotropic or orthotropic behaviours "
          "are supported at this time.");
    }();
    exportUnsignedShortSymbol(
        out, i.getFunctionNameBasis(name) + "_SymmetryType", st);
  }  // end of writeSymmetryTypeSymbols

  void SymbolsGenerator::writeElasticSymmetryTypeSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    const auto est = [&mb]() -> unsigned short {
      if (mb.getElasticSymmetryType() == mfront::ISOTROPIC) {
        return 0u;
      } else if (mb.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        return 1u;
      }
      tfel::raise(
          "SymbolsGenerator::writeElasticSymmetryTypeSymbols: "
          "unsupported behaviour type.\n"
          "only isotropic or orthotropic behaviours are "
          "supported at this time.");
    }();
    exportUnsignedShortSymbol(
        out, i.getFunctionNameBasis(name) + "_ElasticSymmetryType", est);
  }  // end of writeElasticSymmetryTypeSymbols

  void SymbolsGenerator::writeSourceFileSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription&,
      const mfront::FileDescription& fd,
      const std::string& name) const {
    using namespace tfel::system;
    using namespace tfel::utilities;
    exportStringSymbol(out, i.getFunctionNameBasis(name) + "_src",
                       tokenize(fd.fileName, dirSeparator()).back());
  }  // end of writeSourceFileSymbols

  void SymbolsGenerator::writeInterfaceNameSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription&,
      const mfront::FileDescription&,
      const std::string& name) const {
    writeInterfaceSymbol(out, i.getFunctionNameBasis(name),
                         i.getInterfaceName() + i.getInterfaceVersion());
  }  // end of writeInterfaceNameSymbols

  void SymbolsGenerator::writeExternalNames(std::ostream& os,
                                            const BehaviourInterfaceBase& i,
                                            const std::string& name,
                                            const Hypothesis& h,
                                            const std::vector<std::string>& v,
                                            const std::string& t) const {
    this->writeArrayOfStringsSymbol(
        os, this->getSymbolName(i, name, h) + "_" + t, v);
  }  // end of writeExternalNames

  void SymbolsGenerator::writeArrayOfStringsSymbol(
      std::ostream& os,
      const std::string& s,
      const std::vector<std::string>& v) const {
    mfront::exportArrayOfStringsSymbol(os, s, v);
  }  // end of writeArrayOfStringsSymbol

  void SymbolsGenerator::writeArrayOfIntsSymbol(
      std::ostream& os, const std::string& s, const std::vector<int>& v) const {
    mfront::exportArrayOfIntegersSymbol(os, s, v);
  }  // end of writeArrayOfIntsSymbol

  SymbolsGenerator::~SymbolsGenerator() = default;

}  // end of namespace mfront
