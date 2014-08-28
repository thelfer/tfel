/*!
 * \file   mfront/src/MFrontLaTeXLawInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 */

#include<algorithm>
#include<fstream>
#include<sstream>
#include<stdexcept>
#include<iterator>
#include<string>
#include<set>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontCLawInterface.hxx"
#include"MFront/MFrontLaTeXLawInterface.hxx"

namespace mfront
{

  static std::string
  toLaTeX(const std::string& s){
    using namespace std;
    using namespace tfel::utilities;
    string r;
    r = replace_all(r,"\\","\\\\");
    r = replace_all(s,"_","\\textunderscore{}");
    return r;
  }
    
  std::string
  MFrontLaTeXLawInterface::getName(void)
  {
    return "LaTeX";
  }

  MFrontLaTeXLawInterface::MFrontLaTeXLawInterface()
  {}

  void
  MFrontLaTeXLawInterface::reset(void)
  {} // end of MFrontLaTeXLawInterface::reset(void)

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontLaTeXLawInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  MFrontLaTeXLawInterface::~MFrontLaTeXLawInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontLaTeXLawInterface::getGlobalDependencies(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontLaTeXLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontLaTeXLawInterface::getGlobalIncludes(const std::string&,
					     const std::string&,
					     const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontLaTeXLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontLaTeXLawInterface::getGeneratedSources(const std::string&,
					       const std::string&,
					       const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontLaTeXLawInterface::getGeneratedSources

  std::vector<std::string>
  MFrontLaTeXLawInterface::getGeneratedIncludes(const std::string&,
						const std::string&,
						const std::string&)
  {
    using namespace std;
    return vector<string>();
  } // end of MFrontLaTeXLawInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontLaTeXLawInterface::getLibrariesDependencies(const std::string&,
						    const std::string&,
						    const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontLaTeXLawInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontLaTeXLawInterface::getSpecificTargets(const std::string&,
					      const std::string&,
					      const std::string&,
					      const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontLaTeXLawInterface::getSpecificTargets

  static void
  describeVariableBounds(std::ofstream& out,
			 const std::string& name,
			 const std::string& boundsType,
			 const std::vector<VariableBoundsDescription>& bounds)
  {
    using namespace std;
    vector<VariableBoundsDescription>::const_iterator p;
    for(p=bounds.begin();p!=bounds.end();++p){
      if(p->varName==name){
	out << " Les " << boundsType << " de cette variable sont ";
	if(p->boundsType==VariableBoundsDescription::Lower){
	  out << "\\(\\left["  << p->lowerBound << ":+\\infty\\right[\\)";
	} else if(p->boundsType==VariableBoundsDescription::Upper){
	  out << "\\(\\left]-\\infty:" << p->upperBound << "\\right]\\)";
	} else if(p->boundsType==VariableBoundsDescription::LowerAndUpper){
	  out << "\\(\\left["  << p->lowerBound << ":" << p->upperBound << "\right]\\)";
	}
	out << "." << endl;
	return;
      }
    }
  } // end of describeVariableBounds

  void
  MFrontLaTeXLawInterface::writeOutputFiles(const std::string& file,
					    const std::string&,
					    const std::string& material,
					    const std::string& className,
					    const std::string& author,
					    const std::string& date,
					    const std::string& description,
					    const std::string&,
					    const std::string&,
					    const VariableDescriptionContainer& vars,
					    const std::vector<std::string>&,
					    const std::map<std::string,std::string>& glossaryNames,
					    const std::map<std::string,std::string>& entryNames,
					    const StaticVariableDescriptionContainer&,
					    const std::vector<std::string>&,
					    const std::map<std::string,double>&,
					    const LawFunction&,
					    const std::vector<VariableBoundsDescription>& bounds,
					    const std::vector<VariableBoundsDescription>& physicalBounds,
					    const bool,
					    const std::vector<std::string>&)
  {
    using namespace std;
    using namespace tfel::utilities;
    string name;
    VariableDescriptionContainer::const_iterator p;
    std::map<std::string,std::string>::const_iterator p2;
    if(material.empty()){
      name = className;
    } else {
      name = material+"_"+className;
    }
    ofstream out(("src/"+name+".tex").c_str());
    if(!out){
      string msg("MFrontLaTeXLawInterface::writeOutputFiles : ");
      msg += "can't open file '"+name+".tex'";
      throw(runtime_error(msg));
    }
    if(!material.empty()){
      out << "\\subsection{Description de la propri\\'et\\'e mat\\'eriau '"+className+"' du mat\\'eriau '"+material+"'}" << endl;
    } else {
      out << "\\subsection{Description de la propri\\'et\\'e mat\\'eriau '"+className+"'}" << endl;
    }
    out << "Cette documentation a \\'et\\'e g\\'en\\'er\\'ee \\`a partir du fichier {\\tt " << toLaTeX(file)  << "}" << endl;
    out << "\\begin{itemize}" << endl;
    if(!material.empty()){
      out << "\\item Mat\\'eriau~: " << toLaTeX(material) << endl;
    }
    if(!author.empty()){
      out << "\\item Auteur~: " << toLaTeX(author) << endl;
    }
    if(!date.empty()){
      out << "\\item Date~: " << toLaTeX(date) << endl;
    }
    out << "\\end{itemize}" << endl;
    if(!description.empty()){
      string d(description);
      if(d.size()>=2){
	if((d[0]=='*')&&
	   (d[1]==' ')){
	  d = d.substr(2);
	}
      }
      d = replace_all(d,"\n* ","\n");
      d = replace_all(d,"\\\'","'");
      d = replace_all(d,"\\\"","\"");
      d = replace_all(d,"{ ","{");
      d = replace_all(d," {","{");
      d = replace_all(d,"} ","}");
      d = replace_all(d," }","}");
      d = replace_all(d,"[ ","[");
      d = replace_all(d," [","[");
      d = replace_all(d,"] ","]");
      d = replace_all(d," ]","]");
      d = replace_all(d,"\\ ","\\");
      d = replace_all(d,"~ :","~:");
      out << "\\subsubsection{Description}" << endl << endl;
      if(!d.empty()){
	out << "Description1 : '" << d << "'" << endl;
	out << d << endl;
      } else {
	out << "Aucune description disponible" << endl;
      }
    }
    if(!vars.empty()){
      if(vars.size()==1u){
	out << "\\subsubsection{Variable d'entr\\'ee}" << endl << endl;
	out << "Cette propri\\'et\\'e d\\'epend de la variable {\\tt " << toLaTeX(vars.front().name) << "}";
	if((p2=glossaryNames.find(vars.front().name))!=glossaryNames.end()){
	  out << " dont le nom de glossaire est {\\tt " 
	      << toLaTeX(p2->second) << "}" << endl;
	} else if((p2=entryNames.find(vars.front().name))!=entryNames.end()){
	  out << " qui sera d\\'esign\\'ee dans le fichier d'entr\\'ee par {\\tt " 
	      << toLaTeX(p2->second) << "}" << endl;
	}
	out << "." << endl;
	describeVariableBounds(out,vars.front().name,"bornes physiques",physicalBounds);
	describeVariableBounds(out,vars.front().name,"bornes",bounds);
      } else {
	out << "\\subsubsection{Variables d'entr\\'ee}" << endl << endl;
	out << "Cette propri\\'et\\'e d\\'epend des variables~:" << endl;
	out << "\\begin{itemize}" << endl;
	for(p=vars.begin();p!=vars.end();++p){
	  out << "\\item {\\tt " << toLaTeX(p->name) << "}." << endl;
	  describeVariableBounds(out,p->name,"bornes physiques",physicalBounds);
	  describeVariableBounds(out,p->name,"bornes",bounds);
	}
	out << "\\end{itemize}" << endl;
      }

    }
    
  } // end of MFrontLaTeXLawInterface::writeOutputFiles
  
} // end of namespace mfront
