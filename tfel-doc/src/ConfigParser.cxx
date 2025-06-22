/*!
 * \file  tfel-doc/src/ConfigParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>

#include "TFEL/Utilities/ConfigParser.hxx"

namespace tfel {

  namespace utilities {

    static void insert(
        std::map<std::string, std::map<std::string, std::string>>& m,
        const std::string& n1) {
      using namespace std;
      if (!m["english"].insert({n1, n1}).second) {
        string msg("insert : ");
        msg += "category '" + n1 + "' already declared";
        throw(runtime_error(msg));
      }
    }  // end of insert

    static void insert(
        std::map<std::string, std::map<std::string, std::string>>& m,
        const std::string& n1,
        const std::string& n2,
        const std::string& l) {
      using namespace std;
      if (m["english"].find(n1) == m["english"].end()) {
        string msg("insert : ");
        msg += "category '" + n1 + "' undeclared";
        throw(runtime_error(msg));
      }
      if (!m[l].insert({n1, n2}).second) {
        string msg("insert : ");
        msg += "category '" + n1 + "' already declared";
        throw(runtime_error(msg));
      }
    }  // end of insert

    void ConfigParser::execute(
        std::map<std::string, std::map<std::string, std::string>>& m,
        const std::string& f) {
      this->openFile(f);
      auto p = this->begin();
      const auto pe = this->end();
      try {
        this->stripComments();
        this->checkNotEndOfFile(p);
        while (p != pe) {
          std::string key;
          this->readString(key, p);
          insert(m, key);
          this->checkNotEndOfFile(p);
          this->readSpecifiedToken("{", p);
          this->checkNotEndOfFile(p);
          while (p->value != "}") {
            std::string l;
            std::string v;
            this->readString(l, p);
            this->checkNotEndOfFile(p);
            this->readSpecifiedToken(":", p);
            this->readString(v, p);
            this->checkNotEndOfFile(p);
            this->readSpecifiedToken(";", p);
            this->checkNotEndOfFile(p);
            if (p->value != "}") {
              this->readSpecifiedToken(",", p);
              if (p->value != "}") {
                throw(std::runtime_error("ConfigParser::execute"));
              }
            }
            insert(m, key, v, l);
          }
          this->readSpecifiedToken("}", p);
          this->checkNotEndOfFile(p);
          this->readSpecifiedToken(";", p);
        }
      } catch (std::exception& e) {
        std::ostringstream msg;
        msg << "ConfigParser::execute : " << e.what();
        if (p != this->end()) {
          msg << "\nError at line " << p->line << " of file '" << f << "'";
        }
        throw(std::runtime_error(msg.str()));
      }
    }  // end of ConfigParser::execute

    ConfigParser::~ConfigParser() noexcept = default;

  }  // end of namespace utilities

}  // end of namespace tfel
