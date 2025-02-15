/*!
 * \file  BehaviourQuery.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#if (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef small
#undef small
#endif /* small */
#include <cstdlib>
#endif
#include <algorithm>
#include <iostream>
#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/QueryUtilities.hxx"
#include "MFront/BehaviourQuery.hxx"

namespace mfront {

  static void displayDefaultParameterValue(const BehaviourData& bd,
                                           const VariableDescription& p) {
    const auto& n = p.name;
    if (p.getTypeFlag() == mfront::SupportedTypes::SCALAR) {
      if (p.arraySize == 1) {
        std::cout << bd.getFloattingPointParameterDefaultValue(n) << '\n';
      } else {
        for (unsigned short i = 0; i != p.arraySize;) {
          std::cout << bd.getFloattingPointParameterDefaultValue(n, i);
          if (++i != p.arraySize) {
            std::cout << " ";
          }
        }
        std::cout << '\n';
      }
    } else if (p.type == "int") {
      std::cout << bd.getIntegerParameterDefaultValue(n) << '\n';
    } else if (p.type == "ushort") {
      std::cout << bd.getUnsignedShortParameterDefaultValue(n) << '\n';
    } else {
      tfel::raise(
          "displayDefaultParameterValue: "
          "unsupported parameter type");
    }
  }  // end of displayDefaultParameterValue

  static const MaterialKnowledgeAttribute& getAttribute(
      const std::string& n,
      const BehaviourDescription& d,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    const auto& as1 = d.getAttributes();
    const auto& as2 = d.getBehaviourData(h).getAttributes();
    auto p = as2.find(n);
    if (p != as2.end()) {
      return p->second;
    }
    p = as1.find(n);
    if (p != as2.end()) {
      return p->second;
    }
    return p->second;
  }

  static std::string to_string(const BehaviourDescription::SlipSystem& gs,
                               const bool f = false) {
    using SlipSystemsDescription = BehaviourDescription::SlipSystemsDescription;
    using std::to_string;
    auto r = std::string{};
    const auto bb = f ? '<' : '[';
    const auto be = f ? '>' : ']';
    const auto pb = f ? '{' : '(';
    const auto pe = f ? '}' : ')';
    if (gs.is<SlipSystemsDescription::system3d>()) {
      const auto& p = gs.get<SlipSystemsDescription::system3d>().plane;
      const auto& b = gs.get<SlipSystemsDescription::system3d>().burgers;
      r += bb + to_string(b[0]) + ',' + to_string(b[1]) + ',' +
           to_string(b[2]) + be;
      r += pb + to_string(p[0]) + ',' + to_string(p[1]) + ',' +
           to_string(p[2]) + pe;
    } else {
      tfel::raise_if(!gs.is<SlipSystemsDescription::system4d>(),
                     "to_string: internal error "
                     "(unsupported slip system type)");
      const auto& p = gs.get<SlipSystemsDescription::system4d>().plane;
      const auto& b = gs.get<SlipSystemsDescription::system4d>().burgers;
      r += bb + to_string(b[0]) + ',' + to_string(b[1]) + ',' +
           to_string(b[2]) + ',' + to_string(b[3]) + be;
      r += pb + to_string(p[0]) + ',' + to_string(p[1]) + ',' +
           to_string(p[2]) + ',' + to_string(p[3]) + pe;
    }
    return r;
  }  // end of to_string

  static std::string to_string(const std::array<long double, 9u>& a) {
    auto r = std::string{};
    r += '[';
    for (decltype(a.size()) i = 0; i != a.size();) {
      r += std::to_string(a[i]);
      if (++i != 9) {
        r += ',';
      }
    }
    r += ']';
    return r;
  }

  static tfel::material::SlipSystemsDescription::vec extract_crystal_direction(
      const std::string& o) {
    using tfel::utilities::tokenize;
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c, "extract_crystal_direction: " + msg);
    };
    auto throw_if2 = [throw_if](const bool c) {
      throw_if(c, "ill defined direction");
    };
    throw_if2(o.size() < 2);
    throw_if2((o[0] != '<') || (o.back() != '>'));
    const auto ds = tokenize(o.substr(1, o.size() - 2), ',');
    auto dc = tfel::material::SlipSystemsDescription::vec{};
    if (ds.size() == 3u) {
      tfel::material::SlipSystemsDescription::vec3d d3d;
      d3d[0] = std::stoi(ds[0]);
      d3d[1] = std::stoi(ds[1]);
      d3d[2] = std::stoi(ds[2]);
      dc = d3d;
    } else {
      throw_if2(ds.size() != 4);
      tfel::material::SlipSystemsDescription::vec4d d4d;
      d4d[0] = std::stoi(ds[0]);
      d4d[1] = std::stoi(ds[1]);
      d4d[2] = std::stoi(ds[2]);
      d4d[3] = std::stoi(ds[3]);
      dc = d4d;
    }
    return dc;
  }  // end of extract_crystal_direction

  BehaviourQuery::BehaviourQuery(const int argc,
                                 const char* const* const argv,
                                 std::shared_ptr<AbstractBehaviourDSL> d,
                                 const std::string& f)
      : QueryHandlerBase(argc, argv), dsl(d), file(f) {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    this->finalizeArgumentsParsing();
    // registring interfaces
    if (!this->interfaces.empty()) {
      this->dsl->setInterfaces(this->interfaces);
    }
  }  // end of BehaviourQuery

  std::shared_ptr<const AbstractDSL> BehaviourQuery::getDSL() const {
    return this->dsl;
  }  // end of getDSL

  void BehaviourQuery::registerCommandLineCallBacks() {
    QueryHandlerBase::registerCommandLineCallBacks();
    //
    this->registerCallBack(
        "--modelling-hypothesis",
        CallBack(
            "select a modelling hypothesis",
            [this] { this->treatModellingHypothesis(); }, true));
    // standard queries
    const std::vector<std::pair<const char*, const char*>> sq = {
        {"--behaviour-name", "show the behaviour name"},
        {"--class-name", "show the class name"},
        {"--author", "show the author name"},
        {"--unit-system", "show the unit system used"},
        {"--description", "show the file description"},
        {"--date", "show the file implementation date"},
        {"--material", "show the material name"},
        {"--list-dependencies", "list the MFront dependencies"},
        {"--library", "show the library name"},
        {"--type",
         "return the behaviour type\n"
         "The value returned has the following meaning:\n"
         "- 0: general behaviour\n"
         "- 1: strain based behaviour\n"
         "- 2: finite strain behaviour\n"
         "- 3: cohesive zone model\n"},
        {"--is-strain-measure-defined",
         "return 'true' is a strain measure "
         "has been defined, 'false' otherwise"},
        {"--strain-measure",
         "return the strain measure used by the behaviour.\n"
         "The following strain measure are defined: "
         "- 'Linearised': HPP behaviour\n"
         "- 'GreenLagrange'\n"
         "- 'Hencky'\n"},
        {"--symmetry",
         "return the behaviour symmetry. If the returned value is 0, "
         "the behaviour is isotropic. If the returned value is 1, "
         "the behaviour is orthotropic."},
        {"--elastic-symmetry",
         "return the symmetry of the elastic part of the behaviour. "
         "If the returned value is 0, this part of the behaviour is isotropic. "
         "If the returned value is 1, this part of the behaviour is "
         "orthotropic."
         "the behaviour is orthotropic."},
        {"--has-crystal-structure",
         "return `true` if a crystal structure has been defined"},
        {"--crystal-structure", "return the crystal structure"},
        {"--slip-systems", "list all the slip systems, sorted by family"},
        {"--slip-systems-by-index", "list all the slip systems"},
        {"--orientation-tensors",
         "list all the orientation tensors, sorted by family"},
        {"--orientation-tensors-by-index", "list all the orientation tensors"},
        {"--orientation-tensors-by-slip-system",
         "list all the orientation tensors"},
        {"--climb-tensors", "list all the climb tensors, sorted by family"},
        {"--climb-tensors-by-index", "list all the climb tensors"},
        {"--climb-tensors-by-slip-system", "list all the climb tensors"},
        {"--interaction-matrix", "show the interaction matrix"},
        {"--dislocations-mean-free-path-interaction-matrix",
         "show the interaction matrix associated to the effect of dislocations "
         "of the "
         "mean-free-path of a dislocation"},
        {"--interaction-matrix-structure",
         "show the structure of the interaction matrix"},
        {"--supported-modelling-hypotheses",
         "show the list of supported modelling hypothesis"},
        {"--gradients", "show the list of the gradients"},
        {"--thermodynamic-forces", "show the list of the thermodynamic forces"},
        {"--tangent-operator-blocks",
         "show the list of blocks in the tangent operator"},
        {"--material-properties",
         "show the list of material properties for the selected modelling "
         "hypothesis"},
        {"--state-variables",
         "show the list of state variables for the selected modelling "
         "hypothesis"},
        {"--auxiliary-state-variables",
         "show the auxiliary state variables properties for the selected "
         "modelling hypothesis"},
        {"--external-state-variables",
         "show the list of external state variables for the selected modelling "
         "hypothesis"},
        {"--integration-variables",
         "show the list of integration variables for the selected modelling "
         "hypothesis"},
        {"--persistent-variables",
         "show the list of persistent variables for the selected modelling "
         "hypothesis"},
        {"--local-variables",
         "show the list of local variables for the selected modelling "
         "hypothesis"},
        {"--parameters",
         "show the list of parameters for the selected modelling hypothesis"},
        {"--static-variables",
         "show the list of static variables for the selected modelling "
         "hypothesis"},
        {"--attributes",
         "show the list of attributes of the behaviour description"},
        {"--code-blocks",
         "show the list of code-blocks of the behaviour description for "
         " the selected modelling hypothesis"},
        {"--parameters-file",
         "display the name of a text file which can be used to modify the "
         "default value of the parameters"},
        {"--initialize-function-variables",
         "show the initialize function variables for the selected "
         "modelling hypothesis"},
        {"--initialize-functions",
         "show the list of available initialize functions for the selected "
         "modelling hypothesis"},
        {"--post-processing-variables",
         "show the post-processing variables for the selected "
         "modelling hypothesis"},
        {"--post-processings",
         "show the list of available post-processings for the selected "
         "modelling hypothesis"}};
    for (const auto& q : sq) {
      this->registerCallBack(
          q.first,
          CallBack(
              q.second, [this] { this->treatStandardQuery(); }, false));
    }
    const std::vector<std::pair<const char*, const char*>> sq2 = {
        {"--attribute-type", "display an attribute type"},
        {"--attribute-value", "display an attribute value"},
        {"--parameter-type", "display a parameter type"},
        {"--parameter-default-value", "display a parameter default value"},
        {"--static-variable-value", "display the value of a static variable"},
        {"--has-bounds",
         "return `true` if a variable has bounds, `false` otherwise"},
        {"--schmid-factors",
         "compute the Schmid factor for all"
         " slip systems for a given direction"},
        {"--schmid-factors-by-index",
         "compute the Schmid factor for all"
         " slip systems for a given direction"},
        {"--bounds-type",
         "return the bounds type associated to a variable.\n"
         "The returned value has the follwing meaning:\n"
         "- `None`\n"
         "- `Lower`\n"
         "- `Upper`\n"
         "- `LowerAndUpper`"},
        {"--bounds-value", "show the bounds value associated as a range"},
        {"--has-physical-bounds",
         "return `true` if a variable has physical bounds, `false` otherwise"},
        {"--physical-bounds-type",
         "return the physical bounds type associated to a variable.\n"
         "The returned value has the follwing meaning:\n"
         "- `None`\n"
         "- `Lower`\n"
         "- `Upper`\n"
         "- `LowerAndUpper`"},
        {"--physical-bounds-value",
         "show the bounds value associated as a range"},
        {"--code-block", "show the information associated with a code block"}};
    for (const auto& q : sq2) {
      this->registerCallBack(
          q.first,
          CallBack(
              q.second, [this] { this->treatStandardQuery2(); }, true));
    }
  }  // end of registerCommandLineCallBacks

  void BehaviourQuery::treatStandardQuery() {
    using namespace tfel::utilities;
    using std::cout;
    using tfel::material::ModellingHypothesis;
    const auto& q = this->getCurrentCommandLineArgument();
    const auto& qn = q.as_string();
    if (qn == "--class-name") {
      this->queries2.push_back(
          {"class-name",
           [](const FileDescription&, const BehaviourDescription& bd) {
             cout << bd.getClassName() << '\n';
           }});
    } else if (qn == "--behaviour-name") {
      this->queries2.push_back(
          {"behaviour-name",
           [](const FileDescription&, const BehaviourDescription& bd) {
             cout << bd.getBehaviourName() << '\n';
           }});
    } else if (qn == "--author") {
      this->queries2.push_back({"author", [](const FileDescription& fd,
                                             const BehaviourDescription&) {
                                  const auto& a = fd.authorName;
                                  cout << (!a.empty() ? a : "(undefined)")
                                       << '\n';
                                }});
    } else if (qn == "--unit-system") {
      this->queries2.push_back(
          {"unit-system",
           [](const FileDescription&, const BehaviourDescription& bd) {
             if (bd.hasUnitSystem()) {
               std::cout << bd.getUnitSystem() << std::endl;
             } else {
               std::cout << "(undefined)" << std::endl;
             }
           }});
    } else if (qn == "--description") {
      this->queries2.push_back(
          {"description",
           [](const FileDescription& fd, const BehaviourDescription&) {
             if (!fd.description.empty()) {
               const auto d = tokenize(fd.description, '\n');
               for (const auto& l : d) {
                 if ((l.size() >= 2) && ((l)[0] == '*') && ((l)[1] == ' ')) {
                   cout << l.substr(2) << '\n';
                 } else {
                   cout << l << '\n';
                 }
               }
             } else {
               cout << "(undefined)\n";
             }
           }});
    } else if (qn == "--date") {
      this->queries2.push_back(
          {"date", [](const FileDescription& fd, const BehaviourDescription&) {
             const auto& d = fd.date;
             cout << (!d.empty() ? d : "(undefined)") << '\n';
           }});
    } else if (qn == "--material") {
      this->queries2.push_back({"material", [](const FileDescription&,
                                               const BehaviourDescription& d) {
                                  const auto& m = d.getMaterialName();
                                  cout << (!m.empty() ? m : "(undefined)")
                                       << '\n';
                                }});
    } else if (qn == "--list-dependencies") {
      this->queries2.push_back(
          {"list-dependencies",
           [](const FileDescription&, const BehaviourDescription& d) {
             for (const auto& dep : d.getExternalMFrontFiles()) {
               std::cout << dep.first << " ";
             }
             std::cout << std::endl;
           }});
    } else if (qn == "--library") {
      this->queries2.push_back({"library", [](const FileDescription&,
                                              const BehaviourDescription& d) {
                                  const auto& l = d.getLibrary();
                                  cout << (!l.empty() ? l : "(undefined)")
                                       << '\n';
                                }});
    } else if (qn == "--type") {
      this->queries2.push_back(
          {"type", [](const FileDescription&, const BehaviourDescription& d) {
             const auto t = d.getBehaviourType();
             if (t == BehaviourDescription::GENERALBEHAVIOUR) {
               std::cout << 0 << std::endl;
             } else if (t ==
                        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
               std::cout << 1 << std::endl;
             } else if (t ==
                        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
               std::cout << 2 << std::endl;
             } else if (t == BehaviourDescription::COHESIVEZONEMODEL) {
               std::cout << 3 << std::endl;
             } else {
               tfel::raise("unsupported behaviour type");
             }
           }});
    } else if (qn == "--is-strain-measure-defined") {
      this->queries2.push_back(
          {"is-strain-measure-defined",
           [](const FileDescription&, const BehaviourDescription& d) {
             const auto t = d.getBehaviourType();
             tfel::raise_if(
                 t != BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                 "the behaviour is not based on strain");
             cout << (d.isStrainMeasureDefined() ? "true\n" : "false\n");
           }});
    } else if (qn == "--strain-measure") {
      this->queries2.push_back(
          {"strain-measure",
           [](const FileDescription&, const BehaviourDescription& d) {
             const auto t = d.getBehaviourType();
             tfel::raise_if(
                 t != BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                 "the behaviour is not based on a strain measure");
             tfel::raise_if(!d.isStrainMeasureDefined(),
                            "no strain measure defined");
             const auto ss = d.getStrainMeasure();
             if (ss == BehaviourDescription::LINEARISED) {
               cout << "Linearised";
             } else if (ss == BehaviourDescription::GREENLAGRANGE) {
               cout << "GreenLagrange\n";
             } else if (ss == BehaviourDescription::HENCKY) {
               cout << "Hencky\n";
             } else {
               tfel::raise_if(true,
                              "internal error "
                              "(unsupported strain measure)");
             }
           }});
    } else if (qn == "--symmetry") {
      this->queries2.push_back({"symmetry", [](const FileDescription&,
                                               const BehaviourDescription& d) {
                                  const auto s = d.getSymmetryType();
                                  if (s == mfront::ISOTROPIC) {
                                    cout << 0 << '\n';
                                  } else if (s == mfront::ORTHOTROPIC) {
                                    cout << 1 << '\n';
                                  } else {
                                    tfel::raise("unsupported symmetry");
                                  }
                                }});
    } else if (qn == "--has-crystal-structure") {
      this->queries2.push_back(
          {"has-crystal-structure",
           [](const FileDescription&, const BehaviourDescription& d) {
             if (d.hasCrystalStructure()) {
               cout << "true\n";
             } else {
               cout << "false\n";
             }
           }});
    } else if (qn == "--crystal-structure") {
      this->queries2.push_back(
          {"crystal-structure",
           [](const FileDescription&, const BehaviourDescription& d) {
             using tfel::material::CrystalStructure;
             const auto s = d.getCrystalStructure();
             if (s == CrystalStructure::Cubic) {
               cout << "Cubic\n";
             } else if (s == CrystalStructure::HCP) {
               cout << "Hexagonal closed-packed\n";
             } else if (s == CrystalStructure::FCC) {
               cout << "Face-centered cubic\n";
             } else if (s == CrystalStructure::BCC) {
               cout << "Body-centered cubic\n";
             } else {
               tfel::raise("unsupported crystal structure");
             }
           }});
    } else if (qn == "--slip-systems") {
      this->queries2.push_back(
          {"slip-systems",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             for (size_t i = 0; i != nss; ++i) {
               const auto gss = ssd.getSlipSystems(i);
               cout << "- " << to_string(ssd.getSlipSystemFamily(i), true)
                    << ":";
               for (const auto& gs : gss) {
                 cout << " " << to_string(gs);
               }
               cout << '\n';
             }
           }});
    } else if (qn == "--slip-systems-by-index") {
      this->queries2.push_back(
          {"slip-systems-by-index",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             auto r = size_t{};
             for (size_t i = 0; i != nss; ++i) {
               const auto gss = ssd.getSlipSystems(i);
               for (const auto& gs : gss) {
                 cout << "- " << r << ": " << to_string(gs) << '\n';
                 ++r;
               }
             }
           }});
    } else if (qn == "--orientation-tensors") {
      this->queries2.push_back(
          {"orientation-tensors",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             for (size_t i = 0; i != nss; ++i) {
               const auto os = ssd.getOrientationTensors(i);
               cout << "- " << to_string(ssd.getSlipSystemFamily(i), true)
                    << ":";
               for (const auto& o : os) {
                 cout << " " << to_string(o);
               }
               cout << '\n';
             }
           }});
    } else if (qn == "--orientation-tensors-by-index") {
      this->queries2.push_back(
          {"orientation-tensors-by-index",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             auto r = size_t{};
             for (size_t i = 0; i != nss; ++i) {
               const auto os = ssd.getOrientationTensors(i);
               for (const auto& o : os) {
                 cout << "- " << r << ": " << to_string(o) << '\n';
                 ++r;
               }
             }
           }});
    } else if (qn == "--orientation-tensors-by-slip-system") {
      this->queries2.push_back(
          {"orientation-tensors-by-slip-system",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             for (size_t i = 0; i != nss; ++i) {
               const auto os = ssd.getOrientationTensors(i);
               const auto ss = ssd.getSlipSystems(i);
               for (decltype(ss.size()) j = 0; j != ss.size(); ++j) {
                 cout << "- " << to_string(ss[j]) << ": " << to_string(os[j])
                      << '\n';
               }
             }
           }});
    } else if (qn == "--climb-tensors") {
      this->queries2.push_back(
          {"climb-tensors",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             for (size_t i = 0; i != nss; ++i) {
               const auto os = ssd.getClimbTensors(i);
               cout << "- " << to_string(ssd.getSlipSystemFamily(i), true)
                    << ":";
               for (const auto& o : os) {
                 cout << " " << to_string(o);
               }
               cout << '\n';
             }
           }});
    } else if (qn == "--climb-tensors-by-index") {
      this->queries2.push_back(
          {"climb-tensors-by-index",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             auto r = size_t{};
             for (size_t i = 0; i != nss; ++i) {
               const auto os = ssd.getClimbTensors(i);
               for (const auto& o : os) {
                 cout << "- " << r << ": " << to_string(o) << '\n';
                 ++r;
               }
             }
           }});
    } else if (qn == "--climb-tensors-by-slip-system") {
      this->queries2.push_back(
          {"climb-tensors-by-slip-system",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             for (size_t i = 0; i != nss; ++i) {
               const auto os = ssd.getClimbTensors(i);
               const auto ss = ssd.getSlipSystems(i);
               for (decltype(ss.size()) j = 0; j != ss.size(); ++j) {
                 cout << "- " << to_string(ss[j]) << ": " << to_string(os[j])
                      << '\n';
               }
             }
           }});
    } else if (qn == "--interaction-matrix-structure") {
      this->queries2.push_back(
          {"interaction-matrix-structure",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& im = d.getInteractionMatrixStructure();
             const auto r = im.rank();
             const auto& ssis_r = im.getSlidingSystemsInteraction();
             cout << "- number of independent coefficients: " << r << '\n';
             auto i = size_t{};
             for (const auto& ssis : ssis_r) {
               cout << "- rank " << i << ':';
               for (const auto& ssi : ssis) {
                 cout << " (" << to_string(ssi.g1) << ':' << to_string(ssi.g2)
                      << ')';
               }
               cout << '\n';
               ++i;
             }
           }});
    } else if (qn == "--interaction-matrix") {
      this->queries2.push_back(
          {"interaction-matrix",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto& im = d.getInteractionMatrixStructure();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             for (size_t i = 0; i != nss; ++i) {
               for (const auto& gs1 : ssd.getSlipSystems(i)) {
                 cout << '|';
                 for (size_t j = 0; j != nss; ++j) {
                   for (const auto& gs2 : ssd.getSlipSystems(j)) {
                     cout << " " << im.getRank(gs1, gs2);
                   }
                 }
                 cout << " |\n";
               }
             }
             if (d.hasInteractionMatrix()) {
               cout << "with:\n";
               const auto& ivs = ssd.getInteractionMatrix();
               for (size_t i = 0; i != ivs.size(); ++i) {
                 cout << "- coefficient '" << i << "': " << ivs[i] << '\n';
               }
             }
           }});
    } else if (qn == "--dislocations-mean-free-path-interaction-matrix") {
      this->queries2.push_back(
          {"dislocations-mean-free-path-interaction-matrix",
           [](const FileDescription&, const BehaviourDescription& d) {
             tfel::raise_if(!d.areSlipSystemsDefined(),
                            "no slip system defined");
             const auto& ssd = d.getSlipSystems();
             const auto& im = d.getInteractionMatrixStructure();
             const auto nss = ssd.getNumberOfSlipSystemsFamilies();
             for (size_t i = 0; i != nss; ++i) {
               for (const auto& gs1 : ssd.getSlipSystems(i)) {
                 cout << '|';
                 for (size_t j = 0; j != nss; ++j) {
                   for (const auto& gs2 : ssd.getSlipSystems(j)) {
                     cout << " " << im.getRank(gs1, gs2);
                   }
                 }
                 cout << " |\n";
               }
             }
             if (d.hasDislocationsMeanFreePathInteractionMatrix()) {
               cout << "with:\n";
               const auto& ivs =
                   ssd.getDislocationsMeanFreePathInteractionMatrix();
               for (size_t i = 0; i != ivs.size(); ++i) {
                 cout << "- coefficient '" << i << "': " << ivs[i] << '\n';
               }
             }
           }});
    } else if (qn == "--elastic-symmetry") {
      this->queries2.push_back(
          {"elastic-symmetry",
           [](const FileDescription&, const BehaviourDescription& d) {
             const auto s = d.getElasticSymmetryType();
             if (s == mfront::ISOTROPIC) {
               cout << 0 << '\n';
             } else if (s == mfront::ORTHOTROPIC) {
               cout << 1 << '\n';
             } else {
               tfel::raise("unsupported elastic symmetry");
             }
           }});
    } else if (qn == "--supported-modelling-hypotheses") {
      this->queries2.push_back(
          {"supported-modelling-hypotheses",
           [](const FileDescription&, const BehaviourDescription& d) {
             const auto& mh = d.getModellingHypotheses();
             for (const auto& h : mh) {
               cout << ModellingHypothesis::toString(h) << " ";
             }
             cout << '\n';
           }});
    } else if (qn == "--gradients") {
      this->queries2.push_back({"gradients", [](const FileDescription&,
                                                const BehaviourDescription& d) {
                                  for (const auto& v : d.getMainVariables()) {
                                    QueryHandlerBase::displayVariable(v.first);
                                  }
                                }});
    } else if (qn == "--thermodynamic-forces") {
      this->queries2.push_back(
          {"thermodynamic-forces",
           [](const FileDescription&, const BehaviourDescription& d) {
             for (const auto& v : d.getMainVariables()) {
               QueryHandlerBase::displayVariable(v.second);
             }
           }});
    } else if (qn == "--tangent-operator-blocks") {
      this->queries2.push_back(
          {"tangent-operator-blocks",
           [](const FileDescription&, const BehaviourDescription& d) {
             for (const auto& b : d.getTangentOperatorBlocks()) {
               std::cout << "-" << d.getTangentOperatorBlockSymbolicName(b)
                         << ": derivative of '" << b.first.getExternalName()
                         << "' with respect to '" << b.second.getExternalName()
                         << "'\n";
             }
             std::cout << '\n';
           }});
    } else if (qn == "--material-properties") {
      this->queries.emplace_back("material-properties",
                                 this->generateVariablesListQuery<
                                     &BehaviourData::getMaterialProperties>());
    } else if (qn == "--state-variables") {
      this->queries.emplace_back("state-variables",
                                 this->generateVariablesListQuery<
                                     &BehaviourData::getStateVariables>());
    } else if (qn == "--static-variables") {
      auto nq = [](const FileDescription&, const BehaviourDescription& bd,
                   const Hypothesis h) {
        using namespace std;
        using namespace tfel::glossary;
        const auto& d = bd.getBehaviourData(h);
        const auto& vars = d.getStaticVariables();
        for (const auto& v : vars) {
          const auto& n = d.getExternalName(v.name);
          cout << "- " << n;
          if (v.arraySize != 1u) {
            cout << '[' << v.arraySize << ']';
          }
          if (n != v.name) {
            cout << " (" << mfront::displayName(v) << ")";
          }
          if (!v.description.empty()) {
            cout << ": " << v.description;
          } else {
            const auto& glossary = Glossary::getGlossary();
            if (glossary.contains(n)) {
              cout << ": "
                   << glossary.getGlossaryEntry(n).getShortDescription();
            }
          }
          cout << endl;
        }
        if (areParametersTreatedAsStaticVariables(bd)) {
          for (const auto& v : d.getParameters()) {
            QueryHandlerBase::displayVariable(v);
          }
        }
      };
      this->queries.emplace_back("static-variables", nq);
    } else if (qn == "--auxiliary-state-variables") {
      this->queries.emplace_back(
          "auxiliary-state-variables",
          this->generateVariablesListQuery<
              &BehaviourData::getAuxiliaryStateVariables>());
    } else if (qn == "--initialize-function-variables") {
      this->queries.emplace_back(
          "initialize-function-variables",
          this->generateVariablesListQuery<
              &BehaviourData::getInitializeFunctionVariables>());
    } else if (qn == "--post-processing-variables") {
      this->queries.emplace_back(
          "post-processing-variables",
          this->generateVariablesListQuery<
              &BehaviourData::getPostProcessingVariables>());
    } else if (qn == "--external-state-variables") {
      this->queries.emplace_back(
          "external-state-variables",
          this->generateVariablesListQuery<
              &BehaviourData::getExternalStateVariables>());
    } else if (qn == "--integration-variables") {
      this->queries.emplace_back(
          "integration-state-variables",
          this->generateVariablesListQuery<
              &BehaviourData::getIntegrationVariables>());
    } else if (qn == "--persistent-variables") {
      this->queries.emplace_back("persistent-state-variables",
                                 this->generateVariablesListQuery<
                                     &BehaviourData::getPersistentVariables>());
    } else if (qn == "--local-variables") {
      this->queries.emplace_back("local-state-variables",
                                 this->generateVariablesListQuery<
                                     &BehaviourData::getLocalVariables>());
    } else if (qn == "--parameters") {
      auto nq = [](const FileDescription&, const BehaviourDescription& bd,
                   const Hypothesis h) {
        if (!areParametersTreatedAsStaticVariables(bd)) {
          const auto& d = bd.getBehaviourData(h);
          for (const auto& v : d.getParameters()) {
            QueryHandlerBase::displayVariable(v);
          }
        }
      };
      this->queries.emplace_back("parameters", nq);
    } else if (qn == "--attributes") {
      this->queries.push_back(
          {"attributes", [](const FileDescription&,
                            const BehaviourDescription& d, const Hypothesis h) {
             const auto& as1 = d.getAttributes();
             if (!as1.empty()) {
               for (const auto& a : as1) {
                 cout << a.first << " ";
               }
             }
             const auto& as2 = d.getBehaviourData(h).getAttributes();
             for (const auto& a : as2) {
               cout << a.first << " ";
             }
             cout << '\n';
           }});
    } else if (qn == "--code-blocks") {
      this->queries.push_back({"code-blocks", [](const FileDescription&,
                                                 const BehaviourDescription& d,
                                                 const Hypothesis h) {
                                 const auto& names =
                                     d.getBehaviourData(h).getCodeBlockNames();
                                 for (const auto& n : names) {
                                   cout << "- " << n << '\n';
                                 }
                               }});
    } else if (qn == "--initialize-functions") {
      this->queries.push_back(
          {"initialize-functions",
           [](const FileDescription&, const BehaviourDescription& d,
              const Hypothesis h) {
             for (const auto& p :
                  d.getBehaviourData(h).getInitializeFunctions()) {
               const auto& c = p.second;
               cout << "- " << p.first << ": ";
               if (!c.description.empty()) {
                 cout << c.description << ".";
               } else {
                 cout << "no description available.";
               }
               if (hasAttribute<std::vector<VariableDescription>>(
                       c, CodeBlock::used_initialize_function_variables)) {
                 const auto& ivariables =
                     getAttribute<std::vector<VariableDescription>>(
                         c, CodeBlock::used_initialize_function_variables);
                 if (!ivariables.empty()) {
                   std::cout << "Expected initialize function variables are:\n";
                   for (const auto& v : ivariables) {
                     QueryHandlerBase::displayVariable(v, "  ");
                   }
                 } else {
                   std::cout << '\n';
                 }
               }
             }
           }});
    } else if (qn == "--post-processings") {
      this->queries.push_back(
          {"post-processings",
           [](const FileDescription&, const BehaviourDescription& d,
              const Hypothesis h) {
             for (const auto& p : d.getBehaviourData(h).getPostProcessings()) {
               const auto& c = p.second;
               cout << "- " << p.first << ": ";
               if (!c.description.empty()) {
                 cout << c.description;
               } else {
                 cout << "no description available";
               }
               if (hasAttribute<std::vector<VariableDescription>>(
                       c, CodeBlock::used_postprocessing_variables)) {
                 const auto& pvariables =
                     getAttribute<std::vector<VariableDescription>>(
                         c, CodeBlock::used_postprocessing_variables);
                 std::cout << ". Modified post-processing variables are:\n";
                 for (const auto& v : pvariables) {
                   QueryHandlerBase::displayVariable(v, "  ");
                 }
               }
             }
           }});
    } else if (qn == "--parameters-file") {
      this->queries.push_back(
          {"parameters-file",
           [](const FileDescription&, const BehaviourDescription& d,
              const Hypothesis h) {
             if (!areParametersTreatedAsStaticVariables(d)) {
               if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
                 cout << mfront::getParametersFileName(d, h) << '\n';
               } else {
                 cout << mfront::getParametersFileName(d) << '\n';
               }
             }
           }});
    } else {
      tfel::raise(
          "Behaviour::treatStandardQuery : "
          "unsupported query '" +
          qn + "'");
    }
  }

  void BehaviourQuery::treatStandardQuery2() {
    using namespace std;
    using namespace tfel::utilities;
    using tfel::material::ModellingHypothesis;
    const auto& q = this->getCurrentCommandLineArgument();
    const auto& qn = q.as_string();
    const auto o = q.getOption();
    tfel::raise_if(o.empty(),
                   "Behaviour::treatStandardQuery2 : "
                   "no option given to the '" +
                       qn + "' query");
    if (qn == "--attribute-type") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto& a = getAttribute(o, d, h);
        if (a.is<bool>()) {
          cout << "bool\n";
        } else if (a.is<unsigned short>()) {
          cout << "unsigned short\n";
        } else if (a.is<string>()) {
          cout << "string\n";
        } else {
          tfel::raise(
              "Behaviour::treatStandardQuery2 : "
              "unsupported attribute type");
        }
      };
      this->queries.push_back({"attribute-type", l});
    } else if (qn == "--attribute-value") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto& a = getAttribute(o, d, h);
        if (a.is<bool>()) {
          if (a.get<bool>()) {
            cout << "true\n";
          } else {
            cout << "false\n";
          }
        } else if (a.is<unsigned short>()) {
          cout << a.get<unsigned short>() << '\n';
        } else if (a.is<string>()) {
          cout << a.get<string>() << '\n';
        } else if (a.is<vector<string>>()) {
          const auto& vs = a.get<vector<string>>();
          bool first = true;
          for (const auto& v : vs) {
            if (!first) {
              cout << " ";
            }
            cout << v;
            first = false;
          }
          cout << '\n';
        } else {
          tfel::raise(
              "Behaviour::treatStandardQuery2 : "
              "unsupported attribute type");
        }
      };
      this->queries.push_back({"attributes-value", l});
    } else if (qn == "--has-bounds") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto vn = extractVariableNameAndArrayPosition(o);
        const auto& n = std::get<0>(vn);
        const auto& bd = d.getBehaviourData(h);
        const auto& p = bd.getVariableDescriptionByExternalName(n);
        if (std::get<1>(vn)) {
          cout << (p.hasBounds(std::get<2>(vn)) ? "true" : "false") << '\n';
        } else {
          cout << (p.hasBounds() ? "true" : "false") << '\n';
        }
      };
      this->queries.push_back({"has-bounds", l});
    } else if (qn == "--bounds-type") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto& bd = d.getBehaviourData(h);
        const auto vn = extractVariableNameAndArrayPosition(o);
        const auto& n = std::get<0>(vn);
        const auto& p = bd.getVariableDescriptionByExternalName(n);
        const auto& b =
            std::get<1>(vn) ? p.getBounds(std::get<2>(vn)) : p.getBounds();
        displayBounds(cout, b);
      };
      this->queries.push_back({"bounds-type", l});
    } else if (qn == "--bounds-value") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto& bd = d.getBehaviourData(h);
        const auto vn = extractVariableNameAndArrayPosition(o);
        const auto& n = std::get<0>(vn);
        const auto& p = bd.getVariableDescriptionByExternalName(n);
        const auto& b =
            std::get<1>(vn) ? p.getBounds(std::get<2>(vn)) : p.getBounds();
        displayBoundsValue(cout, b);
      };
      this->queries.push_back({"bounds-value", l});
    } else if (qn == "--has-physical-bounds") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto vn = extractVariableNameAndArrayPosition(o);
        const auto& n = std::get<0>(vn);
        const auto& bd = d.getBehaviourData(h);
        const auto& p = bd.getVariableDescriptionByExternalName(n);
        if (std::get<1>(vn)) {
          cout << (p.hasPhysicalBounds(std::get<2>(vn)) ? "true" : "false")
               << '\n';
        } else {
          cout << (p.hasPhysicalBounds() ? "true" : "false") << '\n';
        }
      };
      this->queries.push_back({"has-physical-bounds", l});
    } else if (qn == "--physical-bounds-type") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto& bd = d.getBehaviourData(h);
        const auto vn = extractVariableNameAndArrayPosition(o);
        const auto& n = std::get<0>(vn);
        const auto& p = bd.getVariableDescriptionByExternalName(n);
        const auto& b = (std::get<1>(vn) ? p.getPhysicalBounds(std::get<2>(vn))
                                         : p.getPhysicalBounds());
        displayBounds(cout, b);
      };
      this->queries.push_back({"physical-bounds-type", l});
    } else if (qn == "--physical-bounds-value") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto& bd = d.getBehaviourData(h);
        const auto vn = extractVariableNameAndArrayPosition(o);
        const auto& n = std::get<0>(vn);
        const auto& p = bd.getVariableDescriptionByExternalName(n);
        const auto& b = (std::get<1>(vn) ? p.getPhysicalBounds(std::get<2>(vn))
                                         : p.getPhysicalBounds());
        displayBoundsValue(cout, b);
      };
      this->queries.push_back({"physical-bounds-value", l});
    } else if (qn == "--parameter-type") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        if (!areParametersTreatedAsStaticVariables(d)) {
          const auto& bd = d.getBehaviourData(h);
          const auto& p = bd.getParameterDescriptionByExternalName(o);
          cout << p.type << '\n';
        }
      };
      this->queries.push_back({"parameter-type", l});
    } else if (qn == "--parameter-default-value") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        if (areParametersTreatedAsStaticVariables(d)) {
          return;
        }
        const auto& bd = d.getBehaviourData(h);
        const auto& p = bd.getParameterDescriptionByExternalName(o);
        displayDefaultParameterValue(bd, p);
      };
      this->queries.push_back({"parameter-default-value", l});
    } else if (qn == "--static-variable-value") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto& bd = d.getBehaviourData(h);
        if (areParametersTreatedAsStaticVariables(d)) {
          const auto& parameters = bd.getParameters();
          const auto p = findByExternalName(parameters, o);
          if (p != parameters.end()) {
            const auto& v = parameters.getVariableByExternalName(o);
            displayDefaultParameterValue(bd, v);
            return;
          }
        }
        for (const auto& sv : bd.getStaticVariables()) {
          if (sv.name == o) {
            std::cout << sv.value << '\n';
            return;
          }
        }
        tfel::raise(
            "Behaviour::treatStandardQuery2 : "
            "no static variable '" +
            o + "'");
      };
      this->queries.push_back({"static-variable-value", l});
    } else if (qn == "--schmid-factors") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d) {
        tfel::raise_if(!d.areSlipSystemsDefined(), "no slip system defined");
        const auto dc = extract_crystal_direction(o);
        const auto& ssd = d.getSlipSystems();
        const auto nss = ssd.getNumberOfSlipSystemsFamilies();
        for (size_t i = 0; i != nss; ++i) {
          const auto sfs = ssd.getSchmidFactors(dc, i);
          cout << "- " << to_string(ssd.getSlipSystemFamily(i), true) << ":";
          for (const auto& sf : sfs) {
            cout << " " << std::to_string(sf);
          }
          cout << '\n';
        }
      };
      this->queries2.push_back({"schmid-factors", l});
    } else if (qn == "--code-block") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d,
                   const Hypothesis h) {
        const auto& bd = d.getBehaviourData(h);
        const auto& c = bd.getCodeBlock(o);
        std::cout << "- " << o << ": ";
        if (!c.description.empty()) {
          std::cout << c.description << std::endl;
        } else {
          std::cout << "(no description available)" << std::endl;
        }
        if (!c.members.empty()) {
          std::cout << "  - used variables: ";
          std::copy(c.members.begin(), c.members.end(),
                    std::ostream_iterator<std::string>(std::cout, " "));
          std::cout << endl;
        }
        if (!c.staticMembers.empty()) {
          std::cout << "  - used static variables : ";
          std::copy(c.staticMembers.begin(), c.staticMembers.end(),
                    std::ostream_iterator<std::string>(std::cout, " "));
          std::cout << endl;
        }
        if (!c.code.empty()) {
          std::stringstream scode(c.code);
          std::string code_line;
          std::cout << "  - code:\n";
          while (std::getline(scode, code_line)) {
            std::cout << "  " << code_line << std::endl;
          }
        }
      };
      this->queries.push_back({"code-block", l});
    } else if (qn == "--schmid-factors-by-index") {
      auto l = [o](const FileDescription&, const BehaviourDescription& d) {
        tfel::raise_if(!d.areSlipSystemsDefined(), "no slip system defined");
        const auto dc = extract_crystal_direction(o);
        const auto& ssd = d.getSlipSystems();
        const auto sfs = ssd.getSchmidFactors(dc);
        size_t idx = 0;
        for (size_t i = 0; i != sfs.size(); ++i) {
          for (const auto sf : sfs[i]) {
            cout << "- " << idx << ": " << std::to_string(sf) << '\n';
            ++idx;
          }
        }
      };
      this->queries2.push_back({"schmid-factors-by-index", l});
    } else {
      tfel::raise(
          "Behaviour::treatStandardQuery : "
          "unsupported query '" +
          qn + "'");
    }
  }

  template <const VariableDescriptionContainer& (BehaviourData::*m)() const>
  typename BehaviourQuery::query BehaviourQuery::generateVariablesListQuery() {
    return [](const FileDescription&, const BehaviourDescription& bd,
              const Hypothesis h) {
      const auto& d = bd.getBehaviourData(h);
      const auto& vars = (d.*m)();
      for (const auto& v : vars) {
        QueryHandlerBase::displayVariable(v);
      }
    };
  }

  void BehaviourQuery::treatGeneratedSources() {
    auto q = this->generateGeneratedSourcesQuery(
        this->getCurrentCommandLineArgument().getOption());
    this->queries2.push_back(
        {"generated-sources",
         [q](const FileDescription&, const BehaviourDescription&) { q(); }});
  }  // end of treatGeneratedSources

  void BehaviourQuery::treatSpecificTargetGeneratedSources() {
    auto q = this->generateSpecificTargetGeneratedSourcesQuery(
        this->getCurrentCommandLineArgument().getOption());
    this->queries2.push_back(
        {"specific-target-generated-sources",
         [q](const FileDescription&, const BehaviourDescription&) { q(); }});
  }  // end of treatSpecificTargetGeneratedSources

  void BehaviourQuery::treatAllSpecificTargetsGeneratedSources() {
    auto q = this->generateAllSpecificTargetsGeneratedSourcesQuery();
    this->queries2.push_back(
        {"all-specific-targets-generated-sources",
         [q](const FileDescription&, const BehaviourDescription&) { q(); }});
  }  // end of treatAllSpecificTargetsGeneratedSources

  void BehaviourQuery::treatGeneratedHeaders() {
    auto q = this->generateGeneratedHeadersQuery();
    this->queries2.push_back(
        {"generated-headers",
         [q](const FileDescription&, const BehaviourDescription&) { q(); }});
  }  // end of treatGeneratedHeaders

  void BehaviourQuery::treatCppFlags() {
    auto q = this->generateCppFlagsQuery();
    this->queries2.push_back(
        {"cppflags",
         [q](const FileDescription&, const BehaviourDescription&) { q(); }});
  }  // end of treatCppFlags

  void BehaviourQuery::treatLibrariesDependencies() {
    auto q = this->generateLibrariesDependenciesQuery();
    this->queries2.push_back(
        {"libraries-dependencies",
         [q](const FileDescription&, const BehaviourDescription&) { q(); }});
  }  // end of treatLibrariesDependencies

  void BehaviourQuery::treatSpecificTargets() {
    auto q = this->generateSpecificTargetsQuery();
    this->queries2.push_back(
        {"specific-targets",
         [q](const FileDescription&, const BehaviourDescription&) { q(); }});
  }  // end of treatSpecificTargets

  void BehaviourQuery::treatModellingHypothesis() {
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),
                   "BehaviourQuery::treatModellingHypothesis: "
                   "no option given to --modelling-hypothesis"
                   "command line option");
    tfel::raise_if(this->hypothesis != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "BehaviourQuery::treatModellingHypothesis: "
                   "modelling hypothesis already defined");
    this->hypothesis = ModellingHypothesis::fromString(o);
  }  // end of treatModellingHypothesis

  std::size_t BehaviourQuery::getNumberOfQueriesToBeTreated() {
    return this->queries.size() + this->queries2.size();
  }  // end of getNumberOfQueriesToBeTreated

  void BehaviourQuery::exe() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "Treating file '" << this->file << "'\n";
    }
    // analysing the file
    this->dsl->analyseFile(this->file, this->ecmds, this->substitutions);
    const auto& fd = this->dsl->getFileDescription();
    const auto& d = this->dsl->getBehaviourDescription();
    // checks
    if (!this->queries.empty()) {
      if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        if (d.areAllMechanicalDataSpecialised()) {
          std::ostringstream msg;
          const auto& mh = d.getModellingHypotheses();
          if (mh.size() == 1u) {
            this->hypothesis = *(mh.begin());
          } else {
            msg << "BehaviourQuery::exe : all mechanical data are "
                   "specialised.\n"
                << "A modelling hypothesis must be specified using "
                << "the '--modelling-hypothesis' command line option.\n"
                << "The supported modelling hypothesis are : \n";
            for (const auto h : mh) {
              msg << "- " << ModellingHypothesis::toString(h) << '\n';
            }
            tfel::raise(msg.str());
          }
        }
      } else {
        const auto& mh = d.getModellingHypotheses();
        tfel::raise_if(
            mh.find(this->hypothesis) == mh.end(),
            "BehaviourQuery::exe: the specified modelling hypothesis "
            "('" +
                ModellingHypothesis::toString(this->hypothesis) +
                "') "
                "is not supported by the behaviour");
      }
    }
    // treating the queries
    for (const auto& q : this->queries) {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream() << "Treating query '" << q.first << "'\n";
      }
      q.second(fd, d, this->hypothesis);
    }
    for (const auto& q : this->queries2) {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream() << "Treating query '" << q.first << "'\n";
      }
      q.second(fd, d);
    }
  }  // end of exe

  void BehaviourQuery::treatDSLTarget() {
    std::cout << "behaviour" << std::endl;
  }  // end of treatDSLTarget

  BehaviourQuery::~BehaviourQuery() = default;

}  // end of namespace mfront
