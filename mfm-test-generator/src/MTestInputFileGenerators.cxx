/*!
 * \file   MTestInputFileGenerators.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   25/09/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
#include "MFMTestGenerator/MTestInputFileGenerators.hxx"

namespace mfmtg {

  namespace mtest {

    static void writeBehaviour(std::ostream& os, const BehaviourData& c) {
      os << "@Behaviour '" << c.library << "' '" << c.function << "' ;\n";
    }  // end of writeBehaviour

    static void writeTimes(std::ostream& os, const std::vector<double>& times) {
      os << "@Times {";
      for (decltype(times.size()) i = 0; i != times.size();) {
        os << times[i];
        if (++i != times.size()) {
          os << ", ";
        }
      }
      os << "};\n";
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

    static void writeEvolution(std::ostream& os, const Evolution& e) {
      if (e.is<double>()) {
        os << e.get<double>();
        return;
      } else if (!e.is<std::map<double, double>>()) {
        tfel::raise("getEvolutionType: unsupported evolution type");
      }
      writeEvolution(os, e.get<std::map<double, double>>());
    }  // end of writeEvolution

    static std::string getEvolutionType(const Evolution& e) {
      if ((!e.is<double>()) && (!e.is<std::map<double, double>>())) {
        tfel::raise("getEvolutionType: unsupported evolution type");
      }
      return "evolution";
    }  // end of getEvolutionType

    void generateUniaxialTensileTest(const AbstractTestCase& at){
      using ::tfel::material::MechanicalBehaviourBase;
      using ::tfel::material::ModellingHypothesis;
      using ::mtest::Behaviour;
      auto raise = [](const std::string& msg) {
        tfel::raise("mfmtg::mtest::generateUniaxialTensileTest: " + msg);
      };  // end of raise
      debug("mfmtg::mtest::generateUniaxialTensileTest: begin\n");
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
      auto write = [&os](const std::string& fmt, const std::string& s) {
        os << tfel::utilities::replace_all(fmt, "{}", s) << '\n';
      };
      auto write_if = [&write](const std::string& fmt, const std::string& s) {
        if (!s.empty()) {
          write(fmt, s);
        }
      };
      write_if("@Author '{}';", t.author);
      write_if("@Date   '{}';", t.date);
      write_if("@Description {\n{}\n};", t.description);
      write("@ModellingHypothesis '{}';",t.hypothesis);
      writeBehaviour(os, t);
      writeTimes(os, t.times);
      for (const auto& mp : t.material_properties) {
        os << "@MaterialProperty<" << getMaterialPropertyType(mp.second) << "> '"
           << mp.first << "' ";
        writeMaterialProperty(os, mp.second);
        os << ";\n";
      }
      for (const auto& ev : t.external_state_variables) {
        os << "@ExternalStateVariable<" << getEvolutionType(ev.second) << "> '"
           << ev.first << "' ";
        writeEvolution(os, ev.second);
        os << ";\n";
      }
      os << "\n"
         << "@Evolution 'MFMTGImposedStrain' ";
      writeEvolution(os, t.imposed_strain);
      os << ";\n\n";
      if (bt == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR){
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
      debug("mfmtg::mtest::generateUniaxialTensileTest: end\n");
    }  // end of generateUniaxialTensileTest

  }  // end of namespace mtest

}  // end of namespace mfmtg
