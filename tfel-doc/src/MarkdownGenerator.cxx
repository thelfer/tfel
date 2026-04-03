/*!
 * \file  tfel-doc/src/MarkdownConvertion.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <ctime>
#include <chrono>
#include <fstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Global.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Utilities/GeneratorOptions.hxx"
#include "TFEL/Utilities/TestDocumentation.hxx"
#include "TFEL/Utilities/MarkdownGenerator.hxx"

namespace tfel::utilities {

  static void writeMarkdownHeader(
      std::ostream& log,
      const std::map<std::string, std::vector<TestDocumentation>>& tests,
      const std::string& l) {
    auto authors = std::set<std::string>{};
    log << "% " << capitalize(getTranslation("tests documentation", l)) << '\n';
    for (const auto& tc : tests) {
      for (const auto& t : tc.second) {
        if (!t.author.empty()) {
          authors.insert(t.author);
        }
      }
    }
    log << "% ";
    for (auto pa = authors.begin(); pa != authors.end();) {
      log << *pa;
      if (++pa != authors.end()) {
        log << ", ";
      }
    }
    auto now = std::chrono::system_clock::now();
    auto ttp = std::chrono::system_clock::to_time_t(now);
    log << "\n% " << std::ctime(&ttp) << "\n";
  }

  static std::string getIndexKey(const std::string& s) {
    if (s.empty()) {
      return "";
    }
    if (s[0] == '!') {
      return s.substr(1);
    }
    return s;
  }

  static void writeMarkdownDescription(std::ostream& out,
                                       const TestDocumentation& t,
                                       const GeneratorOptions& o) {
    const auto& l = o.lang;
    out << "## Test " << t.name << "\n\n";
    if (!t.author.empty()) {
      out << "- " << capitalize(getTranslation("author", l)) << " : "
          << t.author << "\n";
    }
    if (!t.date.empty()) {
      out << "- " << capitalize(getTranslation("date", l)) << " : " << t.date
          << "\n";
    }
    if (!t.src.empty()) {
      out << "- " << capitalize(getTranslation("path in source", l)) << " : "
          << t.src << "\n";
    }
    if (!t.install.empty()) {
      out << "- " << capitalize(getTranslation("path in install", l)) << " : "
          << t.install << "\n";
    }
    if (!t.keys.empty()) {
      out << "- " << capitalize(getTranslation("keywords", l)) << ":\n";
      for (const auto& key : t.keys) {
        out << "    + " << getTranslation(getIndexKey(key.first), l);
        if (!key.second.empty()) {
          out << ": ";
          for (auto pk = key.second.cbegin(); pk != key.second.cend();) {
            out << getTranslation(getIndexKey(*pk), l);
            if (++pk != key.second.cend()) {
              out << ", ";
            }
          }
        }
        out << '\n';
      }
    }
    out << "\n";
    if (!t.models.empty()) {
      if (t.models.size() == 1) {
        const auto& m = *(t.models.begin());
        out << "### " << getTranslation("Tested model", l) << "\n\n"
            << getTranslation("The model tested is:", l) << '`' << m.first
            << "`.\n\n";
      } else {
        out << "### " << getTranslation("List of tested models", l) << "\n\n"
            << " " << getTranslation("The models tested are:", l) << "\n\n";
        for (const auto& m : t.models) {
          out << "- `" << m.first.front() << "`\n";
        }
        out << '\n';
      }
    }
    out << "\n### " << capitalize(getTranslation("description", l)) << "\n\n";
    const auto pd = t.descriptions.find(l);
    if (pd != t.descriptions.end()) {
      out << pd->second << "\n\n";
    } else {
      out << capitalize(getTranslation("no description available", l))
          << "\n\n";
    }
  }  // end of writeMarkdownDescription

  static void writeMarkdownSection(std::ostream& out,
                                   const std::string& s,
                                   const std::vector<TestDocumentation>& tests,
                                   const GeneratorOptions& o) {
    out << "# " << capitalize(getTranslation("text in category", o.lang)) << " "
        << s << "\n\n";
    for (const auto& td : tests) {
      writeMarkdownDescription(out, td, o);
    }
  }

  void writeMarkdownFile(
      std::ostream& out,
      const std::map<std::string, std::vector<TestDocumentation>>& tests,
      const GeneratorOptions& o) {
    if (!(o.split) && (o.standAlone)) {
      writeMarkdownHeader(out, tests, o.lang);
    }
    for (const auto& td : tests) {
      if (o.split) {
        const auto& tf = replace_all(td.first, ' ', '_') + ".tex";
        const auto& file = o.outputDirectory + "/" + tf;
        std::ofstream f(file);
        raise_if(!f, "writeMarkdownFile : can't open file '" + file + "'");
        writeMarkdownSection(f, td.first, td.second, o);
      } else {
        writeMarkdownSection(out, td.first, td.second, o);
      }
    }
  }

}  // end of namespace tfel::utilities
