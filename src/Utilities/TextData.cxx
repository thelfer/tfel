/*!
 * \file   src/Utilities/TextData.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 Nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cassert>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/TextData.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

namespace tfel::utilities {

  TextData::TextData(const std::string& file, const std::string& format) {
    auto get_legends = [](const std::string& l) {
      std::vector<std::string> r;
      CxxTokenizer t;
      t.treatCharAsString(true);
      t.parseString(l);
      t.stripComments();
      std::for_each(t.begin(), t.end(), [&r](const Token& w) {
        if (w.flag == Token::String) {
          r.push_back(w.value.substr(1, w.value.size() - 2));
        } else {
          r.push_back(w.value);
        }
      });
      return r;
    };
    auto add_line = [this](const std::string& l, const Token::size_type n) {
      Line nl;
      CxxTokenizer t;
      t.treatCharAsString(true);
      t.parseString(l);
      t.stripComments();
      std::for_each(t.begin(), t.end(), [&nl, &n](const Token& w) {
        nl.tokens.push_back(w);
        nl.tokens.back().line = n;
      });
      this->lines.push_back(std::move(nl));
    };
    auto firstLine = true;
    auto firstComments = true;
    auto nbr = size_type{1};
    std::ifstream f{file};
    raise_if(!f, "TextData::TextData: can't open '" + file + '\'');
    while (!f.eof()) {
      auto line = std::string{};
      getline(f, line);
      if (line.empty()) {
        continue;
      }
      if (line[0] == '#') {
        if (!firstComments) {
          continue;
        }
        if (format.empty()) {
          line.erase(line.begin());
          if (firstLine) {
            this->legends = get_legends(line);
          }
          this->preamble.push_back(line);
        }
      } else {
        if (((format == "gnuplot") || (format == "alcyone")) && (firstLine)) {
          this->legends = get_legends(line);
          bool all_numbers = true;
          for (const auto& l : this->legends) {
            try {
              convert<double>(l);
            } catch (std::exception&) {
              all_numbers = false;
            }
            if (!all_numbers) {
              break;
            }
          }
          if (all_numbers) {
            this->legends.clear();
            add_line(line, nbr);
          }
        } else {
          add_line(line, nbr);
          firstComments = false;
        }
      }
      firstLine = false;
      ++nbr;
    }
  }  // end of TextData::TextData

  const std::vector<std::string>& TextData::getLegends() const {
    return this->legends;
  }  // end of TextData::getLegends

  const std::vector<std::string>& TextData::getPreamble() const {
    return this->preamble;
  }  // end of TextData::getPreamble

  std::string TextData::getLegend(const size_type c) const {
    raise_if(c == 0, "TextData::getLegend: invalid column index");
    if (c >= this->legends.size() + 1) {
      return "";
    }
    auto c2 = c;
    return this->legends[--c2];
  }  // end of TextData::getLegend

  TextData::size_type TextData::findColumn(const std::string& n) const {
    auto p = std::find(this->legends.begin(), this->legends.end(), n);
    raise_if(p == this->legends.end(),
             "TextData::findColumn: "
             "no column named '" +
                 n + "' found'.");
    return static_cast<size_type>(p - this->legends.begin() + 1);
  }  // end of TextData::findColumn

  std::vector<double> TextData::getColumn(const size_type i) const {
    std::vector<double> tab;
    this->getColumn(tab, i);
    return tab;
  }  // end of TextData::getColumn

  void TextData::getColumn(std::vector<double>& tab, const size_type i) const {
    auto throw_if = [](const bool b, const std::string& msg) {
      raise_if(b, "TextData::getColumn: " + msg);
    };
    tab.clear();
    tab.reserve(this->lines.size());
    // sanity check
    throw_if(i == 0u,
             "column '0' requested "
             "(column numbers begins at '1').");
    // treatment
    for (const auto& l : this->lines) {
      auto n = l.tokens.empty() ? 0 : l.tokens[0].line;
      throw_if(l.tokens.size() < i, "line '" + std::to_string(n) +
                                        "' "
                                        "does not have '" +
                                        std::to_string(i) + "' columns.");
      tab.push_back(convert<double>(l.tokens.at(i - 1u).value));
    }
  }  // end of TextData::getColumn

  std::vector<TextData::Line>::const_iterator TextData::begin() const {
    return this->lines.begin();
  }  // end of TextData::begin()

  std::vector<TextData::Line>::const_iterator TextData::end() const {
    return this->lines.end();
  }  // end of TextData::end()

  void TextData::skipLines(const Token::size_type n) {
    auto get_line = [](const Line& l) -> Token::size_type {
      return l.tokens.empty() ? 0 : l.tokens[0].line;
    };
    if (this->lines.empty()) {
      return;
    }
    auto p = this->lines.begin();
    while ((get_line(*p) <= n + 1) && (p != this->lines.end())) {
      ++p;
    }
    lines.erase(lines.begin(), p);
  }

}  // end of namespace tfel::utilities
