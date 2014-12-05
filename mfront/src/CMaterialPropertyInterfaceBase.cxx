/*!
 * \file   mfront/src/CMaterialPropertyInterfaceBase.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/CMaterialPropertyInterfaceBase.hxx"

namespace mfront
{

  CMaterialPropertyInterfaceBase::CMaterialPropertyInterfaceBase()
  {}

  void
  CMaterialPropertyInterfaceBase::reset(void)
  {
    if(this->headerFile.is_open()){
      this->headerFile.close();
    }
    if(this->srcFile.is_open()){
      this->srcFile.close();
    }
  } // end of CMaterialPropertyInterfaceBase::reset(void)
  
  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CMaterialPropertyInterfaceBase::treatKeyword(const std::string&,
					tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  CMaterialPropertyInterfaceBase::~CMaterialPropertyInterfaceBase()
  {}

  void
  CMaterialPropertyInterfaceBase::writeOutputFiles(const std::string& file,
					    const std::string& ,
					    const std::string& material,
					    const std::string& className,
					    const std::string& author,
					    const std::string& date,
					    const std::string& description,
					    const std::string& includes,
					    const std::string& output,
					    const VariableDescriptionContainer& inputs,
					    const std::vector<std::string>& materialLaws,
					    const std::map<std::string,std::string>&,
					    const std::map<std::string,std::string>&,
					    const StaticVariableDescriptionContainer& staticVars,
					    const std::vector<std::string>& params,
					    const std::map<std::string,double>& paramValues,
					    const LawFunction& function,
					    const std::vector<VariableBoundsDescription>& bounds,
					    const std::vector<VariableBoundsDescription>& physicalBounds,
					    const bool,
					    const std::vector<std::string>&)
  {
    using namespace std;
    string header = this->getHeaderFileName(material,className);
    this->srcFileName     = "src/"     + this->getSrcFileName(material,className)+".cxx";
    // the fortran interface does not need any header...
    if(!header.empty()){
      this->headerFileName  = "include/" + header +".hxx";
      this->headerFile.open(this->headerFileName.c_str());
      if(!this->headerFile){
	string msg("MaterialPropertyDSL::writeOutputFiles : ");
	msg += "unable to open ";
	msg += this->headerFileName;
	msg += " for writing output file.";
	throw(runtime_error(msg));
      }
      this->headerFile.exceptions(ios::badbit|ios::failbit);
      this->writeHeaderFile(material,className,author,date,
			    description,inputs,bounds,physicalBounds);
    }
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MaterialPropertyDSL::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    this->writeSrcFile(file,material,className,author,
		       date,includes,output,
		       inputs,materialLaws,staticVars,
		       params,paramValues,function,
		       bounds,physicalBounds);
  } // end of CMaterialPropertyInterfaceBase::writeOutputFiles

  void
  CMaterialPropertyInterfaceBase::replace(std::string& s,
				   const std::string::value_type a,
				   const std::string::value_type b)
  {
    using namespace std;
    string::size_type p = 0;
    while((p=s.find(a))!=string::npos){
      s[p]=b;
    }
  } // end of CMaterialPropertyInterfaceBase::replace

  std::string
  CMaterialPropertyInterfaceBase::transformHeaderName(const std::string& material,
					       const std::string& className)
  {
    using namespace std;
    string header;
    header = makeUpperCase(this->getHeaderFileName(material,className));
    CMaterialPropertyInterfaceBase::replace(header,'.','_');
    CMaterialPropertyInterfaceBase::replace(header,'-','_');
    CMaterialPropertyInterfaceBase::replace(header,'/','_');
    return header;
  } // end of CMaterialPropertyInterfaceBase::transformHeaderName

  void
  CMaterialPropertyInterfaceBase::writeHeaderFile(const std::string& material,
					   const std::string& className,
					   const std::string& author,
					   const std::string& date,
					   const std::string& description,
					   const VariableDescriptionContainer& inputs,
					   const std::vector<VariableBoundsDescription>& bounds,
					   const std::vector<VariableBoundsDescription>& physicalBounds)
  {
    using namespace std;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << className << " MaterialLaw.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if(!author.empty()){
      this->headerFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->headerFile << "* \\date   " << date       << endl;
    }
    if(!description.empty()){
      this->headerFile << description << endl;
    }
    this->headerFile << " */\n\n";
    this->headerFile << "#ifndef _" 
		     << this->transformHeaderName(material,className)
		     << "_HH\n";
    this->headerFile << "#define _"
		     << this->transformHeaderName(material,className)
		     << "_HH\n\n";
    this->writeHeaderPreprocessorDirectives(material,className);
    this->writeBeginHeaderNamespace();
    this->headerFile << this->getFunctionDeclaration(material,className);
    this->headerFile << "(";
    this->writeParameterList(this->headerFile,inputs);
    this->headerFile << ");\n\n";
    if(((!bounds.empty())||(!physicalBounds.empty()))||
       (this->requiresCheckBoundsFunction())){
      this->headerFile << this->getCheckBoundsFunctionDeclaration(material,className);
      this->headerFile << "(";
      this->writeParameterList(this->headerFile,inputs);
      this->headerFile << ");\n\n";
    }
    this->writeEndHeaderNamespace();
    this->headerFile << "#endif /* _"
		     << this->transformHeaderName(material,className)
		     << "_HH */\n";
    this->headerFile.close();
  } // end of CMaterialPropertyInterfaceBase::writeHeaderFile(void)

  void
  CMaterialPropertyInterfaceBase::writeInterfaceSpecificVariables(const VariableDescriptionContainer&)
  {} // end of CMaterialPropertyInterfaceBase::writeInterfaceSpecificVariables

  void
  CMaterialPropertyInterfaceBase::writeParameterList(std::ostream& file,
					      const VariableDescriptionContainer& inputs){
    VariableDescriptionContainer::const_iterator p;
    if(!inputs.empty()){
      for(p=inputs.begin();p!=inputs.end();){
	file << "const double " << p->name;
	if((++p)!=inputs.end()){
	  file << ",";
	}
      }
    } else {
      file << "void";
    }
  } // end of CMaterialPropertyInterfaceBase::writeParameterList

  void
  CMaterialPropertyInterfaceBase::writeSrcFile(const std::string& file,
					const std::string& material,
					const std::string& className,
					const std::string& author,
					const std::string& date,
					const std::string& includes,
					const std::string& output,
					const VariableDescriptionContainer& inputs,
					const std::vector<std::string>& materialLaws,
					const StaticVariableDescriptionContainer& staticVars,
					const std::vector<std::string>& params,
					const std::map<std::string,double>& paramValues,
					const LawFunction& function,
					const std::vector<VariableBoundsDescription>& bounds,
					const std::vector<VariableBoundsDescription>& physicalBounds)
  {
    using namespace std;
    string header;
    vector<string>::const_iterator p;
    map<string,string>::const_iterator p4;
    std::vector<VariableBoundsDescription>::const_iterator p5;
    map<string,double>::const_iterator p6;
    VariableDescriptionContainer::const_iterator pv;
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << className << " MaterialLaw.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!author.empty()){
      this->srcFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->srcFile << "* \\date   " << date       << endl;
    }
    this->srcFile << " */\n\n";
    this->srcFile << "#include<cmath>\n\n";
    this->srcFile << "#include<algorithm>\n\n";
    if(!includes.empty()){
      this->srcFile << includes << endl << endl;
    }
    header = this->getHeaderFileName(material,className);
    if(!header.empty()){
      this->srcFile << "#include\""+header+".hxx\"\n\n";
    }
    this->writeSrcPreprocessorDirectives(material,className);
    this->writeBeginSrcNamespace();
    this->srcFile << this->getFunctionDeclaration(material,className);
    this->srcFile << "(";
    this->writeParameterList(this->srcFile,inputs);
    this->srcFile << ")\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "typedef double real;\n";
    // material laws
    writeMaterialLaws("CMaterialPropertyInterfaceBase::writeSrcFile",
		      this->srcFile,materialLaws);
    // static variables
    writeStaticVariables("CMaterialPropertyInterfaceBase::writeSrcFile",
			 srcFile,staticVars,file);
    // parameters
    if(!params.empty()){
      for(p=params.begin();p!=params.end();++p){
	p6 = paramValues.find(*p);
	if(p6==paramValues.end()){
	  string msg("CMaterialPropertyInterfaceBase::writeSrcFile : ");
	  msg += "internal error (can't find value of parameter " + *p + ")";
	  throw(runtime_error(msg));
	}
	this->srcFile << "static constexpr double " << *p << " = " << p6->second << ";\n";
      }
    }
    this->writeInterfaceSpecificVariables(inputs);
    this->srcFile << "double " << output << ";\n";
    this->srcFile << function.body;
    this->srcFile << "return " << output << ";\n";
    this->srcFile << "} /* end of " << className << " */\n\n";
    if(((!bounds.empty())||(!physicalBounds.empty()))||
       (this->requiresCheckBoundsFunction())){
      this->srcFile << this->getCheckBoundsFunctionDeclaration(material,className);
      this->srcFile << "(";
      this->writeParameterList(this->srcFile,inputs);
      this->srcFile << ")\n{\n";
      this->writeInterfaceSpecificVariables(inputs);
      for(pv=inputs.begin();pv!=inputs.end();++pv){
	this->srcFile << "static_cast<void>(" << pv->name << ");\n";
      }
      if(!physicalBounds.empty()){
	this->srcFile << "/* treating physical bounds */\n";
	for(p5=physicalBounds.begin();
	    p5!=physicalBounds.end();++p5){
	  if(p5->boundsType==VariableBoundsDescription::Lower){
	    this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  } else if(p5->boundsType==VariableBoundsDescription::Upper){
	    this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  } else {
	    this->srcFile << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			  << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  }
	}
      }
      if(!bounds.empty()){
	this->srcFile << "/* treating standard bounds */\n";
	for(p5=bounds.begin();
	    p5!=bounds.end();++p5){
	  if(p5->boundsType==VariableBoundsDescription::Lower){
	    this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  } else if(p5->boundsType==VariableBoundsDescription::Upper){
	    this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  } else {
	    this->srcFile << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			  << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  }
	}
      }
      this->srcFile << "return 0;\n} /* end of " << className << "_checkBounds */\n";
    }
    this->writeEndSrcNamespace();
    this->srcFile.close();
  } // end of CMaterialPropertyInterfaceBase::writeSrcFile(void)

} // end of namespace mfront
