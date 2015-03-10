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
  
  template<typename Map>
  static typename Map::mapped_type
  map_at(const Map& m,
	 const typename Map::key_type& k)
  {
    using namespace std;
    const typename Map::const_iterator p = m.find(k);
    if(p==m.end()){
      string msg("map_at : unknown key '"+k+"'");
      throw(runtime_error(msg));
    }
    return p->second;
  }

  MarkdownBehaviourAnalyser::Data::~Data() noexcept
  {} // end of MarkdownBehaviourAnalyser::Data::~Data

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
      auto& log = getLogStream();
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
      auto& log = getLogStream();
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
      const auto d = tokenize(fd.description,'\n');
      for(const auto& l : d){
	if((l.size()>=2)&&((l)[0]=='*')&&((l)[1]==' ')){
	  out << l.substr(2) << endl;
	} else {
	  out << l << endl;
	}
      }
    } else {
      out << "No description specified";
    }
    out << endl << endl;
    out << "## Mechanical behaviour description" << endl << endl;
    out << "### List of supported Hypotheses" << endl << endl;
    const auto& h = mb.getModellingHypotheses();
    const auto& dh = mb.getDistinctModellingHypotheses();
    for(const auto & elem : h){
      out << "* " <<  ModellingHypothesis::toString(elem);
      if(dh.find(elem)!=dh.end()){
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
      const auto& cn = d.getCodeBlockNames();
      for(const auto & elem : cn){
	const auto& c = d.getCodeBlock(elem);
	if(!c.description.empty()){
	  out <<"### " << elem << " description" << endl;
	  out << c.description << endl << endl;
	}
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  out <<"### " << elem << " listing" << endl;
	  out << endl;
	  out << "~~~~~~~ {.cpp}" << endl;
	  out << c.code << endl;
	  out << "~~~~~~~ " << endl;
	}
      }
    }
    for(const auto & elem : h){
      if(elem!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	const BehaviourData& d =
	  mb.getBehaviourData(elem);
	const auto& cn = d.getCodeBlockNames();
	for(const auto & cn_pcn : cn){
	  bool print = true;
	  const auto& c = d.getCodeBlock(cn_pcn);
	  if(dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS)!=dh.end()){
	    const BehaviourData& duh =
	      mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	    if(duh.hasCode(cn_pcn)){
	      const auto& cuh = duh.getCodeBlock(cn_pcn).code;
	      print = c.code!=cuh;
	    }
	  }
	  if(print){
	    if(!c.description.empty()){
	      out <<"### " << cn_pcn << " description(" << ModellingHypothesis::toString(elem) << ")" << endl;
	      out << c.description << endl << endl;
	    }
	    out <<"### " << cn_pcn << " listing (" << ModellingHypothesis::toString(elem) << ")" << endl;
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
    const auto& h = mb.getModellingHypotheses();
    const auto& dh = mb.getDistinctModellingHypotheses();
    for(const auto & elem : h){
      out << "* " <<  ModellingHypothesis::toString(elem);
      if(dh.find(elem)!=dh.end()){
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
      const auto& cn = d.getCodeBlockNames();
      for(const auto & elem : cn){
	const auto& c = d.getCodeBlock(elem);
	if(!c.description.empty()){
	  out <<"### Description relative à la portion " << elem << endl;
	  out << c.description << endl << endl;
	}
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  out <<"### Listing associée à la portion " << elem << endl;
	  out << endl;
	  out << "~~~~~~~ {.cpp}" << endl;
	  out << c.code << endl;
	  out << "~~~~~~~ " << endl;
	}
      }
    }
    for(const auto & elem : h){
      if(elem!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	const BehaviourData& d =
	  mb.getBehaviourData(elem);
	const auto& cn = d.getCodeBlockNames();
	for(const auto & cn_pcn : cn){
	  bool print = true;
	  const auto& c = d.getCodeBlock(cn_pcn);
	  if(dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS)!=dh.end()){
	    const BehaviourData& duh =
	      mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	    if(duh.hasCode(cn_pcn)){
	      const auto& cuh = duh.getCodeBlock(cn_pcn).code;
	      print = c.code!=cuh;
	    }
	  }
	  if(print){
	    if(!c.description.empty()){
	      out <<"### Description relative à la portion " << cn_pcn << " (spécialisée pour l'hypothèse " << ModellingHypothesis::toString(elem) << ")" << endl;
	      out << c.description << endl << endl;
	    }
	    out <<"### Listing associé à la portion " << cn_pcn << " (spécialisée pour l'hypothèse " << ModellingHypothesis::toString(elem) << ")" << endl;
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
    const auto& glossary = Glossary::getGlossary();
    vector<Data> data;
    const auto& dh = mb.getDistinctModellingHypotheses();
    for(const auto & elem : dh){
      const auto& d = mb.getBehaviourData(elem);
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
	  if(elem==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	    if(pd->description.empty()){
	      pd->description  = pv->description;
	    } else {
	      pd->description  = pv->description+'\n'+pd->description;
	    }
	  } else {
	    if(pd->description.empty()){
	      pd->description  = ModellingHypothesis::toString(elem) + " : "+ pv->description;
	    } else {
	      pd->description  += '\n'+ModellingHypothesis::toString(elem) + " : "+pv->description;
	    }
	  }
	  pd->externalName = d.getExternalName(pv->name);
	  if(pd->description.empty()){
	    if(glossary.contains(pd->externalName)){
	      const auto& e = glossary.getGlossaryEntry(pd->externalName);
	      ostringstream os;
	      if(elem!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
		os << ModellingHypothesis::toString(elem) << " : ";
	      }
	      os << e.getShortDescription() << endl;
	      const auto& cd = e.getDescription();
	      for(const auto & cd_pcd : cd){
		os << cd_pcd << endl;
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
	pd->hypotheses.push_back(elem);
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
    auto& en = translations["english"]; 
    auto& fr = translations["french"]; 
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
    const auto& l = translations[this->language];
    if(getVerboseMode()>=VERBOSE_DEBUG){
      auto& log = getLogStream();
      log << "MarkdownBehaviourAnalyser::printData : begin" << endl;
    }
    vector<Data>::const_iterator pd;
    const auto& dh = mb.getDistinctModellingHypotheses();
    set<string> cbnames;
    vector<ModellingHypothesis::Hypothesis>::const_iterator pvh;
    for(const auto & elem : dh){
      const auto& d = mb.getBehaviourData(elem);
      const auto& cn = d.getCodeBlockNames();
      cbnames.insert(cn.begin(),cn.end());
    }
    for(pd=data.begin();pd!=data.end();++pd){
      os << "* " << pd->externalName << ":" << endl;
      if(pd->externalName!=pd->name){
	os << "\t+ " << map_at(l,"variable name") << ": " << pd->name << endl; 
      }
      os << "\t+ " << map_at(l,"variable type") << ": " << pd->type << endl; 
      if(pd->arraySize!=1u){
	os << "\t+ " << map_at(l,"array size") << ": " << pd->arraySize << endl;
      }
      if(pd->hypotheses.size()!=dh.size()){
	os << "\t+ " << map_at(l,"defined for") << " ";
	for(pvh=pd->hypotheses.begin();pvh!=pd->hypotheses.end();){
	  os << ModellingHypothesis::toString(*pvh);
	  if(++pvh!=pd->hypotheses.end()){
	    os << ", ";
	  }
	}
	os << endl;
      }
      if(!pd->description.empty()){
	os << "\t+ " << map_at(l,"description") << " : " << pd->description << endl;
      }
      for(pvh=pd->hypotheses.begin();pvh!=pd->hypotheses.end();++pvh){
	if(mb.isParameterName(*pvh,pd->name)){
	  if(*pvh==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	    os << "\t+ " << map_at(l,"default value") << ": ";
	  } else {
	    os << "\t+ " << map_at(l,"default value for")+" "+ModellingHypothesis::toString(*pvh)+" : ";
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
      for(const auto & cbname : cbnames){
      	for(const auto & elem : dh){
      	  const auto& d = mb.getBehaviourData(elem);
	  const bool b = d.isIntegrationVariableName(pd->name)||d.isExternalStateVariableName(pd->name);
	  if(d.hasCode(cbname)){
	    const auto& cb = d.getCodeBlock(cbname);
	    if(cb.variables.find(pd->name)!=cb.variables.end()){
	      vcb[cbname].push_back(elem);
	    }
	    if(b){
	      if(cb.variables.find("d"+pd->name)!=cb.variables.end()){
		dvcb[cbname].push_back(elem);
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
      auto& log = getLogStream();
      log << "MarkdownBehaviourAnalyser::printData : end" << endl;
    }
  }
  
} // end of namespace mfront  
