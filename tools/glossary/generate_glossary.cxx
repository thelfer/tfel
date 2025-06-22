/*!
 * \file  tools/glossary/generate_glossary.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 14 mai 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

// g++ -std=c++17 `tfel-config --includes --libs --utilities --glossary`
// generate_glossary.cxx -o generate_glossary

// g++ -std=c++17 generate_glossary.cxx  -o generate_glossary `tfel-config
// --includes` `tfel-config --libs` -lTFELUtilities  -lTFELGlossary &&
// ./generate_glossary && mv Glossary.hxx
// ../../include/TFEL/Glossary/Glossary.hxx && mv Glossary.cxx
// ../../src/Glossary/Glossary.cxx && mv PythonGlossary.cxx
// ../../bindings/python/tfel/Glossary.cxx && pandoc -f
// markdown+tex_math_single_backslash --toc  glossary-pandoc.txt -o
// /tmp/glossary.docx && mv glossary-pandoc.txt ../../docs/web/glossary.md

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <set>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

struct GlossaryTokenizer : protected tfel::utilities::CxxTokenizer,
                           protected std::set<tfel::glossary::GlossaryEntry> {
  using std::set<tfel::glossary::GlossaryEntry>::begin;

  using std::set<tfel::glossary::GlossaryEntry>::end;

  using std::set<tfel::glossary::GlossaryEntry>::const_iterator;

  using std::set<tfel::glossary::GlossaryEntry>::empty;

  using std::set<tfel::glossary::GlossaryEntry>::size;

  void execute(const std::string& f) {
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    std::set<std::string> keys;
    std::set<std::string> names;
    this->treatCharAsString(true);
    this->mergeStrings(true);
    this->openFile(f);
    // stripping comment
    this->stripComments();
    auto p = CxxTokenizer::begin();
    const auto pe = CxxTokenizer::end();
    while (p != pe) {
      std::string key;
      std::string name;
      std::string unit;
      std::string type;
      std::string short_description;
      std::vector<std::string> description;
      std::vector<std::string> notes;
      std::vector<std::string> aliases;
      key = CxxTokenizer::readString(p, pe);
      tfel::raise_if(!this->isValidIdentifier(key, false),
                     "GlossaryTokenizer: invalid glossary key '" + key + "'");
      tfel::raise_if(!keys.insert(key).second,
                     "GlossaryTokenizer::execute: "
                     "key '" +
                         key + "' already used");
      CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", "{", p,
                                       pe);
      CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::execute", p, pe);
      while (p->value != "}") {
        const auto k = p->value;
        ++p;
        CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", ":", p,
                                         pe);
        try {
          if (k == "alias") {
            tfel::raise_if(!aliases.empty(),
                           "GlossaryTokenizer::execute: "
                           "aliases already defined for key '" +
                               key + "'");
            aliases.push_back(CxxTokenizer::readString(p, pe));
          } else if (k == "aliases") {
            tfel::raise_if(!aliases.empty(),
                           "GlossaryTokenizer::execute: "
                           "aliases already defined for key '" +
                               key + "'");
            this->readBlock(aliases, p, pe);
          } else if (k == "unit") {
            tfel::raise_if(!unit.empty(),
                           "GlossaryTokenizer::execute: "
                           "unit already defined for key '" +
                               key + "'");
            unit = CxxTokenizer::readString(p, pe);
          } else if (k == "name") {
            tfel::raise_if(!name.empty(),
                           "GlossaryTokenizer::execute: "
                           "name already defined for key '" +
                               key + "'");
            name = CxxTokenizer::readString(p, pe);
          } else if (k == "type") {
            tfel::raise_if(!type.empty(),
                           "GlossaryTokenizer::execute: "
                           "type already defined for key '" +
                               key + "'");
            type = CxxTokenizer::readString(p, pe);
          } else if (k == "short_description") {
            tfel::raise_if(!short_description.empty(),
                           "GlossaryTokenizer::execute: "
                           "short description already defined for key '" +
                               key + "'");
            short_description = CxxTokenizer::readString(p, pe);
          } else if (k == "description") {
            tfel::raise_if(!description.empty(),
                           "GlossaryTokenizer::execute: "
                           "description already defined for key '" +
                               key + "'");
            this->readBlock(description, p, pe);
            CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", "}",
                                             p, pe);
          } else if (k == "notes") {
            tfel::raise_if(!notes.empty(),
                           "GlossaryTokenizer::execute: "
                           "notes already defined for key '" +
                               key + "'");
            this->readBlock(notes, p, pe);
            CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", "}",
                                             p, pe);
          } else {
            tfel::raise(
                "GlossaryTokenizer::execute: "
                "unknown key '" +
                k + "'");
          }
          CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", ";", p,
                                           pe);
          CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::execute", p, pe);
        } catch (std::exception& ex) {
          tfel::raise(
              "GlossaryTokenizer::execute: "
              "error while treating key '" +
              key +
              "' "
              "(" +
              std::string(ex.what()) + ")");
        }
      }
      CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", "}", p,
                                       pe);
      CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", ";", p,
                                       pe);
      // postprocessing
      tfel::raise_if(short_description.empty(),
                     "GlossaryTokenizer::execute: "
                     "no short_description given for glossary entry '" +
                         key + "'");
      tfel::raise_if(type.empty(),
                     "GlossaryTokenizer::execute: "
                     "no type given for glossary entry '" +
                         key + "'");
      if (name.empty()) {
        name = key;
      }
      tfel::raise_if(!names.insert(name).second,
                     "GlossaryTokenizer::execute: "
                     "name '" +
                         name +
                         "' already used. "
                         "Error while defining key '" +
                         key + "'");
      for (auto pa = aliases.begin(); pa != aliases.end(); ++pa) {
        tfel::raise_if(!names.insert(*pa).second,
                       "GlossaryTokenizer::execute: "
                       "name '" +
                           *pa +
                           "' already used. "
                           "Error while defining key '" +
                           key + "'");
      }
      aliases.push_back(name);
      this->insert(GlossaryEntry(key, aliases, unit, type, short_description,
                                 description, notes));
    }
  }  // end of execute

 private:
  void readBlock(std::vector<std::string>& b,
                 CxxTokenizer::TokensContainer::const_iterator& p,
                 const CxxTokenizer::TokensContainer::const_iterator pe) {
    using namespace tfel::utilities;
    CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::readBlock", "{", p,
                                     pe);
    CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::readBlock", p, pe);
    while (p->value != "}") {
      const auto p2 = p;
      b.push_back(CxxTokenizer::readString(p, pe));
      CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::readBlock", p, pe);
      if (p->value != "}") {
        CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::readBlock", ",", p,
                                         pe);
        CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::readBlock", p, pe);
        tfel::raise_if(p->flag != Token::String,
                       "GlossaryTokenizer::readBlock: "
                       "unpected a string, read '" +
                           p->value + "'");
      }
    }
  }

};  // end of struct GlossaryTokenizer

std::string serialize(const std::vector<std::string>& v, const std::string& d) {
  std::ostringstream out;
  if (v.empty()) {
    out << "\"\" /* no '" << d << "' defined */";
  } else {
    for (auto p = v.begin(); p != v.end(); ++p) {
      if (p != v.begin()) {
        out << "\"@^separator^@\"\n";
      }
      out << "\"" << *p << "\"";
    }
  }
  return out.str();
}

void generateCxxOutput(const GlossaryTokenizer& tokenizer) {
  using namespace tfel::utilities;
  std::ofstream header("Glossary.hxx");
  std::ofstream src("Glossary.cxx");
  tfel::raise_if(!header,
                 "generate_glossary: "
                 "can't open file 'Glossary.hxx'");
  tfel::raise_if(!src,
                 "generate_glossary: "
                 "can't open file 'Glossary.cxx'");
  // gathering all names
  std::vector<std::string> names;
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    const auto& n = p->getNames();
    names.insert(names.end(), n.begin(), n.end());
  }
  // writting headers
  header
      << "/*!\n"
      << " * \\file   Glossary.hxx\n"
      << " * \\author Thomas Helfer\n"
      << " * \\date   09/06/14\n"
      << " * \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights\n"
      << " * reserved.\n"
      << " * This project is publicly released under either the GNU GPL "
         "Licence\n"
      << " * or the CECILL-A licence. A copy of thoses licences are delivered\n"
      << " * with the sources of TFEL. CEA or EDF may also distribute this\n"
      << " * project under specific licensing conditions.\n"
      << " */\n"
      << '\n'
      << "#ifndef TFEL_GLOSSARY_GLOSSARY_HXX\n"
      << "#define TFEL_GLOSSARY_GLOSSARY_HXX\n"
      << '\n'
      << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
      << "#include<set>" << '\n'
      << "#include<vector>\n"
      << "#include<string>\n"
      << '\n'
      << "#include\"TFEL/Glossary/Forward/Glossary.hxx\"\n"
      << '\n'
      << "namespace tfel\n"
      << "{\n"
      << "namespace glossary\n"
      << "{\n"
      << '\n'
      << "/*!\n"
      << " * \\brief Structure in charge of handling the TFEL Glossary\n"
      << " */\n"
      << "struct TFELGLOSSARY_VISIBILITY_EXPORT Glossary\n"
      << "{\n"
      << '\n'
      << "/*!\n"
      << " * \\return the uniq instance of this class (singleton pattern)\n"
      << " */\n"
      << "static Glossary&\n"
      << "getGlossary();\n"
      << '\n';
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    header << "static const GlossaryEntry " << p->getKey() << ";\n";
  }
  header << '\n'
         << "/*!\n"
         << " * \\return true if the glossary contains the given name or key\n"
         << " * \\param[in] n: name\n"
         << " */\n"
         << "bool\n"
         << "contains(const std::string&) const;\n"
         << "/*!\n"
         << " * \\return the glossary entry associated with the given name or "
            "key\n"
         << " * \\param[in] n: name\n"
         << " */\n"
         << "const GlossaryEntry&\n"
         << "getGlossaryEntry(const std::string&) const;\n"
         << "/*!\n"
         << " * \\return all the registred keys\n"
         << " */\n"
         << "const std::vector<std::string>&\n"
         << "getKeys() const;\n"
         << '\n'
         << "protected :\n";
  if (!names.empty()) {
    header << "//! all glossary names (to initialise glossary entries)\n"
           << "static const char * names[" << names.size() << "];\n";
  }
  header << "/*!\n"
         << " * \\brief insert a new entry\n"
         << " */\n"
         << "void\n"
         << "insert(const GlossaryEntry&);\n"
         << '\n'
         << "Glossary();\n"
         << '\n'
         << "Glossary(const Glossary&);\n"
         << '\n'
         << "Glossary&\n"
         << "operator=(const Glossary&);\n"
         << '\n'
         << "/*!\n"
         << "\\return an iterator to the glossary associated with the given "
            "name or key. \n"
         << "Return this->entries.end() if no matching entry is found. \n"
         << "\\param[in] n: name or key.\n"
         << "*/\n"
         << "std::set<GlossaryEntry>::const_iterator\n"
         << "findGlossaryEntry(const std::string&) const;\n"
         << '\n'
         << "//! list of all registred entries\n"
         << "std::set<GlossaryEntry> entries;\n"
         << '\n'
         << "//! list of all registred keys\n"
         << "std::vector<std::string> keys;\n"
         << '\n'
         << "}; // end of struct Glossary\n"
         << '\n'
         << "} // end of namespace glossary\n"
         << '\n'
         << "} // end of namespace tfel\n"
         << '\n'
         << "#endif /* TFEL_GLOSSARY_GLOSSARY_HXX */\n";
  /* writting src file */
  src << "/*!\n"
      << " * \\file   Glossary.cxx\n"
      << " * \\author Thomas Helfer\n"
      << " * \\date   09/06/14\n"
      << " * \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights\n"
      << " * reserved.\n"
      << " * This project is publicly released under either the GNU GPL "
         "Licence\n"
      << " * or the CECILL-A licence. A copy of thoses licences are delivered\n"
      << " * with the sources of TFEL. CEA or EDF may also distribute this\n"
      << " * project under specific licensing conditions.\n"
      << " */\n"
      << '\n'
      << "#include<stdexcept>\n"
      << "#include<algorithm>\n"
      << '\n'
      << "#include\"TFEL/Raise.hxx\"\n"
      << "#include\"TFEL/Glossary/Glossary.hxx\"\n"
      << "#include\"TFEL/Glossary/GlossaryEntry.hxx\"\n"
      << '\n'
      << "namespace tfel\n"
      << "{\n"
      << "namespace glossary\n"
      << "{\n"
      << '\n';
  if (!names.empty()) {
    src << "const char* Glossary::names[" << names.size() << "] = {\n";
    for (auto pn = names.begin(); pn != names.end();) {
      src << '\"' << *pn << '\"';
      if (++pn != names.end()) {
        src << ",\n";
      }
    }
    src << "};\n";
  }
  std::vector<std::string>::size_type pos = 0;
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    const auto& d = replace_all(serialize(p->getDescription(), "description"),
                                "\\", "\\\\");
    const auto& n =
        replace_all(serialize(p->getNotes(), "notes"), "\\", "\\\\");
    const auto& names = p->getNames();
    src << "const GlossaryEntry Glossary::" << p->getKey() << "("
        << "\"" << replace_all(p->getKey(), "\\", "\\\\") << "\",";
    if (names.size() == 1u) {
      src << '\"' << names[0u] << "\",\n";
    } else {
      if (pos == 0) {
        src << "Glossary::names,Glossary::names+" << names.size() << ",\n";
      } else {
        src << "Glossary::names+" << pos << ",Glossary::names+"
            << pos + names.size() << ",\n";
      }
    }
    pos += names.size();
    src << "\"" << replace_all(p->getUnit(), "\\", "\\\\") << "\","
        << "\"" << replace_all(p->getType(), "\\", "\\\\") << "\",\n"
        << "\"" << replace_all(p->getShortDescription(), "\\", "\\\\")
        << "\",\n"
        << d << ",\n"
        << n << ");\n\n";
  }
  src << "Glossary&\n"
      << "Glossary::getGlossary()\n"
      << "{\n"
      << "static Glossary glossary;\n"
      << "return glossary;\n"
      << "} // end of Glossary::getGlossary\n"
      << '\n'
      << "Glossary::Glossary()"
      << "{\n";
  if (!tokenizer.empty()) {
    src << "this->keys.reserve(" << tokenizer.size() << ");\n";
  }
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    src << "this->insert(Glossary::" << p->getKey() << ");\n";
  }
  src << "} // end of Glossary::Glossary\n"
      << '\n'
      << "void Glossary::insert(const GlossaryEntry& e)\n"
      << "{\n"
      << "tfel::raise_if(!this->entries.insert(e).second,\n"
      << "               \"Glossary::insert: \"\n"
      << "               \"'\"+e.getKey()+\"' already declared\");\n"
      << "this->keys.push_back(e.getKey());\n"
      << "} // end of Glossary::insert\n"
      << '\n'
      << "bool Glossary::contains(const std::string& n) const\n"
      << "{\n"
      << "return this->findGlossaryEntry(n)!=this->entries.end();\n"
      << "} // end of Glossary::contains\n"
      << '\n'
      << "const GlossaryEntry&\n"
      << "Glossary::getGlossaryEntry(const std::string& n) const\n"
      << "{\n"
      << "const auto p=this->findGlossaryEntry(n);\n"
      << "tfel::raise_if(p==this->entries.end(),\n"
      << "\"Glossary::getGlossaryEntry: no glossary entry matching "
         "'\"+n+\"'\");\n"
      << "return *p;\n"
      << "} // end of Glossary::contains\n"
      << '\n'
      << "const std::vector<std::string>&\n"
      << "Glossary::getKeys() const\n"
      << "{\n"
      << "return this->keys;\n"
      << "} // end of Glossary::contains\n"
      << '\n'
      << "std::set<GlossaryEntry>::const_iterator\n"
      << "Glossary::findGlossaryEntry(const std::string& n) const\n"
      << "{\n"
      << "using namespace std;\n"
      << "set<GlossaryEntry>::const_iterator p;\n"
      << "for(p=this->entries.begin();p!=this->entries.end();++p){\n"
      << "if(p->getKey()==n){\n"
      << "return p;\n"
      << "}\n"
      << "const auto& enames = p->getNames();\n"
      << "if(find(enames.begin(),enames.end(),n)!=enames.end()){\n"
      << "return p;\n"
      << "}\n"
      << "}\n"
      << "return this->entries.end();\n"
      << "} // end of Glossary::findGlossaryEntry\n"
      << '\n'
      << "} // end of namespace glossary\n"
      << '\n'
      << "} // end of namespace tfel\n";
}

void generatePleiadesCxxOutput(const GlossaryTokenizer& tokenizer) {
  using namespace tfel::utilities;
  std::ofstream header("Glossary.hh");
  std::ofstream src("Glossary.cpp");
  tfel::raise_if(!header,
                 "generate_glossary: "
                 "can't open file 'Glossary.hh'");
  tfel::raise_if(!src,
                 "generate_glossary: "
                 "can't open file 'Glossary.cpp'");
  // gathering all names
  std::vector<std::string> names;
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    const auto& n = p->getNames();
    names.insert(names.end(), n.begin(), n.end());
  }
  // writting headers
  header << "/*!\n"
         << " * \\file   Glossary.hh\n"
         << " * \\author auto generated\n"
         << " * \\date   2014-07-22\n"
         << "*/\n"
         << '\n'
         << "#ifndef PLEIADES_GLOSSARY_GLOSSARY_HXX\n"
         << "#define PLEIADES_GLOSSARY_GLOSSARY_HXX\n"
         << '\n'
         << "#include <set>" << '\n'
         << "#include <vector>\n"
         << "#include <string>\n"
         << '\n'
         << "#include \"Pleiades/Metier/Glossary/GlossaryEntry.hh\"\n"
         << '\n'
         << "namespace Pleiades {\n"
         << '\n'
         << "/*!\n"
         << " * \\brief Class in charge of handling the Pleiades Glossary\n"
         << " */\n"
         << "class  Glossary {\n"
         << "  public:\n"
         << '\n';

  header
      << "  /*!\n"
      << "   * \\return the uniq instance of this class (singleton pattern)\n"
      << "   */\n"
      << "  static Glossary& getGlossary();\n"
      << '\n';
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    header << "  static const GlossaryEntry " << p->getKey() << ";\n";
  }
  header << '\n'
         << "  /*!\n"
         << "   * \\return true if the glossary contains the given name\n"
         << "   * \\param[in] n: name\n"
         << "   */\n"
         << "  bool contains(const std::string&) const;\n"
         << '\n'
         << "  /*!\n"
         << "   * \\return all the registred keys\n"
         << "   */\n"
         << "  const std::vector<std::string>& getKeys() const;\n"
         << '\n'
         << "  protected:\n"
         << '\n';
  if (!names.empty()) {
    header << "  //! all glossary names (to initialise glossary entries)\n"
           << "  static const char * names[" << names.size() << "];\n";
  }
  header << '\n'
         << "  /*!\n"
         << "   * \\brief insert a new entry\n"
         << "   */\n"
         << "  void insert(const GlossaryEntry&);\n"
         << '\n'
         << "  Glossary();\n"
         << '\n'
         << "  Glossary(const Glossary&);\n"
         << '\n'
         << "  Glossary& operator=(const Glossary&);\n"
         << '\n'
         << "  std::set<GlossaryEntry> entries;\n"
         << '\n'
         << "  std::vector<std::string> keys;\n"
         << '\n'
         << "}; // end of class Glossary\n"
         << "} // end of namespace Pleiades\n"
         << '\n'
         << "#endif /* PLEIADES_GLOSSARY_GLOSSARY_HXX */\n";

  /* writting src file */
  src << "/*!\n"
      << " *\\file   Glossary.cpp\n"
      << " *\\author auto generated\n"
      << " *\\date   2014-07-22\n"
      << "*/\n"
      << '\n'
      << "#include <stdexcept>\n"
      << "#include <algorithm>\n"
      << '\n'
      << "#include \"Pleiades/Metier/Glossary/Glossary.hh\"\n"
      << '\n'
      << "namespace Pleiades { \n"
      << '\n';
  if (!names.empty()) {
    src << "const char* Glossary::names[" << names.size() << "] = {\n";
    for (auto pn = names.begin(); pn != names.end();) {
      src << '\"' << *pn << '\"';
      if (++pn != names.end()) {
        src << ",\n";
      }
    }
    src << "};\n";
  }
  src << '\n';

  std::vector<std::string>::size_type pos = 0;
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    const auto& d = replace_all(serialize(p->getDescription(), "description"),
                                "\\", "\\\\");
    const auto& n =
        replace_all(serialize(p->getNotes(), "notes"), "\\", "\\\\");
    const auto& names = p->getNames();
    src << "const GlossaryEntry Glossary::" << p->getKey() << "("
        << "\"" << replace_all(p->getKey(), "\\", "\\\\") << "\",";
    if (names.size() == 1u) {
      src << '\"' << names[0u] << "\",\n";
    } else {
      if (pos == 0) {
        src << "Glossary::names,Glossary::names+" << names.size() << ",\n";
      } else {
        src << "Glossary::names+" << pos << ",Glossary::names+"
            << pos + names.size() << ",\n";
      }
    }
    pos += names.size();
    src << "\"" << replace_all(p->getUnit(), "\\", "\\\\") << "\","
        << "\"" << replace_all(p->getType(), "\\", "\\\\") << "\",\n"
        << "\"" << replace_all(p->getShortDescription(), "\\", "\\\\")
        << "\",\n"
        << d << ",\n"
        << n << ");\n\n";
  }

  src << "Glossary& Glossary::getGlossary() {\n"
      << "  static Glossary glossary;\n"
      << "  return glossary;\n"
      << "} // end of Glossary::getGlossary\n"
      << '\n';

  src << "Glossary::Glossary() {";
  if (!tokenizer.empty()) {
    src << '\n' << "  this->keys.reserve(" << tokenizer.size() << ");\n";
  }
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    src << "  this->insert(Glossary::" << p->getKey() << ");\n";
  }
  src << "} // end of Glossary::Glossary\n"
      << '\n'
      << "void Glossary::insert(const GlossaryEntry& e) {\n"
      << "  tfel::raise_if(!this->entries.insert(e).second,\n"
      << "                 \"Glossary::insert: \"\n"
      << "                 \"'\"+e.getKey()+\"' already declared\";)\n"
      << "  this->keys.push_back(e.getKey());\n"
      << "} // end of Glossary::insert\n"
      << '\n'
      << "bool Glossary::contains(const std::string& w) const {\n"
      << "  for(const auto& e :this->entries){\n"
      << "    const auto& n = e.getNames();\n"
      << "    if(find(n.begin(),n.end(),w)!=n.end()){\n"
      << "      return true;\n"
      << "    }\n"
      << "  }\n"
      << "  return false;\n"
      << "} // end of Glossary::contains\n"
      << '\n'
      << "const std::vector<std::string>& Glossary::getKeys() const {\n"
      << "  return this->keys;\n"
      << "} // end of Glossary::contains\n"
      << '\n'
      << "} // end of namespace Pleiades\n";
}

void generateBoostPythonBindings(const GlossaryTokenizer& tokenizer) {
  using namespace tfel::utilities;
  std::ofstream psrc("PythonGlossary.cxx");
  tfel::raise_if(!psrc,
                 "generate_glossary: "
                 "can't open file 'PythonGlossary.cxx'");
  // python bindings
  psrc
      << "/*!\n"
      << " * \\file   PythonGlossary.cxx\n"
      << " * \\author Thomas Helfer\n"
      << " * \\date   09/06/14\n"
      << " * \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights\n"
      << " * reserved.\n"
      << " * This project is publicly released under either the GNU GPL "
         "Licence\n"
      << " * or the CECILL-A licence. A copy of thoses licences are delivered\n"
      << " * with the sources of TFEL. CEA or EDF may also distribute this\n"
      << " * project under specific licensing conditions.\n"
      << " */\n"
      << '\n'
      << "#include<boost/python.hpp>\n"
      << '\n'
      << "#include\"TFEL/Glossary/Glossary.hxx\"\n"
      << "#include\"TFEL/Glossary/GlossaryEntry.hxx\"\n"
      << '\n'
      << "void\ndeclareGlossary()\n"
      << "{\n"
      << "using namespace boost;\n"
      << "using namespace boost::python;\n"
      << "using namespace tfel::glossary;\n"
      << "class_<Glossary,noncopyable>(\"Glossary\",no_init)\n"
      << ".def(\"getGlossary\",Glossary::getGlossary,\n"
      << "return_value_policy<reference_existing_object>())\n"
      << ".staticmethod(\"getGlossary\")\n"
      << ".def(\"contains\",&Glossary::contains)\n";
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    psrc << ".def_readonly(\"" << p->getKey()
         << "\",&Glossary::" << p->getKey();
    const auto& d = replace_all(p->getShortDescription(), "\\", "\\\\");
    if (!d.empty()) {
      psrc << ",\n\"" << d << "\"";
    }
    psrc << ")\n";
  }
  psrc << ";\n\n"
       << "}\n";
}  // end of generateBoostPythonBindings

void generateXMLOutput(const GlossaryTokenizer& tokenizer) {
  std::ofstream xml("glossary.xml");
  tfel::raise_if(!xml,
                 "generate_glossary: "
                 "can't open file 'glossary.xml'");
  xml << "<?xml version=\"1.0\"?>\n"
      << "<glossary>\n";
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    const auto& n = p->getNames();
    xml << "<glossary_entry key=\"" << p->getKey() << "\">\n";
    for (auto pn = n.begin(); pn != n.end(); ++pn) {
      xml << "<name>" << *pn << "</name>\n";
    }
    xml << "<unit>" << p->getUnit() << "</unit>\n"
        << "<type>" << p->getType() << "</type>\n"
        << "<short_description>" << p->getShortDescription()
        << "</short_description>\n"
        << "<description>\n"
        << "<![CDATA[\n";
    const auto& d = p->getDescription();
    for (auto p2 = d.begin(); p2 != d.end(); ++p2) {
      xml << *p2 << '\n';
    }
    xml << "]]>"
        << "\n</description>\n"
        << "</glossary_entry>\n";
    ;
  }
  xml << "</glossary>\n";
}

void generatePandocOutput(const GlossaryTokenizer& tokenizer) {
  std::ofstream doc("glossary-pandoc.txt");
  tfel::raise_if(!doc,
                 "generatePandocOutput: "
                 "can't open file 'glossary-pandoc.txt'");
  doc << "% TFEL Glossary description\n"
      << "% Helfer Thomas; Bernaud Stéphane\n"
      << '\n'
      << "\\newcommand{\\dtot}{\\mathrm{d}}\n"
      << "\\newcommand{\\paren}[1]{{\\displaystyle \\left(#1\\right)}}\n"
      << "\\newcommand{\\average}[1]{{\\displaystyle "
         "\\left\\langle#1\\right\\rangle}}\n"
      << "\\newcommand{\\Frac}[2]{{\\displaystyle \\frac{\\displaystyle "
         "#1}{\\displaystyle #2}}}\n"
      << "\\newcommand{\\deriv}[2]{{\\displaystyle \\frac{\\displaystyle "
         "\\partial #1}{\\displaystyle \\partial #2}}}\n";
  for (auto p = tokenizer.begin(); p != tokenizer.end(); ++p) {
    const auto& n = p->getNames();
    doc << '\n';
    doc << "# L'entrée " << p->getKey() << '\n' << '\n';
    if (!p->getShortDescription().empty()) {
      doc << "Cette entrée décrit " << p->getShortDescription() << ".\n"
          << '\n';
    }
    doc << "* noms : ";
    for (auto pn = n.begin(); pn != n.end();) {
      doc << *pn;
      if (++pn != n.end()) {
        doc << ", ";
      }
    }
    doc << '\n';
    doc << "* unité: ";
    if (!p->getUnit().empty()) {
      doc << "\\(" << p->getUnit() << "\\)\n";
    } else {
      doc << "sans unité\n";
    }
    if (p->getType() == "scalar") {
      doc << "* type: scalaire " << '\n';
    } else if (p->getType() == "vector") {
      doc << "* type: vecteur " << '\n';
    } else if (p->getType() == "tensor") {
      doc << "* type: tenseur symétrique\n";
    } else {
      doc << "* type: unsupported type\n";
    }
    const auto& d = p->getDescription();
    if (!d.empty()) {
      doc << '\n' << "## Description\n\n";
    }
    for (auto p2 = d.begin(); p2 != d.end(); ++p2) {
      doc << *p2 << '\n';
    }
    const auto& notes = p->getNotes();
    if (!notes.empty()) {
      doc << '\n' << "## Notes \n\n";
    }
    for (auto p2 = notes.begin(); p2 != notes.end(); ++p2) {
      doc << *p2 << '\n';
    }
  }
  doc << "<!-- Local IspellDict: english -->\n";
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main(int argc, char* argv[]) {
  GlossaryTokenizer tokenizer;
  tokenizer.execute("glossary.txt");
  if (argc == 1) {
    generateCxxOutput(tokenizer);
    generateBoostPythonBindings(tokenizer);
    generateXMLOutput(tokenizer);
    generatePandocOutput(tokenizer);
  } else if ((argc == 2) && (std::string(argv[1]) == "--pleiades")) {
    generatePleiadesCxxOutput(tokenizer);
  } else {
    tfel::raise("Usage: " + std::string(argv[0]) + " [--pleiades]");
  }
}  // end of main
