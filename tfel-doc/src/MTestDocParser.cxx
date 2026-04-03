/*!
 * \file  tfel-doc/src/MTestDocParser.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 28 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>

#include "MTest/MTestParser.hxx"
#include "TFEL/Utilities/MTestDocParser.hxx"

namespace tfel::utilities {

  MTestDocParser::MTestDocParser(const std::string& f) : file(f) {
    this->readInputFile(f, {}, {});
  }  // end of MTestDocParser::MTestDocParser

  void MTestDocParser::addDocumentation(
      std::map<std::string, std::vector<TestDocumentation>>& r) {
    const auto c = "Mechanical behaviour unary testing";
    auto ptest = r.find(c);
    if (r.find(c) == r.end()) {
      ptest = r.insert({c, std::vector<TestDocumentation>()}).first;
    }
    ptest->second.emplace_back(TestDocumentation());
    auto& t = ptest->second.back();
    t.name = this->file;
    t.date = this->date;
    t.author = this->author;
    t.descriptions.insert({"english", this->description});
  }  // end of MTestDocParser::execute

  MTestDocParser::~MTestDocParser() = default;

}  // end of namespace tfel::utilities
