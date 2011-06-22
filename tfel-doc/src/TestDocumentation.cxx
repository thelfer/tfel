/*! 
 * \file  TestDocumentation.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#include"TFEL/Utilities/Global.hxx"
#include"TFEL/Utilities/LaTeXConvertion.hxx"
#include"TFEL/Utilities/TestDocumentation.hxx"

namespace tfel{

  namespace utilities {

    static
    void replace_all(std::string& s,
		     const char c,
		     const std::string& n)
    {
      using namespace std;
      string::size_type p  = 0u;
      string::size_type ns = n.size();
      if((s.size()==0)||(ns==0)){
	return;
      }
      while((p=s.find(c,p))!=string::npos){
	s.replace(p,1u,n);
	p+=ns;
      }
    } // end of replace_all
    
    static std::string
    toLaTeX(const std::string& s){
      using namespace std;
      string r(s);
      replace_all(r,'/',"/\\-");
      return r;
    }

    static std::string
    getIndexCommand(const std::string& s,
		    const std::string& i)
    {
      return "\\index{"+i+"}{"+LaTeXConvertion::capitalize(s)+"@\\textbf{"+LaTeXConvertion::capitalize(s)+"}}";
    }

    static std::string
    getIndexCommand(const std::string& s1,
		    const std::string& s2,
		    const std::string& i)
    {
      return "\\index{"+i+"}{"+LaTeXConvertion::capitalize(s1)+
	"@\\textbf{"+LaTeXConvertion::capitalize(s1)+
	"}!"+LaTeXConvertion::capitalize(s2)+"}";
    } // end of getIndexCommand

    void
    TestDocumentation::writeLaTexDescription(std::ostream& log,
					     const std::string& l) const
    {
      using namespace std;
      map<string,vector<string> >::const_iterator p;
      map<string,string>::const_iterator p2;
      log << "\\subsection{Test " << this-> name << "}" << endl;
      log << endl;
      log << "\\begin{center}" << endl;
      log << "\\begin{tabular}[!h]{lcp{0.65\\linewidth}}" << endl;
      log << LaTeXConvertion::capitalize(getTranslation("author",l));
      if(!this->author.empty()){
	log << " &:& " << this->author << "\\\\"<< endl;
      } else {
	log << "\\textcolor{red}{" << getTranslation("unknown author",l) << "} \\\\"<< endl;
      }
      log << LaTeXConvertion::capitalize(getTranslation("date",l));
      if(!this->date.empty()){
	log << " &:& " << this->date   << "\\\\"<< endl;
      } else {
	log << " &:& \\textcolor{red}{" << getTranslation("unspecified date",l) << "} \\\\"<< endl;
      }
      if(!this->src.empty()){
	log << LaTeXConvertion::capitalize(getTranslation("path in source",l));
	log << " &:& \\texttt{\\$\\{srcdir\\}/" << toLaTeX(this->src) << "}\\\\"<< endl;
      }
      if(!this->install.empty()){
	log << LaTeXConvertion::capitalize(getTranslation("path in install",l));
	log << " &:& \\texttt{\\$\\{prefix\\}/" << toLaTeX(this->install) << "}\\\\"<< endl;
      }
      log << "\\end{tabular}" << endl;
      log << "\\end{center}" << endl;
      log << endl;

      if(!this->keys.empty()){
	log << "\\begin{center}" << endl;
	log << "\\begin{tabular}[!h]{|l|l|}" << endl;
	log << "\\hline"    << endl;
	log << "\\multicolumn{1}{|c|}{" << LaTeXConvertion::capitalize(getTranslation("category",l)) 
	    << "} & \\multicolumn{1}{|c|}{" << LaTeXConvertion::capitalize(getTranslation("keyword",l)) << "} \\\\" << endl;
	log << "\\hline"    << endl;
	log << "\\hline"    << endl; 
	for(p=this->keys.begin();p!=this->keys.end();++p){
	  string key1;
	  key1 = getKeyValue(p->first,l);
	  if(p->second.empty()){
	    log << getIndexCommand(key1,"general") << endl;
	    log << LaTeXConvertion::capitalize(key1) << " & \\\\" << endl;
	  } else {
	    string key2;
	    vector<string>::const_iterator p3;
	    p3=p->second.begin();
	    key2 = getKeyValue(*p3,l);
	    log << getIndexCommand(key1,key2,"general") << endl;
	    log << LaTeXConvertion::capitalize(key1) << " & " << LaTeXConvertion::capitalize(key2) << "\\\\" << endl;
	    ++p3;
	    for(;p3!=p->second.end();++p3){
	      key2 = getKeyValue(*p3,l);
	      log << getIndexCommand(key1,key2,"general") << endl;;
	      log << " & " << LaTeXConvertion::capitalize(key2) << "\\\\" << endl;
	    }
	  }
	  log << "\\hline"    << endl; 
	}
	log << "\\end{tabular}" << endl;
	log << "\\end{center}" << endl;
	log << endl;
      }

      if(!this->models.empty()){
	map<string,vector<string> >::const_iterator pm;
	vector<string>::const_iterator pm2;
	if(models.size()==1){
	  log << getTranslation("\\paragraph{Tested model} The model tested is ",l) << models.begin()->first 
	      << getIndexCommand(models.begin()->first,"models");
	  for(pm2=models.begin()->second.begin();
	      pm2!=models.begin()->second.end();++pm2){
	    log << getIndexCommand(models.begin()->first,*pm2,"models") << endl;
	  }
	} else {
	  log << getTranslation("\\paragraph{List of tested models} The models tested are:",l) << endl;
	  log << "\\begin{itemize}" << endl;
	  for(pm=this->models.begin();pm!=this->models.end();++pm){
	    log << "\\item " << pm->first << getIndexCommand(pm->first,"models");
	    for(pm2=pm->second.begin();
		pm2!=pm->second.end();++pm2){
	      log << getIndexCommand(pm->first,*pm2,"models") << endl;
	    }
	    log << "~;" << endl;
	  }
	  log << "\\end{itemize}" << endl;
	}
      }

      p2 = this->descriptions.find(l);
      if(p2!=this->descriptions.end()){
	log << "\\paragraph{" << LaTeXConvertion::capitalize(getTranslation("description",l)) << "} "
	    << p2->second << endl << endl;
      } else {
	log << "\\paragraph{" << LaTeXConvertion::capitalize(getTranslation("description",l)) << "} "
	    << LaTeXConvertion::capitalize(getTranslation("no description available",l)) << endl << endl;
      }
    
    } // end of TestDocumentation::writeLaTexDescription

  } // end of namespace utilities

} // end of namespace tfel

