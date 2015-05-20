/*! 
 * \file  BehaviourDocumentationGenerator.cxx
 * \brief
 * \author Helfer Thomas
 * \date   01 avril 2015
 */

#include<sstream>

#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/AbstractBehaviourDSL.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/BehaviourDescription.hxx"
#include"MFront/BehaviourDocumentationGenerator.hxx"

namespace mfront{

  // map::at() is missing in ekopath standard library
  template<typename Map>
  static typename Map::mapped_type
  map_at(const Map& m,
	 const typename Map::key_type& k)
  {
    using namespace std;
    const auto p = m.find(k);
    if(p==m.end()){
      string msg("map_at : unknown key '"+k+"'");
      throw(runtime_error(msg));
    }
    return p->second;
  }

  /*!
   * internal structure gathering data from mechanical behaviour
   * description
   */
  struct Data
  {
    using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
    Data() = default;
    Data(Data&&) = default;
    Data(const Data&) = default;
    Data& operator=(Data&&) = default;
    Data& operator=(const Data&) = default;
    ~Data() noexcept;
    std::string name;
    std::string type;
    std::string description;
    //! specific description per modelling hypothesis
    std::map<Hypothesis,std::string> descriptions;
    std::string externalName;
    std::vector<Hypothesis> hypotheses;
    unsigned short arraySize;
  };

  static void
  getData(std::vector<Data>& data,
	  const BehaviourDescription& mb,
	  const VariableDescriptionContainer& (BehaviourData::* m)(void) const,
	  const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& mdata = (d.*m)();
    for(auto pv=mdata.begin();pv!=mdata.end();++pv){
      bool found = false;
      auto pd = data.begin();
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
	pd->externalName = d.getExternalName(pv->name);
      } else {
	if((pd->name         != pv->name)||
	   (pd->type         != pv->type)||
	   (pd->externalName != d.getExternalName(pv->name))||
	   (pd->arraySize    != pv->arraySize)){
	  string msg("getData : ");
	  msg += "inconsistent data across hypothesis for variable '"+pd->name+"'";
	  throw(runtime_error(msg));
	}
      }
      if(!pv->description.empty()){
	pd->descriptions[h] = pv->description;
	cout << pv->name << " : " << pv->description << endl;
      }
      pd->hypotheses.push_back(h);
    }
  } // end of 

  static std::vector<Data>
  getData(const BehaviourDescription& mb,
	  const VariableDescriptionContainer& (BehaviourData::* m)(void) const)
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::glossary;
    const auto& glossary = Glossary::getGlossary();
    vector<Data> data;
    const auto& dh = mb.getDistinctModellingHypotheses();
    for(const auto & h : dh){
      getData(data,mb,m,h);
    }
    // description deserves a specific treatment
    for(auto& d:data){
      if(glossary.contains(d.externalName)){
	const auto& e = glossary.getGlossaryEntry(d.externalName);
	ostringstream os;
	os << e.getShortDescription();
	// const auto& cd = e.getDescription();
	// for(const auto & cd_pcd : cd){
	//   if(!cd_pcd.empty()){
	//     os << cd_pcd << endl;	    
	//   }
	// }
	d.description += os.str();
      }
      auto ddc = d.descriptions.size();
      if(ddc==1u){
	auto pd=d.descriptions.begin();
	if(!pd->second.empty()){
	  if(!d.description.empty()){
	    d.description += "\n";
	  }
	  d.description += pd->second;
	}
      } else if(ddc!=0u){
	// Two cases: all descriptions are the same
	bool b = true;
	auto pd=d.descriptions.begin();
	auto pd2=pd;
	advance(pd2,1u);
	for(;(pd2!=d.descriptions.end())&&(b);++pd2){
	  b = pd->second==pd2->second;
	}
	if(b){
	  if(!d.description.empty()){
	    d.description += "\n";
	  }
	  d.description += pd->second;
	} else {
	  for(pd2=pd;(pd2!=d.descriptions.end())&&(b);++pd2){
	    if(!pd2->second.empty()){
	      if(!d.description.empty()){
		d.description += "\n";
	      }
	      if(pd2->first==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
		d.description += "Default Hypothesis : " + pd2->second;
	      } else {
		d.description += ModellingHypothesis::toString(pd2->first) + " : " + pd2->second;
	      }
	    }
	  }
	}
      }
    }
    return data;
  }
  
  static void
  printData(std::ostream& os,
	    const BehaviourDescription& mb,
	    const std::vector<Data>& data,
	    const std::string& language = "english")
  {
    using namespace std;
    using namespace tfel::material;
    auto translations = map<string,map<string,string> >{};
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
    const auto& l = translations[language];
    if(getVerboseMode()>=VERBOSE_DEBUG){
      auto& log = getLogStream();
      log << "printData : begin" << endl;
    }
    const auto& dh = mb.getDistinctModellingHypotheses();
    set<string> cbnames;
    vector<ModellingHypothesis::Hypothesis>::const_iterator pvh;
    for(const auto & elem : dh){
      const auto& d = mb.getBehaviourData(elem);
      const auto& cn = d.getCodeBlockNames();
      cbnames.insert(cn.begin(),cn.end());
    }
    for(const auto& d : data){
      os << "* " << d.externalName << ":" << endl;
      if(d.externalName!=d.name){
	os << "\t+ " << map_at(l,"variable name") << ": " << d.name << endl; 
      }
      os << "\t+ " << map_at(l,"variable type") << ": " << d.type << endl; 
      if(d.arraySize!=1u){
	os << "\t+ " << map_at(l,"array size") << ": " << d.arraySize << endl;
      }
      if(d.hypotheses.size()!=dh.size()){
	os << "\t+ " << map_at(l,"defined for") << " ";
	for(pvh=d.hypotheses.begin();pvh!=d.hypotheses.end();){
	  os << ModellingHypothesis::toString(*pvh);
	  if(++pvh!=d.hypotheses.end()){
	    os << ", ";
	  }
	}
	os << endl;
      }
      if(!d.description.empty()){
	os << "\t+ " << map_at(l,"description") << ": " << d.description << endl;
      }
      for(pvh=d.hypotheses.begin();pvh!=d.hypotheses.end();++pvh){
	if(mb.isParameterName(*pvh,d.name)){
	  if(*pvh==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	    os << "\t+ " << map_at(l,"default value") << ": ";
	  } else {
	    os << "\t+ " << map_at(l,"default value for")+" "+ModellingHypothesis::toString(*pvh)+" : ";
	  }
	  if(d.type=="real"){
	    os << mb.getFloattingPointParameterDefaultValue(*pvh,d.name);
	  } else if(d.type=="int"){
	    os << mb.getIntegerParameterDefaultValue(*pvh,d.name);
	  } else {
	    os << mb.getUnsignedShortParameterDefaultValue(*pvh,d.name);
	  }
	  os << endl;
	}
      }
      // codes blocks referring to the current variable
      auto vcb  = map<string,vector<ModellingHypothesis::Hypothesis> >{};
      auto dvcb = map<string,vector<ModellingHypothesis::Hypothesis> >{};
      for(const auto & cbname : cbnames){
      	for(const auto & elem : dh){
      	  const auto& bd = mb.getBehaviourData(elem);
	  const bool b = bd.isIntegrationVariableName(d.name)||bd.isExternalStateVariableName(d.name);
	  if(bd.hasCode(cbname)){
	    const auto& cb = bd.getCodeBlock(cbname);
	    if(cb.variables.find(d.name)!=cb.variables.end()){
	      vcb[cbname].push_back(elem);
	    }
	    if(b){
	      if(cb.variables.find("d"+d.name)!=cb.variables.end()){
		dvcb[cbname].push_back(elem);
	      }
	    }
	  }
	}
      }
      if(getVerboseMode()>=VERBOSE_DEBUG){
	if(!vcb.empty()){
	  os << "\t+ used in ";
	  for(auto pc=vcb.begin();pc!=vcb.end();){
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
	  os << "\t+ increment (or rate) used in ";
	  for(auto pc=dvcb.begin();pc!=dvcb.end();){
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
      log << "printData : end" << endl;
    }
  }
  
  Data::~Data() noexcept
  {} // end of MarkdownBehaviourAnalyser::Data::~Data

  BehaviourDocumentationGenerator::BehaviourDocumentationGenerator(const int argc,
								   const char *const *const argv,
								   std::shared_ptr<AbstractBehaviourDSL> d,
								   const std::string& f)
    : tfel::utilities::ArgumentParserBase<BehaviourDocumentationGenerator>(argc,argv),
      dsl(d),
      file(f),
      otype(BehaviourDocumentationGenerator::FULL)
  {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    // registring interfaces
    if(!this->interfaces.empty()){
      dsl->setInterfaces(this->interfaces);
    }
  } // end of BehaviourDocumentationGenerator::BehaviourDocumentationGenerator

  void
  BehaviourDocumentationGenerator::registerCommandLineCallBacks(void){
    using namespace std;
    using Parser = tfel::utilities::ArgumentParserBase<BehaviourDocumentationGenerator>;
    Parser::registerNewCallBack("--verbose",&BehaviourDocumentationGenerator::treatVerbose,
				"set verbose output",true);
    Parser::registerNewCallBack("--include","-I",&BehaviourDocumentationGenerator::treatSearchPath,
				"add a new path at the beginning of the search paths",true);
    Parser::registerNewCallBack("--search-path",&BehaviourDocumentationGenerator::treatSearchPath,
				"add a new path at the beginning of the search paths",true);
    Parser::registerNewCallBack("--web",&BehaviourDocumentationGenerator::treatWeb,
				"output a web version of the file");
  } // end of BehaviourDocumentationGenerator::registerCommandLineCallBacks
  
  void BehaviourDocumentationGenerator::treatUnknownArgument(void)
  {
    if(!MFrontBase::treatUnknownArgumentBase()){
#if not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      ArgumentParserBase<BehaviourDocumentationGenerator>::treatUnknownArgument();
#else
      cerr << "mfront : unsupported option '" << a << "'\n";
      exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  } // end of BehaviourDocumentationGenerator::treatUnknownArgument

  const tfel::utilities::Argument&
  BehaviourDocumentationGenerator::getCurrentCommandLineArgument() const{
    return *(this->currentArgument);
  }

  std::string
  BehaviourDocumentationGenerator::getVersionDescription(void) const{
    return MFrontHeader::getHeader();
  } // end of BehaviourDocumentationGenerator::getVersionDescription

  std::string 
  BehaviourDocumentationGenerator::getUsageDescription(void) const
  {
    using namespace std;
    string usage("Usage : ");
    usage += this->programName;
    usage += " [options] [files]";
    return usage;
  } // end of BehaviourDocumentationGenerator::getUsageDescription

  void
  BehaviourDocumentationGenerator::treatWeb(void)
  {
    using namespace std;
    if(this->otype!=FULL){
      throw(runtime_error("BehaviourDocumentationGenerator::treatWeb: "
			  "output type already specified"));
    }
    this->otype=WEB;
  } // end of BehaviourDocumentationGenerator::treatWeb

  

  void
  BehaviourDocumentationGenerator::exe(void)
  {
    using namespace std;
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      getLogStream() << "Treating file '" << this->file << "'" << endl;
    }
    // analysing the file
    this->dsl->analyseFile(this->file,this->ecmds);
    const auto& fd = this->dsl->getFileDescription();
    const auto& mb = this->dsl->getBehaviourDescription();
    if(getVerboseMode()>=VERBOSE_DEBUG){
      auto& log = getLogStream();
      log << "BehaviourDocumentationGenerator::exe : begin" << endl;
    }
    const auto name = (!mb.getLibrary().empty()) ? mb.getLibrary() + mb.getClassName() : mb.getClassName();
    ofstream out(name+".md");
    out.exceptions(ios::badbit|ios::failbit);
    if(!out){
      string msg("BehaviourDocumentationGenerator::exe : ");
      msg += "could not open file ";
      msg += "'src/"+name+".txt'";
      throw(runtime_error(msg));
    }
    if(this->otype==FULL){
      this->writeFullOutput(out,mb,fd);
    } else if(this->otype==WEB){
      this->writeWebOutput(out,mb,fd);
    } else {
      throw(runtime_error("BehaviourDocumentationGenerator::exe: "
			  "unsupported output type"));
    }
    out.close();
    if(getVerboseMode()>=VERBOSE_DEBUG){
      auto& log = getLogStream();
      log << "BehaviourDocumentationGenerator::exe : end" << endl;
    }
  } // end of BehaviourDocumentationGenerator::exe

  void
  BehaviourDocumentationGenerator::writeWebOutput(std::ostream& out,
						  const BehaviourDescription& mb,
						  const FileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::utilities;
    ifstream f(this->file);
    if(!f){
      string msg("BehaviourDocumentationGenerator::writeWebOutput: "
		 "can't open file '"+this->file+"'");
      throw(runtime_error(msg));
    }
    out << "# " << mb.getClassName() << " behaviour description" << endl << endl;
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
    if(mb.hasAttribute(BehaviourData::algorithm)){
      out << "* algorithme: " << mb.getAttribute<string>(BehaviourData::algorithm) << endl;
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
    out << endl;
    out << endl;
    out << "~~~~ {#" << mb.getClassName() << " .cpp .numberLines}" << endl;
    while(!f.eof()){
      string line;
      getline(f,line);
      out << line << endl;
    }
    out << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    out << endl;

  } // end of BehaviourDocumentationGenerator::writeWebOutput

  void
  BehaviourDocumentationGenerator::writeFullOutput(std::ostream& out,
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
  } // end of BehaviourDocumentationGenerator::writeFullOutput

  BehaviourDocumentationGenerator::~BehaviourDocumentationGenerator()
  {} // end of BehaviourDocumentationGenerator::~BehaviourDocumentationGenerator

} // end of namespace mfront
