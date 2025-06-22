/*!
 * \file   LicosInputFileGenerators.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02/10/2019
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
#include "TFEL/System/ExternalBehaviourDescription.hxx"
#include "MFMTestGenerator/Log.hxx"
#include "MFMTestGenerator/Evolution.hxx"
#include "MFMTestGenerator/BehaviourData.hxx"
#include "MFMTestGenerator/UniaxialTensileTest.hxx"
#include "MFMTestGenerator/LicosInputFileGenerators.hxx"

namespace mfmtg {

  namespace licos {

    /*!
     * \brief write the material declaration
     * \tparam TestCase: type of the test
     * \param[in] os: output stream
     * \param[in] c: test case
     * \param[in] m: name of the material
     */
    static void writeMaterial(std::ostream& os,
                              const BehaviourData& c,
                              const std::string& m) {
      auto write_mp = [&os, &c](const Evolution& mp, const std::string& n) {
        if (!mp.template is<double>()) {
          tfel::raise(
              "writeMaterial: "
              "material properties must be constant");
        }
        os << "MaterialProperty<constant> '" << n << "' "
           << mp.template get<double>() << "\n";
      };
      auto write_mp_or_default = [&os, &c, &write_mp](const std::string& n) {
        const auto p = c.material_properties.find(n);
        if (p != c.material_properties.end()) {
          write_mp(p->second, n);
        } else {
          os << "MaterialProperty<constant> '" << n << "' 0\n";
        }
      };
      const auto b = tfel::system::ExternalBehaviourDescription(
          c.library, c.function, c.hypothesis);
      if (b.mfront_interface != "Castem") {
        tfel::raise(
            "mfmtg::licos::writeMaterial: "
            "unsupported interface for behaviour '" +
            c.function +
            "' "
            "in library '" +
            c.library + "'");
      }
      os << "Material '" << m << "'\n";
      write_mp_or_default("MassDensity");
      if (b.stype == 0) {
        os << "MechanicalBehaviour<Isotropic>";
      } else {
        os << "MechanicalBehaviour<Orthotropic>";
      }
      os << " 'UmatBehaviour' '" << c.library << "' '" << c.function << "'\n";
      for (const auto& mp : c.material_properties) {
        write_mp(mp.second, mp.first);
      }
      if (b.stype == 0) {
        write_mp_or_default("ThermalExpansion");
      } else {
        write_mp_or_default("ThermalExpansion1");
        write_mp_or_default("ThermalExpansion2");
        write_mp_or_default("ThermalExpansion3");
      }
      for (decltype(b.ivnames.size()) i = 0; i != b.ivnames.size(); ++i) {
        const auto& n = b.ivnames[i];
        os << "InternalStateVariable";
        if (b.ivtypes[i] == 0) {
          os << "<Scalar>";
        } else if (b.ivtypes[i] == 1) {
          os << "<Stensor>";
        } else {
          tfel::raise("Unsupported type for variable '" + n + "'");
        }
        os << " '" << n << "'\n";
      }
      os << "EndOfMechanicalBehaviour\n";
      for (const auto& ev : c.external_state_variables) {
        if (ev.second.template is<double>()) {
          os << "StateVariable '" << ev.first << "' "
             << ev.second.template get<double>() << "\n";
        }
      }
      os << "EndOfMaterial\n";
    }  // end of writeBehaviour

    /*!
     * \brief write the non constant and non uniform external state variables as
     * loadings.
     * \tparam TestCase: type of the test
     * \param[in] os: output stream
     * \param[in] c: test case
     * \param[in] m: name of the material
     */
    template <typename TestCase>
    static void writeExternalStateVariables(std::ostream&,
                                            const TestCase& c,
                                            const std::string&) {
      for (const auto& ev : c.external_state_variables) {
        if (!ev.second.template is<double>()) {
          tfel::raise(
              "mfmtg::licos::writeExternalStateVariables: "
              "externa state variables must be constant");
        }
      }
    }  // end of writeExternalStateVariables

    /*!
     * \brief write the times
     * \param[in] os: output stream
     * \param[in] times: list of times
     */
    static void writeTimes(std::ostream& os, const Times& t) {
      const auto& times = t.get<std::vector<double>>();
      os << "Times {";
      for (decltype(times.size()) i = 0; i != times.size();) {
        os << times[i];
        if (++i != times.size()) {
          os << ", ";
        }
      }
      os << "}\n";
    }  // end of writeTimes

    void generateUniaxialTensileTest(const AbstractTestCase& at) {
      auto raise = [](const std::string& msg) {
        tfel::raise("mfmtg::licos::generateUniaxialTensileTest: " + msg);
      };  // end of raise
      debug("mfmtg::licos::generateUniaxialTensileTest: begin");
      const auto& t = dynamic_cast<const UniaxialTensileTest&>(at);
      const auto b = tfel::system::ExternalBehaviourDescription(
          t.library, t.function, t.hypothesis);
      if (b.btype != 1) {
        raise("only small strain behahaviour are supported");
      }
      const auto& f = t.name + "-licos.ple";
      std::ofstream os(t.name + "-licos.ple");
      if (!os) {
        raise("can't open file '" + f + "'");
      }
      os.exceptions(std::ios::badbit | std::ios::failbit);
      os.precision(14);
      writeMaterial(os, t, "CUBE");
      writeExternalStateVariables(os, t, "CUBE");
      os << "Coupling 'LoopCoupling' 1\n"
         << "Model 'CastemMechanicalModel'\n"
         << "EndOfCoupling\n";
      writeTimes(os, t.times);
      os << "LoadingEvolution<Table>\n"
         << "Time Uz\n";
      if (!t.imposed_strain.is<std::map<double, double>>()) {
        tfel::raise(
            "mfmtg::licos::generateUniaxialTensileTest: "
            "unsupported description of the imposed strain");
      }
      for (const auto& s : t.imposed_strain.get<std::map<double, double>>()) {
        os << s.first << " " << s.second << "\n";
      }
      os << "EndOfLoadingEvolution\n";
      os.close();
      debug("mfmtg::licos::generateUniaxialTensileTest: end");
    }  // end of generateUniaxialTensileTest

  }  // end of namespace licos

}  // end of namespace mfmtg
