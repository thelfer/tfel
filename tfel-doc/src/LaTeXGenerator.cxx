/*! 
 * \file  tfel-doc/src/LaTeXConvertion.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<fstream>
#include<iostream>

#include"TFEL/Utilities/Global.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/Utilities/TestDocumentation.hxx"
#include"TFEL/Utilities/LaTeXGenerator.hxx"

namespace tfel
{

  namespace utilities
  {

    static std::string
    capitalize(const std::string& s)
    {
      using namespace std;
      string r(s);
      if(r[0]=='é'){
	r[0]='É';
      } else if(r[0]=='è'){
	r[0]='È';
      } else if(r[0]=='ê'){
	r[0]='Ê';
      } else if(r[0]=='à'){
	r[0]='À';
      } else if(r[0]=='ç'){
	r[0]='Ç';
      } else {
	r[0] = static_cast<char>(toupper(r[0]));
      }
      return r;
    } // end of capitalize
    
    static std::string
    getSectionHeaderTranslation(const std::string& s,
				const std::string& l)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(l=="french"){
	return "Test de la catégorie \\og~"+getCategory(s,l)+"~\\fg{}";
      }
      if(l!="english"){
	cerr << TerminalColors::Red;
	cerr << "getSectionHeaderTranslation : unsupported language'"+l
	     << "', using default" << endl;
	cerr << TerminalColors::Reset;
      }
      return s;
    }

    static std::string
    toLaTeX(const std::string& s){
      using namespace std;
      string r(s);
      replace_all(r,'/',"/\\-");
      replace_all(r,'_',"{\\textunderscore}");
      return r;
    }

    static std::string
    getIndexCommand(const std::string& s,
		    const std::string& i)
    {
      return "\\index{"+i+"}{"+capitalize(s)+"@\\textbf{"+capitalize(s)+"}}";
    }

    static std::string
    getIndexCommand(const std::string& s1,
		    const std::string& s2,
		    const std::string& i)
    {
      return "\\index{"+i+"}{"+capitalize(s1)+
	"@\\textbf{"+capitalize(s1)+
	"}!"+capitalize(s2)+"}";
    } // end of getIndexCommand
    
    static void
    writeLaTexDescription(std::ostream& log,
			  const TestDocumentation& td,
			  const std::string& prefix,
			  const std::string& l)
    {
      using namespace std;
      map<string,vector<string> >::const_iterator p;
      map<string,string>::const_iterator p2;
      log << "\\subsection{Test " << toLaTeX(td. name) << "}" << endl;
      log << endl;
      log << "\\begin{center}" << endl;
      log << "\\begin{tabular}[!h]{lcp{0.65\\linewidth}}" << endl;
      log << capitalize(getTranslation("author",l));
      if(!td.author.empty()){
	log << " &:& " << td.author << "\\\\"<< endl;
      } else {
	log << " &:& \\textcolor{red}{" << getTranslation("unknown author",l) << "} \\\\"<< endl;
      }
      log << capitalize(getTranslation("date",l));
      if(!td.date.empty()){
	log << " &:& " << td.date   << "\\\\"<< endl;
      } else {
	log << " &:& \\textcolor{red}{" << getTranslation("unspecified date",l) << "} \\\\"<< endl;
      }
      if(!td.src.empty()){
	log << capitalize(getTranslation("path in source",l));
	log << " &:& \\texttt{\\$\\{srcdir\\}/" << toLaTeX(td.src) << "}\\\\"<< endl;
      }
      if(!td.install.empty()){
	log << capitalize(getTranslation("path in install",l));
	log << " &:& ";
	if(prefix.empty()){
	  log << "\\texttt{\\$\\{prefix\\}/" << toLaTeX(td.install) << "}\\\\"<< endl;
	} else {
	  log << "\\href{run:" << prefix << "/" << td.install 
	      << "}{\\texttt{\\$\\{prefix\\}/" << toLaTeX(td.install) << "}}\\\\"<< endl;
	}
      }
      log << "\\end{tabular}" << endl;
      log << "\\end{center}" << endl;
      log << endl;

      if(!td.keys.empty()){
	log << "\\begin{center}" << endl;
	log << "\\begin{tabular}[!h]{|l|l|}" << endl;
	log << "\\hline"    << endl;
	log << "\\multicolumn{1}{|c|}{" << capitalize(getTranslation("category",l)) 
	    << "} & \\multicolumn{1}{|c|}{" << capitalize(getTranslation("keyword",l)) << "} \\\\" << endl;
	log << "\\hline"    << endl;
	log << "\\hline"    << endl; 
	for(p=td.keys.begin();p!=td.keys.end();++p){
	  string key1;
	  key1 = getKeyValue(p->first,l);
	  if(p->second.empty()){
	    log << getIndexCommand(key1,"general") << endl;
	    log << capitalize(key1) << " & \\\\" << endl;
	  } else {
	    string key2;
	    vector<string>::const_iterator p3;
	    p3=p->second.begin();
	    key2 = getKeyValue(*p3,l);
	    log << getIndexCommand(key1,key2,"general") << endl;
	    log << capitalize(key1) << " & " << capitalize(key2) << "\\\\" << endl;
	    ++p3;
	    for(;p3!=p->second.end();++p3){
	      key2 = getKeyValue(*p3,l);
	      log << getIndexCommand(key1,key2,"general") << endl;;
	      log << " & " << capitalize(key2) << "\\\\" << endl;
	    }
	  }
	  log << "\\hline"    << endl; 
	}
	log << "\\end{tabular}" << endl;
	log << "\\end{center}" << endl;
	log << endl;
      }

      if(!td.models.empty()){
	if(td.models.size()==1){
	  log << getTranslation("\\paragraph{Tested model} The model tested is ",l)
	      << td.models.begin()->first 
	      << getIndexCommand(td.models.begin()->first,"models") << "." << endl;
	  for(auto pm2=td.models.begin()->second.begin();
	      pm2!=td.models.begin()->second.end();++pm2){
	    log << getIndexCommand(td.models.begin()->first,*pm2,"models") << endl;
	  }
	} else {
	  log << getTranslation("\\paragraph{List of tested models} The models tested are:",l) << endl;
	  log << "\\begin{itemize}" << endl;
	  for(auto pm=td.models.begin();pm!=td.models.end();++pm){
	    log << "\\item " << pm->first << getIndexCommand(pm->first,"models");
	    for(auto pm2=pm->second.begin();pm2!=pm->second.end();++pm2){
	      log << getIndexCommand(pm->first,*pm2,"models") << endl;
	    }
	    log << "~;" << endl;
	  }
	  log << "\\end{itemize}" << endl;
	}
      }

      p2 = td.descriptions.find(l);
      if(p2!=td.descriptions.end()){
	log << "\\paragraph{" << capitalize(getTranslation("description",l)) << "} "
	    << p2->second << endl << endl;
      } else {
	log << "\\paragraph{" << capitalize(getTranslation("description",l)) << "} "
	    << capitalize(getTranslation("no description available",l)) << endl << endl;
      }
    } // end of TestDocumentation::writeLaTexDescription

    
    static std::string
    getBabelPackage(const std::string& l)
    {
      using namespace std;
      if(l!="french"){
	cerr << TerminalColors::Red;
	cerr << "getBabelPackage : unsupported language '" << l << 
	  "', using default" << endl;
	cerr << TerminalColors::Reset;
	return "";
      }
      return "\\usepackage[frenchb]{babel}";
    }

    static void
    writeLaTeXHeader(std::ostream& log,
		     const std::string& l){
      log << "\\documentclass[a4paper,12pt]{article}\n\n";;
      log << "\\usepackage[utf8]{inputenc}\n";
      log << "\\usepackage{multind}\n";
      log << "\\usepackage{amsmath}\n";
      log << "\\usepackage{color}\n\n";;
      log << "\\newcommand{\\Frac}[2]{\\displaystyle\\frac{\\displaystyle #1}{\\displaystyle #2}}\n";
      log << "\\newcommand{\\paren}[1]{\\ensuremath\\left(#1\\right)}\n";
      log << "\\newcommand{\\tenseur}[1]{\\ensuremath\\underline{#1}}\n";
      log << "\\newcommand{\\tenseurq}[1]{\\ensuremath\\underline{\\underline{#1}}}\n";
      log << "\\newcommand{\\nom}[1]{\\textsc{#1}}\n\n";;
      log << "% one column index\n";
      log << "\\makeatletter\n";
      log << "\\def\\printindex#1#2{\\section*{#2}\n";
      log << "\\addcontentsline{toc}{section}{#2}\n";
      log << "\\@input{#1.ind}}\n";
      log << "\\makeatother\n\n";
      if(l!="english"){
	log << getBabelPackage(l) << '\n';
      }
      log << "\\makeindex{general}\n";
      log << "\\makeindex{models}\n\n";;
      log << "\\begin{document}\n\n";
    }

    static void
    writeLaTeXEndOfDocument(std::ostream& log,
			    const std::string& l){
      log << "\\clearpage\n";
      log << "\\newpage\n";
      log << "\\printindex{general}{"
	  << capitalize(getTranslation("general index",l)) << "}\n\n";
      log << "\\clearpage\n";
      log << "\\newpage\n";
      log << "\\printindex{models}{"
	  << capitalize(getTranslation("models index",l)) << "}\n\n";
      log << "\\end{document}\n";
    }

    static void
    writeLaTeXSection(std::ostream& out,
		      const std::string& s,
		      const std::vector<TestDocumentation>& tests,
		      const std::string& prefix,
		      const std::string& l){
      out  << "\\section{" << getSectionHeaderTranslation(s,l) << "}\n\n";
      for(const auto& td : tests){
	writeLaTexDescription(out,td,prefix,l);
      }
    }
    
    void
    printLaTeXFile(std::ostream& log,
		   const std::map<std::string,std::vector<TestDocumentation> >& tests,
		   const std::string& d,
		   const std::string& prefix,
		   const std::string& l,
		   const bool fragment,
		   const bool split)
    {
      if(!fragment){
	writeLaTeXHeader(log,l);
      }
      for(const auto& td : tests){
	log << "\\clearpage\n";
	log << "\\newpage\n";
	if(split){
	  const auto& tf = replace_all(td.first,' ','_')+".tex";
	  const auto& file = d+"/"+tf;
	  std::ofstream f(file);
	  if(!f){
	    throw(std::runtime_error("printLaTeXFile : can't open file '"+file+"'"));
	  }
	  writeLaTeXSection(f,td.first,td.second,prefix,l);
	  log << "\\input{" << tf << "}\n\n";
	} else {
	  writeLaTeXSection(log,td.first,td.second,prefix,l);
	}
      }
      if(!fragment){
	writeLaTeXEndOfDocument(log,l);
      }
    }
    
  } // end of namespace utilities

} // end of namespace tfel

