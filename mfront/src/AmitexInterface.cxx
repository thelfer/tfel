/*!
 * \file   mfront/src/AmitexInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AmitexInterface.hxx"

namespace mfront {

  std::string AmitexInterface::getName() { return "amitex"; }  // end of getName

  AmitexInterface::AmitexInterface() {
  }  // end of AmitexInterface::AmitexInterface

  void AmitexInterface::generateInputFileExampleForHypothesis(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "AmitexInterface::generateInputFileExampleForHypothesis: " + m);
    };
    if (h != ModellingHypothesis::TRIDIMENSIONAL) {
      return;
    }
    const auto& d = bd.getBehaviourData(h);
    auto persistentVarsHolder = d.getPersistentVariables();
    // const auto& externalStateVarsHolder = d.getExternalStateVariables();
    // list of material properties (must use the real hypothesis)
    const auto mprops = this->buildMaterialPropertiesList(bd, h);
    auto tmp = std::string{};
    out << "<Material numM=\"I\" Lib=\"" << this->getLibraryName(bd) << "\" "
        << "Law=\"" << this->getUmatFunctionName(bd) << "\">\n";
    auto nprops = std::size_t{1};
    for (const auto& pm : mprops.first) {
      const auto flag = SupportedTypes::getTypeFlag(pm.type);
      throw_if(flag != SupportedTypes::SCALAR,
               "material properties shall be scalars");
      if (pm.arraySize == 1) {
        out << "<!-- material property " << pm.name << " -->\n"
            << "<Coeff Index = \"" << nprops << "\" "
            << "Type = \"Constant\" Value = \"...\" />\n";
        ++nprops;
      } else {
        for (unsigned short j = 0; j != pm.arraySize;) {
          out << "<!-- material property " << pm.name << "[" << j << "] -->\n"
              << "<Coeff Index = \"" << nprops << "\" "
              << "Type = \"Constant\" Value = \"...\" />\n";
          ++nprops;
        }
      }
    }
    auto nisvs = std::size_t{1};
    if (!persistentVarsHolder.empty()) {
      for (const auto& v : persistentVarsHolder) {
        const auto flag = SupportedTypes::getTypeFlag(v.type);
        if (v.arraySize == 1) {
          out << "<!-- internal state variable " << v.getExternalName()
              << " -->\n";
          if (flag == SupportedTypes::SCALAR) {
            out << "<IntVar Index = \"" << nisvs << "\" "
                << "Type = \"Constant\" Value = \"...\" />\n";
            ++nisvs;
          } else if (flag == SupportedTypes::STENSOR) {
            for (unsigned short i = 0; i != tfel::material::getStensorSize(h);
                 ++i) {
              out << "<IntVar Index = \"" << nisvs << "\" "
                  << "Type = \"Constant\" Value = \"...\" />\n";
              ++nisvs;
            }
          } else if (flag == SupportedTypes::TENSOR) {
            for (unsigned short i = 0; i != tfel::material::getStensorSize(h);
                 ++i) {
              out << "<IntVar Index = \"" << nisvs << "\" "
                  << "Type = \"Constant\" Value = \"...\" />\n";
              ++nisvs;
            }
          } else {
            tfel::raise(
                "AmitexInterface::generateInputFileExampleForHypothesis: "
                "unsupported state variable type");
          }
        } else {
          for (unsigned short j = 0; j != v.arraySize; ++j) {
            out << "<!-- internal state variable " << v.getExternalName() << "["
                << j << "] -->\n";
            if (flag == SupportedTypes::SCALAR) {
              out << "<IntVar Index = \"" << nisvs << "\" "
                  << "Type = \"Constant\" Value = \"...\" />\n";
              ++nisvs;
            } else if (flag == SupportedTypes::STENSOR) {
              for (unsigned short i = 0; i != tfel::material::getStensorSize(h);
                   ++i) {
                out << "<IntVar Index = \"" << nisvs << "\" "
                    << "Type = \"Constant\" Value = \"...\" />\n";
                ++nisvs;
              }
            } else if (flag == SupportedTypes::TENSOR) {
              for (unsigned short i = 0; i != tfel::material::getStensorSize(h);
                   ++i) {
                out << "<IntVar Index = \"" << nisvs << "\" "
                    << "Type = \"Constant\" Value = \"...\" />\n";
                ++nisvs;
              }
            } else {
              tfel::raise(
                  "AmitexInterface::generateInputFileExampleForHypothesis: "
                  "unsupported state variable type");
            }
          }
        }
      }
    }
    out << "</Material>\n\n";
  }  // end of AmitexInterface::generateInputFileExampleForHypothesis

  void AmitexInterface::generateInputFileExample(const BehaviourDescription& bd,
                                                 const FileDescription&) const {
    try {
      const auto name((!bd.getLibrary().empty())
                          ? bd.getLibrary() + bd.getClassName()
                          : bd.getClassName());
      const auto fileName("amitex/" + name + ".xml");
      // opening output file
      tfel::system::systemCall::mkdir("amitex");
      std::ofstream out;
      out.open(fileName);
      tfel::raise_if(!out,
                     "AmitexInterface::generateInputFileExample: "
                     "could not open file '" +
                         fileName + "'");
      // header
      out << "<!--\n"
          << "    Simple template for use in AMITEX_FFTP.\n"
          << "    \n"
          << "    You may copy-paste those lines in your material.xml file\n"
          << "    and adjust:\n\n"
          << "    - the material number I,\n"
          << "    - the path to the library (currently "
          << this->getLibraryName(bd) << "),\n"
          << "    - the coefficients and initial internal variables values\n"
          << "      which are currently replaced by three dots.\n"
          << "-->\n\n";
      // loop over hypothesis
      for (const auto& h : this->getModellingHypothesesToBeTreated(bd)) {
        this->generateInputFileExampleForHypothesis(out, bd, h);
      }
      out.close();
    } catch (std::exception& e) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << e.what() << std::endl;
      }
    } catch (...) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << "AmitexInterface::generateInputFileExample: "
                       << "unknown exception thrown" << std::endl;
      }
    }
  }  // end of generateInputFileExample

  AmitexInterface::~AmitexInterface() = default;

}  // end of namespace mfront
