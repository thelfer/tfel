/*!
 * \file   MTestInputFileGenerators.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MTest/Behaviour.hxx"
#include "MFMTestGenerator/Log.hxx"
#include "MFMTestGenerator/Evolution.hxx"
#include "MFMTestGenerator/BehaviourData.hxx"
#include "MFMTestGenerator/UniaxialTensileTest.hxx"
#include "MFMTestGenerator/ClosedPipeTest.hxx"
#include "MFMTestGenerator/MTestInputFileGenerators.hxx"

namespace mfmtg {

  namespace mtest {

    static void write(std::ostream& os,
                      const std::string& fmt,
                      const std::string& s) {
      os << tfel::utilities::replace_all(fmt, "{}", s) << '\n';
    }  // end of write

    static void write_if(std::ostream& os,
                         const std::string& fmt,
                         const std::string& s) {
      if (!s.empty()) {
        write(os, fmt, s);
      }
    }  // end of write_if

    template <typename TestCaseType>
    static void writeMetaData(std::ostream& os, const TestCaseType& t) {
      write_if(os, "@Author '{}';", t.author);
      write_if(os, "@Date   '{}';", t.date);
      write_if(os, "@Description {\n{}\n};", t.description);
    }  // end of writeMetadata

    static void writeBehaviour(std::ostream& os, const BehaviourData& c) {
      os << "@Behaviour '" << c.library << "' '" << c.function << "' ;\n";
    }  // end of writeBehaviour

    static void writeTimes(std::ostream& os, const Times& t) {
      if (t.is<std::vector<double>>()) {
        const auto& times = t.get<std::vector<double>>();
        os << "@Times {";
        for (decltype(times.size()) i = 0; i != times.size();) {
          os << times[i];
          if (++i != times.size()) {
            os << ", ";
          }
        }
        os << "};\n";
        return;
      } else if (!t.is<TimesFromFile>()) {
        tfel::raise(
            "mfmtg::mtest::writeTimes: "
            "unsupported type for times");
      }
      const auto times = t.get<TimesFromFile>();
      os << "@Times<data> '" << times.file << "' using ";
      if (times.times.is<unsigned int>()) {
        os << times.times.get<unsigned int>();
      } else if (times.times.is<std::string>()) {
        os << times.times.get<std::string>();
      } else {
        tfel::raise(
            "mfmtg::mtest::writeTimes: "
            "unsupported times description");
      }
      os << ";\n";
    }  // end of writeTimes

    static void writeEvolution(std::ostream& os,
                               const std::map<double, double>& e) {
      os << "{";
      auto p = e.begin();
      const auto pe = e.end();
      while (p != pe) {
        os << p->first << " : " << p->second;
        if (++p != pe) {
          os << ", ";
        }
      }
      os << "}";
    }  // end of writeEvolution

    static void writeEvolution(std::ostream& os, const Evolution& e) {
      if (e.is<double>()) {
        os << e.get<double>();
        return;
      } else if (e.is<EvolutionFromFile>()) {
        auto write_values = [&os](const EvolutionFromFile::Values& v) {
          if (v.is<unsigned int>()) {
            os << v.get<unsigned int>();
          } else {
            os << "'" << v.get<std::string>() << "'";
          }
        };
        const auto& d = e.get<EvolutionFromFile>();
        os << "'" << d.file << "' using ";
        write_values(d.times);
        os << ":";
        write_values(d.values);
        return;
      } else if (!e.is<std::map<double, double>>()) {
        tfel::raise("getEvolutionType: unsupported evolution type");
      }
      writeEvolution(os, e.get<std::map<double, double>>());
    }  // end of writeEvolution

    static std::string getEvolutionType(const Evolution& e) {
      if (e.is<EvolutionFromFile>()) {
        return "data";
      } else if ((!e.is<double>()) && (!e.is<std::map<double, double>>())) {
        tfel::raise("getEvolutionType: unsupported evolution type");
      }
      return "evolution";
    }  // end of getEvolutionType

    static void writeMaterialProperty(std::ostream& os, const Evolution& e) {
      if (!e.is<double>()) {
        tfel::raise("getMaterialPropertyType: unsupported evolution type");
      }
      os << e.get<double>();
    }  // end of writeMaterialProperty

    static std::string getMaterialPropertyType(const Evolution& e) {
      if (!e.is<double>()) {
        tfel::raise("getMaterialPropertyType: unsupported evolution type");
      }
      return "constant";
    }  // end of getMaterialPropertyType

    static void writeMaterialProperties(std::ostream& os,
                                        const BehaviourData& c) {
      for (const auto& mp : c.material_properties) {
        os << "@MaterialProperty<" << getMaterialPropertyType(mp.second)
           << "> '" << mp.first << "' ";
        writeMaterialProperty(os, mp.second);
        os << ";\n";
      }
    }  // end of writeMaterialProperties

    static void writeExternalStateVariables(std::ostream& os,
                                            const BehaviourData& c) {
      for (const auto& ev : c.external_state_variables) {
        os << "@ExternalStateVariable<" << getEvolutionType(ev.second) << "> '"
           << ev.first << "' ";
        writeEvolution(os, ev.second);
        os << ";\n";
      }
    }  // end of writeExternalStateVariables

    void generateUniaxialTensileTest(const AbstractTestCase& at) {
      using ::mtest::Behaviour;
      using ::tfel::material::MechanicalBehaviourBase;
      using ::tfel::material::ModellingHypothesis;
      auto raise = [](const std::string& msg) {
        tfel::raise("mfmtg::mtest::generateUniaxialTensileTest: " + msg);
      };  // end of raise
      debug("mfmtg::mtest::generateUniaxialTensileTest: begin");
      const auto& t = dynamic_cast<const UniaxialTensileTest&>(at);
      const auto& f = t.name + ".mtest";
      // loading the behaviour
      const auto b = Behaviour::getBehaviour(
          "", t.library, t.function, Behaviour::Parameters{},
          ModellingHypothesis::fromString(t.hypothesis));
      const auto bt = b->getBehaviourType();
      if ((bt != MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (bt != MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR)) {
        raise("Invalid behaviour type");
      }
      std::ofstream os(t.name + ".mtest");
      if (!os) {
        raise("can't open file '" + f + "'");
      }
      os.exceptions(std::ios::badbit | std::ios::failbit);
      os.precision(14);
      writeMetaData(os, t);
      write(os, "@ModellingHypothesis '{}';", t.hypothesis);
      writeBehaviour(os, t);
      writeTimes(os, t.times);
      writeMaterialProperties(os, t);
      writeExternalStateVariables(os, t);
      os << "\n"
         << "@Evolution<" << getEvolutionType(t.imposed_strain)
         << "> 'MFMTGImposedStrain' ";
      writeEvolution(os, t.imposed_strain);
      os << ";\n\n";
      if (bt == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) {
        os << "@ImposedStrain<function> 'EXX' 'MFMTGImposedStrain';\n";
      } else {
        os << "@ImposedDeformationGradient<function> "
           << "'FXX' '1+MFMTGImposedStrain';\n"
           << "@ImposedDeformationGradient 'FXY' 0;\n"
           << "@ImposedDeformationGradient 'FYX' 0;\n"
           << "@ImposedDeformationGradient 'FXZ' 0;\n"
           << "@ImposedDeformationGradient 'FZX' 0;\n"
           << "@ImposedDeformationGradient 'FYZ' 0;\n"
           << "@ImposedDeformationGradient 'FZY' 0;\n";
      }
      os.close();
      debug("mfmtg::mtest::generateUniaxialTensileTest: end");
    }  // end of generateUniaxialTensileTest

    void generateClosedPipeTest(const AbstractTestCase& at) {
      using ::mtest::Behaviour;
      using ::tfel::material::MechanicalBehaviourBase;
      using ::tfel::material::ModellingHypothesis;
      auto raise = [](const std::string& msg) {
        tfel::raise("mfmtg::mtest::generateClosedPipeTest: " + msg);
      };  // end of raise
      debug("mfmtg::mtest::generateClosedPipeTest: begin");
      const auto& t = dynamic_cast<const ClosedPipeTest&>(at);
      const auto& f = t.name + ".mtest";
      // loading the behaviour
      const auto b = Behaviour::getBehaviour(
          "", t.library, t.function, Behaviour::Parameters{},
          ModellingHypothesis::fromString(t.hypothesis));
      const auto bt = b->getBehaviourType();
      if ((bt != MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (bt != MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR)) {
        raise("Invalid behaviour type");
      }
      std::ofstream os(t.name + ".ptest");
      if (!os) {
        raise("can't open file '" + f + "'");
      }
      os.exceptions(std::ios::badbit | std::ios::failbit);
      os.precision(14);
      writeMetaData(os, t);
      os << "@InnerRadius " << t.inner_radius << ";\n"
         << "@OuterRadius " << t.outer_radius << ";\n"
         << "@NumberOfElements " << t.number_of_elements << ";\n";
      if (t.element_type == ClosedPipeTest::LINEAR) {
        os << "@ElementType 'Linear';\n";
      } else if (t.element_type == ClosedPipeTest::QUADRATIC) {
        os << "@ElementType 'Quadratic'\n";
      } else {
        tfel::raise(
            "mfmtg::mtest::generateClosedPipeTest: "
            "unsupported element type");
      }
      if (bt == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) {
        os << "@PerformSmallStrainAnalysis true;\n";
      }
      writeBehaviour(os, t);
      writeMaterialProperties(os, t);
      writeExternalStateVariables(os, t);
      os << "\n"
         << "@InnerPressureEvolution<" << getEvolutionType(t.inner_pressure)
         << "> ";
      writeEvolution(os, t.inner_pressure);
      os << ";\n"
         << "@OuterPressureEvolution<" << getEvolutionType(t.outer_pressure)
         << "> ";
      writeEvolution(os, t.outer_pressure);
      os << ";\n\n";
      writeTimes(os, t.times);
      os.close();
      debug("mfmtg::mtest::generateClosedPipeTest: end");
    }  // end of generateClosedPipeTest

  }  // end of namespace mtest

}  // end of namespace mfmtg
