/*! 
 * \file   mfront/src/MarkdownBehaviourAnalyser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  19 mai 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<fstream>
#include<sstream>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"

#include"MFront/MFrontLogStream.hxx"
#include"MFront/BehaviourDescription.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/MarkdownBehaviourAnalyser.hxx"

namespace mfront
{

  std::string
  MarkdownBehaviourAnalyser::getName(void)
  {
    return "Markdown";
  }

  MarkdownBehaviourAnalyser::MarkdownBehaviourAnalyser()
    : language("default")
  {} // end of MarkdownBehaviourAnalyser::MarkdownBehaviourAnalyser

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MarkdownBehaviourAnalyser::treatKeyword(const std::string& key,
						tfel::utilities::CxxTokenizer::TokensContainer::const_iterator p,
						const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator pe)
  {
    using namespace std;
    using namespace tfel::utilities;
    if (key=="@MarkdownLanguage"){
      if(this->language!="default"){
	string msg("MarkdownBehaviourAnalyser::treatKeyword : "
		   "language already defined");
	throw(runtime_error(msg));
      }
      CxxTokenizer::checkNotEndOfLine("MarkdownBehaviourAnalyser::treatKeyword","",p,pe);
      const string l = (p->flag==Token::String) ? p->value.substr(1,p->value.size()-2) : p->value;
      ++p;
      CxxTokenizer::readSpecifiedToken("MarkdownBehaviourAnalyser::treatKeyword",";",p,pe);
      if((l=="default")||(l=="english")){
	this->language="english";
      } else if(l=="french"){
	this->language=l;
      } else if(l!="default"){
	string msg("MarkdownBehaviourAnalyser::treatKeyword : "
		   "unknown language '"+l+"'");
	throw(runtime_error(msg));
      }
      return make_pair(true,p);
    }
    return make_pair(false,p);
  }

  void
  MarkdownBehaviourAnalyser::endTreatement(const BehaviourDescription& mb,
						 const FileDescription& fd)
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::utilities;
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MarkdownBehaviourAnalyser::endTreatement : begin" << endl;
    }
    string name;
    if(!mb.getLibrary().empty()){
      name += mb.getLibrary();
    }
    name += mb.getClassName();
    ofstream out(("src/"+name+".txt").c_str());
    if(!out){
      string msg("MarkdownBehaviourAnalyser::endTreatement : ");
      msg += "could not open file ";
      msg += "'src/"+name+".txt'";
      throw(runtime_error(msg));
    }
    if(this->language=="default"){
      this->language="english";
    }
    if(this->language=="english"){
      this->treatEnglishOutput(out,mb,fd);
    } else if(this->language=="french"){
      this->treatFrenchOutput(out,mb,fd);
    } else {
      string msg("MarkdownBehaviourAnalyser::endTreatement : ");
      msg += "unsupported language '"+this->language+"'";
      throw(runtime_error(msg));
    }
    out.close();
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MarkdownBehaviourAnalyser::endTreatement : end" << endl;
    }
  } // end of MarkdownBehaviourAnalyser::endTreatement

  void
  MarkdownBehaviourAnalyser::treatEnglishOutput(std::ostream& out,
						      const BehaviourDescription& mb,
						      const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::material;
    out << "# " << mb.getClassName() << " behaviour description" << endl << endl;
    out << "## Source file" << endl << endl;
    out << "* file   : " << fd.fileName   << endl;
    out << "* author : ";
    if(!fd.authorName.empty()){
      out << fd.authorName << endl;
    } else {
      out << "(unspecified)" << endl;
    }
    out << "* date   : ";
    if(!fd.date.empty()){
      out << fd.date;
    } else {
      out << "(unspecified)";
    }
    out << endl << endl;
    if(!fd.description.empty()){
      vector<string> d = tokenize(fd.description,'\n');
      for(vector<string>::const_iterator pd=d.begin();pd!=d.end();++pd){
	if((pd->size()>=2)&&((*pd)[0]=='*')&&((*pd)[1]==' ')){
	  out << pd->substr(2) << endl;
	} else {
	  out << *pd << endl;
	}
      }
    } else {
      out << "No description specified";
    }
    out << endl << endl;
    out << "## Mechanical behaviour description" << endl << endl;
    out << "### List of supported Hypotheses" << endl << endl;
    const set<ModellingHypothesis::Hypothesis>& h  = mb.getModellingHypotheses();
    const set<ModellingHypothesis::Hypothesis>& dh = mb.getDistinctModellingHypotheses();
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      out << "* " <<  ModellingHypothesis::toString(*ph);
      if(dh.find(*ph)!=dh.end()){
	out << ", specialised";
      }
      out << endl;
    }
    out << endl;
    out << "### Material properties" << endl << endl;
    printData(out,mb,getData(mb,&BehaviourData::getMaterialProperties));
    out << endl;
    out << "### State variables " << endl << endl;
    printData(out,mb,getData(mb,&BehaviourData::getPersistentVariables));
    out << endl;
    out << "### External state variables " << endl << endl;
    printData(out,mb,getData(mb,&BehaviourData::getExternalStateVariables));
    out << endl;
    out << "### Local variables " << endl << endl;
    printData(out,mb,getData(mb,&BehaviourData::getLocalVariables));
    out << endl;
    if(mb.hasParameters()){
      out << "### Parameters " << endl << endl;
      printData(out,mb,getData(mb,&BehaviourData::getParameters));
    }
    out << endl;
    out << "## Code blocks" << endl << endl;
    if(dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS)!=dh.end()){
      const BehaviourData& d =
	mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      const vector<string>& cn = d.getCodeBlockNames();
      for(vector<string>::const_iterator pcn = cn.begin();pcn!=cn.end();++pcn){
	const CodeBlock& c = d.getCodeBlock(*pcn);
	if(!c.description.empty()){
	  out <<"### " << *pcn << " description" << endl;
	  out << c.description << endl << endl;
	}
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  out <<"### " << *pcn << " listing" << endl;
	  out << endl;
	  out << "~~~~~~~ {.cpp}" << endl;
	  out << c.code << endl;
	  out << "~~~~~~~ " << endl;
	}
      }
    }
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(*ph!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	const BehaviourData& d =
	  mb.getBehaviourData(*ph);
	const vector<string>& cn = d.getCodeBlockNames();
	for(vector<string>::const_iterator pcn = cn.begin();pcn!=cn.end();++pcn){
	  bool print = true;
	  const CodeBlock& c = d.getCodeBlock(*pcn);
	  if(dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS)!=dh.end()){
	    const BehaviourData& duh =
	      mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	    if(duh.hasCode(*pcn)){
	      const string& cuh = duh.getCodeBlock(*pcn).code;
	      print = c.code!=cuh;
	    }
	  }
	  if(print){
	    if(!c.description.empty()){
	      out <<"### " << *pcn << " description(" << ModellingHypothesis::toString(*ph) << ")" << endl;
	      out << c.description << endl << endl;
	    }
	    out <<"### " << *pcn << " listing (" << ModellingHypothesis::toString(*ph) << ")" << endl;
	    out << endl;
	    out << "~~~~~~~ {.cpp} "<< endl;
	    out << c.code << endl;
	    out << "~~~~~~~ "<< endl;
	  }
	}
      }
    }
  } // end of MarkdownBehaviourAnalyser::treatEnglishOutput

  void
  MarkdownBehaviourAnalyser::treatFrenchOutput(std::ostream& out,
						     const BehaviourDescription& mb,
						     const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::material;
    out << "# Desription de la loi de comportement mécanique " << mb.getClassName() << endl << endl;
    if(!fd.description.empty()){
      vector<string> d = tokenize(fd.description,'\n');
      for(vector<string>::const_iterator pd=d.begin();pd!=d.end();++pd){
	if((pd->size()>=2)&&((*pd)[0]=='*')&&((*pd)[1]==' ')){
	  out << pd->substr(2) << endl;
	} else {
	  out << *pd << endl;
	}
      }
    } else {
      out << "Aucune description spécifiée";
    }
    out << endl << endl;
    out << "## Informations générales relatives au fichier source" << endl << endl;
    out << "* nom du fichier : " << fd.fileName   << endl;
    out << "* auteur du fichier : ";
    if(!fd.authorName.empty()){
      out << fd.authorName << endl;
    } else {
      out << "(aucun auteur défini)" << endl;
    }
    out << "* date d'écriture du fichier   : ";
    if(!fd.date.empty()){
      out << fd.date;
    } else {
      out << "(date non définie)";
    }
    out << endl << endl;
    out << "## Informations générales sur la loi de comportement" << endl << endl;
    out << "### Liste des hypothèses modélisées" << endl << endl;
    const set<ModellingHypothesis::Hypothesis>& h  = mb.getModellingHypotheses();
    const set<ModellingHypothesis::Hypothesis>& dh = mb.getDistinctModellingHypotheses();
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      out << "* " <<  ModellingHypothesis::toString(*ph);
      if(dh.find(*ph)!=dh.end()){
	out << ", spécialisée";
      }
      out << endl;
    }
    out << endl;
    out << "### Liste des propriétés matériaux" << endl << endl;
    printData(out,mb,getData(mb,&BehaviourData::getMaterialProperties));
    out << endl;
    out << "### Liste des variabes internes" << endl << endl;
    printData(out,mb,getData(mb,&BehaviourData::getPersistentVariables));
    out << endl;
    out << "### Liste des variables externes" << endl << endl;
    printData(out,mb,getData(mb,&BehaviourData::getExternalStateVariables));
    out << endl;
    out << "### Liste des variables locales " << endl << endl;
    printData(out,mb,getData(mb,&BehaviourData::getLocalVariables));
    out << endl;
    if(mb.hasParameters()){
      out << "### Liste des paramètres" << endl << endl;
      printData(out,mb,getData(mb,&BehaviourData::getParameters));
    }
    out << endl;
    out << "## Description des différentes portions de code" << endl << endl;
    if(dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS)!=dh.end()){
      const BehaviourData& d =
	mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      const vector<string>& cn = d.getCodeBlockNames();
      for(vector<string>::const_iterator pcn = cn.begin();pcn!=cn.end();++pcn){
	const CodeBlock& c = d.getCodeBlock(*pcn);
	if(!c.description.empty()){
	  out <<"### Description relative à la portion " << *pcn << endl;
	  out << c.description << endl << endl;
	}
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  out <<"### Listing associée à la portion " << *pcn << endl;
	  out << endl;
	  out << "~~~~~~~ {.cpp}" << endl;
	  out << c.code << endl;
	  out << "~~~~~~~ " << endl;
	}
      }
    }
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(*ph!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	const BehaviourData& d =
	  mb.getBehaviourData(*ph);
	const vector<string>& cn = d.getCodeBlockNames();
	for(vector<string>::const_iterator pcn = cn.begin();pcn!=cn.end();++pcn){
	  bool print = true;
	  const CodeBlock& c = d.getCodeBlock(*pcn);
	  if(dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS)!=dh.end()){
	    const BehaviourData& duh =
	      mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	    if(duh.hasCode(*pcn)){
	      const string& cuh = duh.getCodeBlock(*pcn).code;
	      print = c.code!=cuh;
	    }
	  }
	  if(print){
	    if(!c.description.empty()){
	      out <<"### Description relative à la portion " << *pcn << " (spécialisée pour l'hypothèse " << ModellingHypothesis::toString(*ph) << ")" << endl;
	      out << c.description << endl << endl;
	    }
	    out <<"### Listing associé à la portion " << *pcn << " (spécialisée pour l'hypothèse " << ModellingHypothesis::toString(*ph) << ")" << endl;
	    out << endl;
	    out << "~~~~~~~ {.cpp} "<< endl;
	    out << c.code << endl;
	    out << "~~~~~~~ "<< endl;
	  }
	}
      }
    }
  } // end of MarkdownBehaviourAnalyser::treatFrenchOutput

  void
  MarkdownBehaviourAnalyser::reset(void)
  {
    this->language="default";
  } // end of MarkdownBehaviourAnalyser::reset
  
  MarkdownBehaviourAnalyser::~MarkdownBehaviourAnalyser()
  {} // end of MFrontBehaviourMarkdownAnalyser::~MFrontBehaviourMarkdownAnalyser

  std::vector<MarkdownBehaviourAnalyser::Data>
  MarkdownBehaviourAnalyser::getData(const BehaviourDescription& mb,
					   const VariableDescriptionContainer&
					   (BehaviourData::* m)(void) const)
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::glossary;
    const Glossary& glossary = Glossary::getGlossary();
    vector<Data> data;
    const set<ModellingHypothesis::Hypothesis>& dh = mb.getDistinctModellingHypotheses();
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=dh.begin();ph!=dh.end();++ph){
      const BehaviourData& d = mb.getBehaviourData(*ph);
      const VariableDescriptionContainer& mdata = (d.*m)();
      VariableDescriptionContainer::const_iterator pv;
      for(pv=mdata.begin();pv!=mdata.end();++pv){
	vector<Data>::iterator pd;
	bool found = false;
	pd = data.begin();
	while((pd!=data.end())&&(!found)){
	  found = pd->name==pv->name;
	  if(!found){
	    ++pd;
	  }
	}
	if(pd==data.end()){
	  data.push_back(Data());
	  pd = data.end();
	  --pd;
	  pd->name         = pv->name;
	  pd->arraySize    = pv->arraySize;
	  pd->type         = pv->type;
	  if(*ph==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	    if(pd->description.empty()){
	      pd->description  = pv->description;
	    } else {
	      pd->description  = pv->description+'\n'+pd->description;
	    }
	  } else {
	    if(pd->description.empty()){
	      pd->description  = ModellingHypothesis::toString(*ph) + " : "+ pv->description;
	    } else {
	      pd->description  += '\n'+ModellingHypothesis::toString(*ph) + " : "+pv->description;
	    }
	  }
	  pd->externalName = d.getExternalName(pv->name);
	  if(pd->description.empty()){
	    if(glossary.contains(pd->externalName)){
	      const GlossaryEntry& e = glossary.getGlossaryEntry(pd->externalName);
	      ostringstream os;
	      if(*ph!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
		os << ModellingHypothesis::toString(*ph) << " : ";
	      }
	      os << e.getShortDescription() << endl;
	      const vector<string>& cd = e.getDescription();
	      for(vector<string>::const_iterator pcd=cd.begin();pcd!=cd.end();++pcd){
		os << *pcd << endl;
	      }
	      pd->description += os.str();
	    }
	  }
	}
	if((pd->name         != pv->name)||
	   (pd->type         != pv->type)||
	   (pd->type         != pv->type)||
	   (pd->externalName != d.getExternalName(pv->name))||
	   (pd->arraySize    != pv->arraySize)){
	  string msg("MarkdownBehaviourAnalyser::getData : ");
	  msg += "inconsistent data across hypothesis for variable '"+pd->name+"'";
	  throw(runtime_error(msg));
	}
	pd->hypotheses.push_back(*ph);
      }
    }
    return data;
  }

  void
  MarkdownBehaviourAnalyser::printData(std::ostream& os,
					     const BehaviourDescription& mb,
					     const std::vector<Data>& data) const
  {
    using namespace std;
    using namespace tfel::material;
    map<string,map<string,string> > translations;
    map<string,string>& en = translations["english"]; 
    map<string,string>& fr = translations["french"]; 
    en["variable name"] = "variable name";
    fr["variable name"] = "nom";
    en["variable type"] = "variable type";
    fr["variable type"] = "type";
    en["array size"]    = "array size";
    fr["array size"]    = "taille";
    en["defined for"]   = "defined for";
    fr["defined for"]   = "définie pour";
    en["description"]   = "description";
    fr["description"]   = "description";
    en["default value"] = "default value";
    fr["default value"] = "valeur par défaut";
    en["default value for"] = "default value for";
    fr["default value for"] = "valeur par défaut pour l'hypothèse de modélsation ";
    const map<string,string>& l = translations[this->language];
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MarkdownBehaviourAnalyser::printData : begin" << endl;
    }
    vector<Data>::const_iterator pd;
    const set<ModellingHypothesis::Hypothesis>& dh = mb.getDistinctModellingHypotheses();
    set<string> cbnames;
    vector<ModellingHypothesis::Hypothesis>::const_iterator pvh;
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=dh.begin();ph!=dh.end();++ph){
      const BehaviourData& d = mb.getBehaviourData(*ph);
      const vector<string>& cn = d.getCodeBlockNames();
      cbnames.insert(cn.begin(),cn.end());
    }
    for(pd=data.begin();pd!=data.end();++pd){
      os << "* " << pd->externalName << ":" << endl;
      if(pd->externalName!=pd->name){
	os << "\t+ " << l.at("variable name") << ": " << pd->name << endl; 
      }
      os << "\t+ " << l.at("variable type") << ": " << pd->type << endl; 
      if(pd->arraySize!=1u){
	os << "\t+ " << l.at("array size") << ": " << pd->arraySize << endl;
      }
      if(pd->hypotheses.size()!=dh.size()){
	os << "\t+ " << l.at("defined for") << " ";
	for(pvh=pd->hypotheses.begin();pvh!=pd->hypotheses.end();){
	  os << ModellingHypothesis::toString(*pvh);
	  if(++pvh!=pd->hypotheses.end()){
	    os << ", ";
	  }
	}
	os << endl;
      }
      if(!pd->description.empty()){
	os << "\t+ " << l.at("description") << " : " << pd->description << endl;
      }
      for(pvh=pd->hypotheses.begin();pvh!=pd->hypotheses.end();++pvh){
	if(mb.isParameterName(*pvh,pd->name)){
	  if(*pvh==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	    os << "\t+ " << l.at("default value") << ": ";
	  } else {
	    os << "\t+ " << l.at("default value for")+" "+ModellingHypothesis::toString(*pvh)+" : ";
	  }
	  if(pd->type=="real"){
	    os << mb.getFloattingPointParameterDefaultValue(*pvh,pd->name);
	  } else if(pd->type=="int"){
	    os << mb.getIntegerParameterDefaultValue(*pvh,pd->name);
	  } else {
	    os << mb.getUnsignedShortParameterDefaultValue(*pvh,pd->name);
	  }
	  os << endl;
	}
      }
      // codes blocks referring to the current variable
      map<string,vector<ModellingHypothesis::Hypothesis> > vcb;
      map<string,vector<ModellingHypothesis::Hypothesis> > dvcb;
      for(set<string>::const_iterator pc=cbnames.begin();pc!=cbnames.end();++pc){
      	for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=dh.begin();ph!=dh.end();++ph){
      	  const BehaviourData& d = mb.getBehaviourData(*ph);
	  const bool b = d.isIntegrationVariableName(pd->name)||d.isExternalStateVariableName(pd->name);
	  if(d.hasCode(*pc)){
	    const CodeBlock& cb = d.getCodeBlock(*pc);
	    if(cb.variables.find(pd->name)!=cb.variables.end()){
	      vcb[*pc].push_back(*ph);
	    }
	    if(b){
	      if(cb.variables.find("d"+pd->name)!=cb.variables.end()){
		dvcb[*pc].push_back(*ph);
	      }
	    }
	  }
	}
      }
      if(getVerboseMode()>=VERBOSE_DEBUG){
	if(!vcb.empty()){
	  map<string,vector<ModellingHypothesis::Hypothesis> >::const_iterator pc;
	  os << "\t+ used in ";
	  for(pc=vcb.begin();pc!=vcb.end();){
	    os << pc->first;
	    if(pc->second.size()!=dh.size()){
	      os << " (";
	      for(pvh=pc->second.begin();pvh!=pc->second.end();){
		os << ModellingHypothesis::toString(*pvh);
		if(++pvh!=pc->second.end()){
		  os << ", ";
		}
	      }
	      os << ")";
	    }
	    if(++pc!=vcb.end()){
	      os << ", ";
	    }
	  }
	  os << endl;
	}
	if(!dvcb.empty()){
	  map<string,vector<ModellingHypothesis::Hypothesis> >::const_iterator pc;
	  os << "\t+ increment (or rate) used in ";
	  for(pc=dvcb.begin();pc!=dvcb.end();){
	    os << pc->first;
	    if(pc->second.size()!=dh.size()){
	      os << " (";
	      for(pvh=pc->second.begin();pvh!=pc->second.end();){
		os << ModellingHypothesis::toString(*pvh);
		if(++pvh!=pc->second.end()){
		  os << ", ";
		}
	      }
	      os << ")";
	    }
	    if(++pc!=dvcb.end()){
	      os << ", ";
	    }
	  }
	  os << endl;
	}
      }
    }
    // bounds
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MarkdownBehaviourAnalyser::printData : end" << endl;
    }
  }
  
} // end of namespace mfront  
