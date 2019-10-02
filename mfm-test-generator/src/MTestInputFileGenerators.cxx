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
#include "MFMTestGenerator/Log.hxx"
#include "MFMTestGenerator/MTestInputFileGenerators.hxx"
#include "MFMTestGenerator/UniaxialTensileTest.hxx"

namespace mfmtg {

  namespace mtest {

    template <typename TestCase>
    static void writeBehaviour(std::ostream& os, const TestCase& c) {
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

    static void writeEvolution(std::ostream& os, const Evolution& e) {
      if (e.is<double>()) {
        os << e.get<double>();
      } else if (!e.is<std::map<double, double>>()) {
        tfel::raise("getEvolutionType: unsupported evolution type");
      }
      os << "{";
      const auto& ev = e.get<std::map<double, double>>();
      auto p = ev.begin();
      const auto pe = ev.end();
      while (p != pe) {
        os << p->first << " : " << p->second;
        if (++p != pe) {
          os << ", ";
        }
      }
      os << "}\n";
    }  // end of writeEvolution

    static std::string getEvolutionType(const Evolution& e) {
      if (e.is<double>()) {
        return "constant";
      } else if (!e.is<std::map<double, double>>()) {
        tfel::raise("getEvolutionType: unsupported evolution type");
      }
      return "evolution";
    }  // end of getEvolutionType

    void generateUniaxialTensileTest(const AbstractTestCase& at){
      auto raise = [](const std::string& msg) {
        tfel::raise("mfmtg::mtest::generateUniaxialTensileTest: " + msg);
      };  // end of raise
      debug("mfmtg::mtest::generateUniaxialTensileTest: begin\n");
      const auto& t = dynamic_cast<const UniaxialTensileTest&>(at);
      const auto& f = t.name + ".mtest";
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
        os << "@MaterialProperty<" << getEvolutionType(mp.second) << "> '"
           << mp.first << "' ";
        writeEvolution(os, mp.second);
        os << ";\n";
      }
      for (const auto& ev : t.external_state_variables) {
        os << "@ExternalStateVariable<" << getEvolutionType(ev.second) << "> '"
           << ev.first << "' ";
        writeEvolution(os, ev.second);
        os << ";\n";
      }
      os << "@ImposedStrain 'EXX' {";
      auto p = t.imposed_strain.begin();
      const auto pe = t.imposed_strain.end();
      while (p != pe) {
        os << p->first << " : " << p->second;
        if (++p != pe) {
          os << ", ";
        }
      }
      os << "};\n";
      os.close();
      debug("mfmtg::mtest::generateUniaxialTensileTest: end\n");
    }  // end of generateUniaxialTensileTest

  }  // end of namespace mtest

}  // end of namespace mfmtg
