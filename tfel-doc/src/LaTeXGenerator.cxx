/*!
 * \file  tfel-doc/src/LaTeXConvertion.cxx
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

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Global.hxx"
#include "TFEL/Utilities/GeneratorOptions.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Utilities/TestDocumentation.hxx"
#include "TFEL/Utilities/LaTeXGenerator.hxx"

namespace tfel {

  namespace utilities {

    static std::string getSectionHeaderTranslation(const std::string& s,
                                                   const std::string& l) {
      using namespace std;
      using namespace tfel::utilities;
      if (l == "french") {
        return "Test de la catégorie \\og~" + getCategory(s, l) + "~\\fg{}";
      }
      if (l != "english") {
        cerr << TerminalColors::Red;
        cerr << "getSectionHeaderTranslation : unsupported language'" + l
             << "', using default\n";
        cerr << TerminalColors::Reset;
      }
      return s;
    }

    static std::string toLaTeX(const std::string& s) {
      std::string r(s);
      replace_all(r, '/', "/\\-");
      replace_all(r, '_', "{\\textunderscore}");
      return r;
    }

    static std::string getIndexCommand(const std::string& s,
                                       const std::string& i) {
      return "\\index{" + i + "}{" + capitalize(s) + "@\\textbf{" +
             capitalize(s) + "}}";
    }

    static std::string getIndexCommand(const std::string& s1,
                                       const std::string& s2,
                                       const std::string& i) {
      return "\\index{" + i + "}{" + capitalize(s1) + "@\\textbf{" +
             capitalize(s1) + "}!" + capitalize(s2) + "}";
    }  // end of getIndexCommand

    static void writeLaTexDescription(std::ostream& out,
                                      const TestDocumentation& t,
                                      const std::string& prefix,
                                      const std::string& l) {
      using namespace std;
      out << "\\subsection{Test " << toLaTeX(t.name) << "}\n";
      out << endl;
      out << "\\begin{center}\n";
      out << "\\begin{tabular}[!h]{lcp{0.65\\linewidth}}\n";
      out << capitalize(getTranslation("author", l));
      if (!t.author.empty()) {
        out << " &:& " << t.author << "\\\\" << endl;
      } else {
        out << " &:& \\textcolor{red}{" << getTranslation("unknown author", l)
            << "} \\\\" << endl;
      }
      out << capitalize(getTranslation("date", l));
      if (!t.date.empty()) {
        out << " &:& " << t.date << "\\\\" << endl;
      } else {
        out << " &:& \\textcolor{red}{" << getTranslation("unspecified date", l)
            << "} \\\\" << endl;
      }
      if (!t.src.empty()) {
        out << capitalize(getTranslation("path in source", l));
        out << " &:& \\texttt{\\$\\{srcdir\\}/" << toLaTeX(t.src) << "}\\\\"
            << endl;
      }
      if (!t.install.empty()) {
        out << capitalize(getTranslation("path in install", l));
        out << " &:& ";
        if (prefix.empty()) {
          out << "\\texttt{\\$\\{prefix\\}/" << toLaTeX(t.install) << "}\\\\"
              << endl;
        } else {
          out << "\\href{run:" << prefix << "/" << t.install
              << "}{\\texttt{\\$\\{prefix\\}/" << toLaTeX(t.install) << "}}\\\\"
              << endl;
        }
      }
      out << "\\end{tabular}\n";
      out << "\\end{center}\n";
      out << endl;

      if (!t.keys.empty()) {
        out << "\\begin{center}\n";
        out << "\\begin{tabular}[!h]{|l|l|}\n";
        out << "\\hline" << endl;
        out << "\\multicolumn{1}{|c|}{"
            << capitalize(getTranslation("category", l))
            << "} & \\multicolumn{1}{|c|}{"
            << capitalize(getTranslation("keyword", l)) << "} \\\\\n";
        out << "\\hline" << endl;
        out << "\\hline" << endl;
        for (const auto& k : t.keys) {
          const auto key1 = getKeyValue(k.first, l);
          if (k.second.empty()) {
            out << getIndexCommand(key1, "general") << endl;
            out << capitalize(key1) << " & \\\\\n";
          } else {
            auto p3 = k.second.begin();
            auto key2 = getKeyValue(*p3, l);
            out << getIndexCommand(key1, key2, "general") << endl;
            out << capitalize(key1) << " & " << capitalize(key2) << "\\\\\n";
            ++p3;
            for (; p3 != k.second.end(); ++p3) {
              key2 = getKeyValue(*p3, l);
              out << getIndexCommand(key1, key2, "general") << endl;
              out << " & " << capitalize(key2) << "\\\\\n";
            }
          }
          out << "\\hline" << endl;
        }
        out << "\\end{tabular}\n";
        out << "\\end{center}\n";
        out << endl;
      }

      if (!t.models.empty()) {
        if (t.models.size() == 1) {
          out << "\\paragraph{" << getTranslation("Tested model", l) << "}"
              << " " << getTranslation("The model tested is:", l) << endl
              << t.models.begin()->first
              << getIndexCommand(t.models.begin()->first, "models") << ".\n";
          for (auto pm2 = t.models.begin()->second.begin();
               pm2 != t.models.begin()->second.end(); ++pm2) {
            out << getIndexCommand(t.models.begin()->first, *pm2, "models")
                << endl;
          }
        } else {
          out << "\\paragraph{" << getTranslation("List of tested models", l)
              << "}"
              << " " << getTranslation("The models tested are:", l) << endl;
          out << "\\begin{itemize}\n";
          for (const auto& m : t.models) {
            out << "\\item " << m.first << getIndexCommand(m.first, "models");
            for (const auto& m2 : m.second) {
              out << getIndexCommand(m.first, m2, "models") << endl;
            }
            out << "~;\n";
          }
          out << "\\end{itemize}\n";
        }
      }
      auto pd = t.descriptions.find(l);
      if (pd != t.descriptions.end()) {
        out << "\\paragraph{" << capitalize(getTranslation("description", l))
            << "} " << pd->second << endl
            << endl;
      } else {
        out << "\\paragraph{" << capitalize(getTranslation("description", l))
            << "} " << capitalize(getTranslation("no description available", l))
            << endl
            << endl;
      }
    }  // end of TestDocumentation::writeLaTexDescription

    static std::string getBabelPackage(const std::string& l) {
      using namespace std;
      if (l != "french") {
        cerr << TerminalColors::Red;
        cerr << "getBabelPackage : unsupported language '" << l
             << "', using default\n";
        cerr << TerminalColors::Reset;
        return "";
      }
      return "\\usepackage[frenchb]{babel}";
    }

    static void writeLaTeXHeader(std::ostream& out, const std::string& l) {
      out << "\\documentclass[a4paper,12pt]{article}\n\n";
      out << "\\usepackage[utf8]{inputenc}\n";
      out << "\\usepackage{multind}\n";
      out << "\\usepackage{amsmath}\n";
      out << "\\usepackage{color}\n\n";
      out << "\\newcommand{\\Frac}[2]{\\displaystyle\\frac{\\displaystyle "
             "#1}{\\displaystyle #2}}\n";
      out << "\\newcommand{\\paren}[1]{\\ensuremath\\left(#1\\right)}\n";
      out << "\\newcommand{\\tenseur}[1]{\\ensuremath\\underline{#1}}\n";
      out << "\\newcommand{\\tenseurq}[1]{\\ensuremath\\underline{\\underline{#"
             "1}}}\n";
      out << "\\newcommand{\\nom}[1]{\\textsc{#1}}\n\n";
      out << "% one column index\n";
      out << "\\makeatletter\n";
      out << "\\def\\printindex#1#2{\\section*{#2}\n";
      out << "\\addcontentsline{toc}{section}{#2}\n";
      out << "\\@input{#1.ind}}\n";
      out << "\\makeatother\n\n";
      if (l != "english") {
        out << getBabelPackage(l) << '\n';
      }
      out << "\\makeindex{general}\n";
      out << "\\makeindex{models}\n\n";
      out << "\\begin{document}\n\n";
    }

    static void writeLaTeXEndOfDocument(std::ostream& out,
                                        const std::string& l) {
      out << "\\clearpage\n";
      out << "\\newpage\n";
      out << "\\printindex{general}{"
          << capitalize(getTranslation("general index", l)) << "}\n\n";
      out << "\\clearpage\n";
      out << "\\newpage\n";
      out << "\\printindex{models}{"
          << capitalize(getTranslation("models index", l)) << "}\n\n";
      out << "\\end{document}\n";
    }

    static void writeLaTeXSection(std::ostream& out,
                                  const std::string& s,
                                  const std::vector<TestDocumentation>& tests,
                                  const std::string& prefix,
                                  const std::string& l) {
      out << "\\section{" << getSectionHeaderTranslation(s, l) << "}\n\n";
      for (const auto& td : tests) {
        writeLaTexDescription(out, td, prefix, l);
      }
    }

    void writeLaTeXFile(
        std::ostream& out,
        const std::map<std::string, std::vector<TestDocumentation>>& tests,
        const GeneratorOptions& o) {
      if (o.standAlone) {
        writeLaTeXHeader(out, o.lang);
      }
      for (const auto& td : tests) {
        out << "\\clearpage\n";
        out << "\\newpage\n";
        if (o.split) {
          const auto& tf = replace_all(td.first, ' ', '_') + ".tex";
          const auto& file = o.outputDirectory + "/" + tf;
          std::ofstream f(file);
          raise_if(!f, "writeLaTeXFile : can't open file '" + file + "'");
          writeLaTeXSection(f, td.first, td.second, o.prefix, o.lang);
          out << "\\input{" << tf << "}\n\n";
        } else {
          writeLaTeXSection(out, td.first, td.second, o.prefix, o.lang);
        }
      }
      if (o.standAlone) {
        writeLaTeXEndOfDocument(out, o.lang);
      }
    }

  }  // end of namespace utilities

}  // end of namespace tfel
