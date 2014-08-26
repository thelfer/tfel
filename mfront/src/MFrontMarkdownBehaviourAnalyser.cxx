/*! 
 * \file  MFrontMarkdownBehvaviourAnalyser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 mai 2014
 */

#include<iostream>
#include<fstream>

#include"TFEL/Utilities/StringAlgorithms.hxx"

#include"MFront/MFrontLogStream.hxx"
#include"MFront/MechanicalBehaviourDescription.hxx"
#include"MFront/MFrontFileDescription.hxx"
#include"MFront/MFrontMarkdownBehaviourAnalyser.hxx"

namespace mfront
{

  std::string
  MFrontMarkdownBehaviourAnalyser::getName(void)
  {
    return "Markdown";
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontMarkdownBehaviourAnalyser::treatKeyword(const std::string&,
						tfel::utilities::CxxTokenizer::TokensContainer::const_iterator p,
						const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    return std::make_pair(false,p);
  }

  void
  MFrontMarkdownBehaviourAnalyser::endTreatement(const MechanicalBehaviourDescription& mb,
						 const MFrontFileDescription& mf) const
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::utilities;
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MFrontMarkdownBehaviourAnalyser::endTreatement : begin" << endl;
    }
    string name;
    if(!mb.getLibrary().empty()){
      name += mb.getLibrary();
    }
    name += mb.getClassName();
    ofstream out(("src/"+name+".txt").c_str());
    if(!out){
      string msg("MFrontMarkdownBehaviourAnalyser::endTreatement : ");
      msg += "could not open file ";
      msg += "'src/"+name+".txt'";
      throw(runtime_error(msg));
    }
    out << "# " << mb.getClassName() << " behaviour description" << endl << endl;
    out << "## Source file" << endl << endl;
    out << "* file   : " << mf.fileName   << endl;
    out << "* author : ";
    if(!mf.authorName.empty()){
      out << mf.authorName << endl;
    } else {
      out << "(unspecified)" << endl;
    }
    out << "* date   : ";
    if(!mf.date.empty()){
      out << mf.date;
    } else {
      out << "(unspecified)";
    }
    out << endl << endl;
    if(!mf.description.empty()){
      vector<string> d = tokenize(mf.description,'\n');
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
    printData(out,mb,getData(mb,&MechanicalBehaviourData::getMaterialProperties));
    out << endl;
    out << "### State variables " << endl << endl;
    printData(out,mb,getData(mb,&MechanicalBehaviourData::getPersistentVariables));
    out << endl;
    out << "### External state variables " << endl << endl;
    printData(out,mb,getData(mb,&MechanicalBehaviourData::getExternalStateVariables));
    out << endl;
    out << "### Local variables " << endl << endl;
    printData(out,mb,getData(mb,&MechanicalBehaviourData::getLocalVariables));
    out << endl;
    if(mb.hasParameters()){
      out << "### Parameters " << endl << endl;
      printData(out,mb,getData(mb,&MechanicalBehaviourData::getParameters));
    }
    out << endl;
    out << "## Code blocks" << endl << endl;
    if(dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS)!=dh.end()){
      const MechanicalBehaviourData& d =
	mb.getMechanicalBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      const vector<string>& cn = d.getCodeBlockNames();
      for(vector<string>::const_iterator pcn = cn.begin();pcn!=cn.end();++pcn){
	const CodeBlock& c = d.getCode(*pcn);
	if(!c.description.empty()){
	  out <<"### " << *pcn << " description" << endl;
	  out << c.description << endl << endl;
	}
	out <<"### " << *pcn << " listing" << endl;
	out << endl;
	out << "~~~~~~~ {.cpp}" << endl;
	out << c.code << endl;
	out << "~~~~~~~ " << endl;
      }
    }
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(*ph!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	const MechanicalBehaviourData& d =
	  mb.getMechanicalBehaviourData(*ph);
	const vector<string>& cn = d.getCodeBlockNames();
	for(vector<string>::const_iterator pcn = cn.begin();pcn!=cn.end();++pcn){
	  bool print = true;
	  const string& c = d.getCode(*pcn).code;
	  if(dh.find(ModellingHypothesis::UNDEFINEDHYPOTHESIS)!=dh.end()){
	    const MechanicalBehaviourData& duh =
	      mb.getMechanicalBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
	    if(duh.hasCode(*pcn)){
	      const string& cuh = duh.getCode(*pcn).code;
	      print = c!=cuh;
	    }
	  }
	  if(print){
	    out <<"### " << *pcn << " listing (" << ModellingHypothesis::toString(*ph) << ")" << endl;
	    out << endl;
	    out << "~~~~~~~ {.cpp} "<< endl;
	    out << c << endl;
	    out << "~~~~~~~ "<< endl;
	  }
	}
      }
    }    
    out.close();
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MFrontMarkdownBehaviourAnalyser::endTreatement : end" << endl;
    }
  } // end of MFrontMarkdownBehaviourAnalyser::endTreatement

  void
  MFrontMarkdownBehaviourAnalyser::reset(void)
  {} // end of MFrontMarkdownBehaviourAnalyser::reset
  
  MFrontMarkdownBehaviourAnalyser::~MFrontMarkdownBehaviourAnalyser()
  {} // end of MFrontBehaviourMarkdownAnalyser::~MFrontBehaviourMarkdownAnalyser

  std::vector<MFrontMarkdownBehaviourAnalyser::Data>
  MFrontMarkdownBehaviourAnalyser::getData(const MechanicalBehaviourDescription& mb,
					   const VariableDescriptionContainer&
					   (MechanicalBehaviourData::* m)(void) const)
  {
    using namespace std;
    using namespace tfel::material;
    vector<Data> data;
    const set<ModellingHypothesis::Hypothesis>& dh = mb.getDistinctModellingHypotheses();
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=dh.begin();ph!=dh.end();++ph){
      const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(*ph);
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
	  pd->glossaryName = d.getGlossaryName(pv->name);
	}
	if((pd->name         != pv->name)||
	   (pd->type         != pv->type)||
	   (pd->type         != pv->type)||
	   (pd->glossaryName != d.getGlossaryName(pv->name))||
	   (pd->arraySize    != pv->arraySize)){
	  string msg("MFrontMarkdownBehaviourAnalyser::getData : ");
	  msg += "inconsistent data across hypothesis for variable '"+pd->name+"'";
	  throw(runtime_error(msg));
	}
	pd->hypotheses.push_back(*ph);
      }
    }
    return data;
  }

  void
  MFrontMarkdownBehaviourAnalyser::printData(std::ostream& os,
					     const MechanicalBehaviourDescription& mb,
					     const std::vector<Data>& data)
  {
    using namespace std;
    using namespace tfel::material;
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MFrontMarkdownBehaviourAnalyser::printData : begin" << endl;
    }
    vector<Data>::const_iterator pd;
    const set<ModellingHypothesis::Hypothesis>& dh = mb.getDistinctModellingHypotheses();
    set<string> cbnames;
    vector<ModellingHypothesis::Hypothesis>::const_iterator pvh;
    for(set<ModellingHypothesis::Hypothesis>::const_iterator ph=dh.begin();ph!=dh.end();++ph){
      const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(*ph);
      const vector<string>& cn = d.getCodeBlockNames();
      cbnames.insert(cn.begin(),cn.end());
    }
    for(pd=data.begin();pd!=data.end();++pd){
      os << "* " << pd->glossaryName << ":" << endl;
      
      if(pd->glossaryName!=pd->name){
	os << "\t+ variable name : " << pd->name << endl; 
      }
      os << "\t+ variable type : " << pd->type << endl; 
      if(pd->arraySize!=1u){
	os << "\t+ array size : " << pd->arraySize << endl;
      }
      if(pd->hypotheses.size()!=dh.size()){
	os << "\t+ defined for ";
	for(pvh=pd->hypotheses.begin();pvh!=pd->hypotheses.end();){
	  os << ModellingHypothesis::toString(*pvh);
	  if(++pvh!=pd->hypotheses.end()){
	    os << ", ";
	  }
	}
	os << endl;
      }
      if(!pd->description.empty()){
	os << "\t+ description : " << pd->description << endl;
      }
      for(pvh=pd->hypotheses.begin();pvh!=pd->hypotheses.end();++pvh){
	if(mb.isParameterName(*pvh,pd->name)){
	  if(*pvh==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	    os << "\t+ default value: ";
	  } else {
	    os << "\t+ default value for "+ModellingHypothesis::toString(*pvh)+": ";
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
      	  const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(*ph);
	  const bool b = d.isIntegrationVariableName(pd->name)||d.isExternalStateVariableName(pd->name);
	  if(d.hasCode(*pc)){
	    const CodeBlock& cb = d.getCode(*pc);
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
      //      os << endl;
    }
    // bounds
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MFrontMarkdownBehaviourAnalyser::printData : end" << endl;
    }
  }
  
} // end of namespace mfront  
