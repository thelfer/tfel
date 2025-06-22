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

// g++ -std=c++20 `tfel-config --includes --libs --utilities --glossary`
// generate_glossary.cxx -o generate_glossary

// g++ -std=c++20 generate_glossary.cxx  -o generate_glossary `tfel-config
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
#include <optional>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

static std::map<std::string, std::string> readPhysicalBoundsMap(
    tfel::utilities::CxxTokenizer::const_iterator& p,
    const tfel::utilities::CxxTokenizer::const_iterator pe) {
  using CxxTokenizer = tfel::utilities::CxxTokenizer;
  auto throw_if = [](const bool b, const std::string& msg) {
    if (b) {
      tfel::raise("readPhysicalBoundsMap: " + msg);
    };
  };
  auto is_convertible = [throw_if](const std::string& s) {
    throw_if(s.empty(), "empty string");
    std::size_t p;
    double r;
    try {
      r = std::stod(s, &p);
    } catch (std::exception&) {
      return false;
    }
    if (p != s.size()) {
      return false;
    }
    return true;
  };
  std::map<std::string, std::string> bounds;
  CxxTokenizer::checkNotEndOfLine("readPhysicalBoundsMap", p, pe);
  CxxTokenizer::readSpecifiedToken("readPhysicalBoundsMap", "{", p, pe);
  CxxTokenizer::checkNotEndOfLine("readPhysicalBoundsMap", p, pe);
  while (p->value != "}") {
    const auto k = [&p, pe]() -> std::string {
      if (p->flag != tfel::utilities::Token::String) {
        auto c = p;
        ++p;
        return c->value;
      }
      return CxxTokenizer::readString(p, pe);
    }();
    CxxTokenizer::readSpecifiedToken("readPhysicalBoundsMap", ":", p, pe);
    CxxTokenizer::checkNotEndOfLine("readPhysicalBoundsMap", p, pe);
    const auto v = p->value;
    throw_if(!is_convertible(v), "invalid bound '" + v + "'");
    ++p;
    CxxTokenizer::checkNotEndOfLine("readPhysicalBoundsMap", p, pe);
    if (p->value == ",") {
      CxxTokenizer::readSpecifiedToken("readPhysicalBoundsMap", ",", p, pe);
      CxxTokenizer::checkNotEndOfLine("readPhysicalBoundsMap", p, pe);
      throw_if(p->value == "}", "unexpected token '}'");
    } else {
      throw_if(p->value != "}", "unexpected token '" + p->value + "'");
    }
    const auto pv = bounds.find(k);
    if (pv == bounds.end()) {
      bounds.insert({k, v});
    } else {
      throw_if(true, "key '" + k + "' multiply defined");
    }
  }
  CxxTokenizer::readSpecifiedToken("readPhysicalBoundsMap", "}", p, pe);
  return bounds;
}  // end of readPhysicalBoundsMap

struct GlossaryEntryData {
  //! \brief main key
  std::string key;
  //! \brief aliases
  std::vector<std::string> aliases;
  //! \brief units of the entry, sorted by unit system
  std::map<std::string, std::string> units;
  //! \brief type of the entry
  std::string type;
  //! \brief short description of the entry
  std::string short_description;
  //! \brief description
  std::vector<std::string> description;
  //! \brief notes
  std::vector<std::string> notes;
  //! \brief lower physical bounds
  std::map<std::string, std::string> lower_physical_bounds;
  //! \brief upper physical bounds
  std::map<std::string, std::string> upper_physical_bounds;
};  // end of GlossaryEntryData

bool operator<(const GlossaryEntryData& e1, const GlossaryEntryData& e2) {
  return e1.key < e2.key;
}

struct GlossaryTokenizer : protected tfel::utilities::CxxTokenizer,
                           protected std::set<GlossaryEntryData> {
  using std::set<GlossaryEntryData>::begin;

  using std::set<GlossaryEntryData>::end;

  using std::set<GlossaryEntryData>::const_iterator;

  using std::set<GlossaryEntryData>::empty;

  using std::set<GlossaryEntryData>::size;

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
      GlossaryEntryData d;
      std::string name;
      d.key = CxxTokenizer::readString(p, pe);
      tfel::raise_if(!this->isValidIdentifier(d.key, false),
                     "GlossaryTokenizer: invalid glossary key '" + d.key + "'");
      tfel::raise_if(!keys.insert(d.key).second,
                     "GlossaryTokenizer::execute: "
                     "key '" +
                         d.key + "' already used");
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
            tfel::raise_if(!d.aliases.empty(),
                           "GlossaryTokenizer::execute: "
                           "aliases already defined for key '" +
                               d.key + "'");
            d.aliases.push_back(CxxTokenizer::readString(p, pe));
          } else if (k == "aliases") {
            tfel::raise_if(!d.aliases.empty(),
                           "GlossaryTokenizer::execute: "
                           "aliases already defined for key '" +
                               d.key + "'");
            this->readBlock(d.aliases, p, pe);
          } else if (k == "units") {
            tfel::raise_if(!d.units.empty(),
                           "GlossaryTokenizer::execute: "
                           "unit already defined for key '" +
                               d.key + "'");
            d.units =
                convert<std::map<std::string, std::string>>(Data::read(p, pe));
          } else if (k == "name") {
            tfel::raise_if(!name.empty(),
                           "GlossaryTokenizer::execute: "
                           "name already defined for key '" +
                               d.key + "'");
            name = CxxTokenizer::readString(p, pe);
          } else if (k == "type") {
            tfel::raise_if(!d.type.empty(),
                           "GlossaryTokenizer::execute: "
                           "type already defined for key '" +
                               d.key + "'");
            d.type = CxxTokenizer::readString(p, pe);
          } else if (k == "short_description") {
            tfel::raise_if(!d.short_description.empty(),
                           "GlossaryTokenizer::execute: "
                           "short description already defined for key '" +
                               d.key + "'");
            d.short_description = CxxTokenizer::readString(p, pe);
          } else if (k == "description") {
            tfel::raise_if(!d.description.empty(),
                           "GlossaryTokenizer::execute: "
                           "description already defined for key '" +
                               d.key + "'");
            this->readBlock(d.description, p, pe);
            CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", "}",
                                             p, pe);
          } else if (k == "notes") {
            tfel::raise_if(!d.notes.empty(),
                           "GlossaryTokenizer::execute: "
                           "notes already defined for key '" +
                               d.key + "'");
            this->readBlock(d.notes, p, pe);
            CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute", "}",
                                             p, pe);
          } else if (k == "lower_physical_bounds") {
            tfel::raise_if(!d.lower_physical_bounds.empty(),
                           "GlossaryTokenizer::execute: "
                           "lower bound already defined for key '" +
                               d.key + "'");
            d.lower_physical_bounds = readPhysicalBoundsMap(p, pe);
          } else if (k == "upper_physical_bounds") {
            tfel::raise_if(!d.upper_physical_bounds.empty(),
                           "GlossaryTokenizer::execute: "
                           "upper bound already defined for key '" +
                               d.key + "'");
            d.upper_physical_bounds = readPhysicalBoundsMap(p, pe);
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
              d.key +
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
      tfel::raise_if(d.short_description.empty(),
                     "GlossaryTokenizer::execute: "
                     "no short_description given for glossary entry '" +
                         d.key + "'");
      tfel::raise_if(d.type.empty(),
                     "GlossaryTokenizer::execute: "
                     "no type given for glossary entry '" +
                         d.key + "'");
      if (name.empty()) {
        name = d.key;
      }
      tfel::raise_if(!names.insert(name).second,
                     "GlossaryTokenizer::execute: "
                     "name '" +
                         name +
                         "' already used. "
                         "Error while defining key '" +
                         d.key + "'");
      for (const auto& a : d.aliases) {
        tfel::raise_if(!names.insert(a).second,
                       "GlossaryTokenizer::execute: "
                       "name '" +
                           a +
                           "' already used. "
                           "Error while defining key '" +
                           d.key + "'");
      }
      d.aliases.push_back(name);
      this->insert(d);
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

static std::string serialize(const std::vector<std::string>& lines) {
  auto r = std::string{};

  for (auto p = lines.begin(); p != lines.end(); ++p) {
    if (p != lines.begin()) {
      r += '\"';
      r += tfel::glossary::GlossaryEntry::separator;
      r += "\"\n";
    }
    r += "\"" + *p + "\"";
  }
  return r;
}

static std::string serialize(const std::vector<std::string>& lines,
                             const std::string& d) {
  if (lines.empty()) {
    return "\"\" /* no '" + d + "' defined */";
  }
  return serialize(lines);
}

static std::string serialize(const std::map<std::string, std::string>& m) {
  auto check = [](const std::string& s) {
    const auto& sep = tfel::glossary::GlossaryEntry::separator;
    if ((s.find(sep) != std::string::npos) ||
        ((s.find(':') != std::string::npos))) {
      tfel::raise("serialize: invalid key or value '" + s + "'");
    }
  };
  std::ostringstream out;
  for (auto p = m.begin(); p != m.end(); ++p) {
    if (p != m.begin()) {
      out << tfel::glossary::GlossaryEntry::separator;
    }
    check(p->first);
    check(p->second);
    out << p->first << ':' << p->second;
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
    const auto& n = p->aliases;
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
      << "#ifndef LIB_TFEL_GLOSSARY_GLOSSARY_HXX\n"
      << "#define LIB_TFEL_GLOSSARY_GLOSSARY_HXX\n"
      << '\n'
      << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
      << "#include<set>" << '\n'
      << "#include<vector>\n"
      << "#include<string>\n"
      << '\n'
      << "#include\"TFEL/Glossary/Forward/Glossary.hxx\"\n"
      << '\n'
      << "namespace tfel::glossary {\n"
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
    header << "static const GlossaryEntry " << p->key << ";\n";
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
  header
      << "/*!\n"
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
      << " * \\return an iterator to the glossary associated with the given \n"
      << " * name or key. If no matching key exists, this->entries.end() is \n"
      << " * returned. \n"
      << " * \\param[in] n: name or key.\n"
      << "*/\n"
      << "std::set<GlossaryEntry>::const_iterator\n"
      << "findGlossaryEntry(const std::string&) const;\n"
      << '\n'
      << "//! \brief list of all registred entries\n"
      << "std::set<GlossaryEntry> entries;\n"
      << '\n'
      << "//! \brief list of all registred keys\n"
      << "std::vector<std::string> keys;\n"
      << '\n'
      << "}; // end of struct Glossary\n"
      << '\n'
      << "} // end of namespace tfel::glossary\n"
      << '\n'
      << "#endif /* LIB_TFEL_GLOSSARY_GLOSSARY_HXX */\n";
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
      << "namespace tfel::glossary{\n"
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
  for (const auto& t : tokenizer) {
    const auto& d =
        replace_all(serialize(t.description, "description"), "\\", "\\\\");
    const auto& n = replace_all(serialize(t.notes, "notes"), "\\", "\\\\");
    const auto& names = t.aliases;
    src << "const GlossaryEntry Glossary::" << t.key << "("
        << "\"" << replace_all(t.key, "\\", "\\\\") << "\",";
    if (pos == 0) {
      src << "Glossary::names,Glossary::names+" << names.size() << ",\n";
    } else {
      src << "Glossary::names+" << pos << ",Glossary::names+"
          << pos + names.size() << ",\n";
    }
    pos += names.size();
    src << "\"" << serialize(t.units) << "\","
        << "\"" << replace_all(t.type, "\\", "\\\\") << "\",\n"
        << "\"" << replace_all(t.short_description, "\\", "\\\\") << "\",\n"
        << d << ",\n"
        << n << ", \"" << serialize(t.lower_physical_bounds) << "\", \""
        << serialize(t.upper_physical_bounds) << "\");\n\n";
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
    src << "this->insert(Glossary::" << p->key << ");\n";
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
      << "} // end of namespace tfel::glossary\n";
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
    const auto& n = p->aliases;
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
    header << "  static const GlossaryEntry " << p->key << ";\n";
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
    const auto& d =
        replace_all(serialize(p->description, "description"), "\\", "\\\\");
    const auto& n = replace_all(serialize(p->notes, "notes"), "\\", "\\\\");
    const auto& names = p->aliases;
    src << "const GlossaryEntry Glossary::" << p->key << "("
        << "\"" << replace_all(p->key, "\\", "\\\\") << "\",";
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
    src << "\"" << serialize(p->units) << "\","
        << "\"" << replace_all(p->type, "\\", "\\\\") << "\",\n"
        << "\"" << replace_all(p->short_description, "\\", "\\\\") << "\",\n"
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
    src << "  this->insert(Glossary::" << p->key << ");\n";
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
    psrc << ".def_readonly(\"" << p->key << "\",&Glossary::" << p->key;
    const auto& d = replace_all(p->short_description, "\\", "\\\\");
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
    const auto& n = p->aliases;
    xml << "<glossary_entry key=\"" << p->key << "\">\n";
    for (auto pn = n.begin(); pn != n.end(); ++pn) {
      xml << "<name>" << *pn << "</name>\n";
    }
    xml << "<unit>" << serialize(p->units) << "</unit>\n"
        << "<type>" << p->type << "</type>\n"
        << "<short_description>" << p->short_description
        << "</short_description>\n"
        << "<description>\n"
        << "<![CDATA[\n";
    const auto& d = p->description;
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
  doc << "---\n"
      << "title: Description of the TFEL glossary\n"
      << "author: Helfer Thomas, Maxence Wangermez, Bernaud Stéphane\n"
      << "date: 21/11/2017\n"
      << "lang: en-EN\n"
      << "link-citations: true\n"
      << "colorlinks: true\n"
      << "geometry:\n"
      << "  - margin=2cm\n"
      << "papersize: a4\n"
      << "figPrefixTemplate: \"$$i$$\"\n"
      << "tblPrefixTemplate: \"$$i$$\"\n"
      << "secPrefixTemplate: \"$$i$$\"\n"
      << "eqnPrefixTemplate: \"($$i$$)\"\n"
      << "---\n"
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
    const auto& n = p->aliases;
    doc << '\n';
    doc << "# The `" << p->key << "` entry\n\n";
    if (!p->short_description.empty()) {
      doc << "This entry describes " << p->short_description << ".\n" << '\n';
    }
    if (n.size() != 1u) {
      doc << "* alternative names: ";
      for (auto pn = n.begin(); pn != n.end();) {
        doc << *pn;
        if (++pn != n.end()) {
          doc << ", ";
        }
      }
    }
    doc << '\n' << "* units:\n";
    if (!p->units.empty()) {
      for (const auto& su : p->units) {
        doc << "  *" << su.first << ": " << su.second << '\n';
      }
    } else {
      doc << "no units specified\n";
    }
    doc << "* type: ";
    if (p->type.empty()) {
      doc << "unspecified\n";
    } else {
      doc << p->type << "\n";
    }
    const auto& d = p->description;
    if (!d.empty()) {
      doc << '\n' << "## Description\n\n";
    }
    for (auto p2 = d.begin(); p2 != d.end(); ++p2) {
      doc << *p2 << '\n';
    }
    const auto& notes = p->notes;
    if (!p->notes.empty()) {
      doc << '\n' << "## Notes \n\n";
      for (const auto& n : p->notes) {
        doc << n << '\n';
      }
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
