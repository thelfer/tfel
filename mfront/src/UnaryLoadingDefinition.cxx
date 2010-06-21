/*!
 * \file   UnaryLoadingDefinition.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 fév 2007
 */

#include<iostream>
#include<stdexcept>
#include<sstream>
#include<cmath>
#include<utility>
#include<limits>
#include<iterator>
#include<algorithm>

#include"MFront/ParserUtilities.hxx"
#include"MFront/UnaryLoadingDefinition.hxx"

namespace mfront{

  const long double UnaryLoadingDefinition::Cst_1_Sqrt2 = std::sqrt(2.L);

  UnaryLoadingDefinition::UnaryLoadingDefinition(const std::string&   testType_,
						 const unsigned short dimension_,
						 const std::string&   numericType_,
						 const bool           useQt_)
    : testType(testType_),dimension(dimension_),
      numericType(numericType_),useQt(useQt_),
      isTemperatureInterpolationTypeDefined(false),
      isLoadingInterpolationTypeDefined(false)
  {
    using namespace std;

    if((this->testType!="\"sigxx\"")&&
       (this->testType!="\"sigyy\"")&&
       (this->testType!="\"sigzz\"")&&
       (this->testType!="\"sigxy\"")&&
       (this->testType!="\"sigxz\"")&&
       (this->testType!="\"sigyz\"")&&
       (this->testType!="\"etoxx\"")&&
       (this->testType!="\"etoyy\"")&&
       (this->testType!="\"etozz\"")&&
       (this->testType!="\"etoxy\"")&&
       (this->testType!="\"etoxz\"")&&       
       (this->testType!="\"etoyz\"")&&
       (this->testType!="\"Ux\"")&&
       (this->testType!="\"Uy\"")&&       
       (this->testType!="\"Uz\"")){
      string msg("UnaryLoadingDefinition::UnaryLoadingDefinition : internal error.\n");
      msg += " unsupported loading type ";
      msg += this->testType;
      throw(runtime_error(msg));
    }
    this->testVariable = this->testType.substr(1,this->testType.size()-2);
  } // end of UnaryLoadingDefinition::UnaryLoadingDefinition

  template<typename T>
  T
  UnaryLoadingDefinition::read(UnaryLoadingDefinition::TokensIterator& current,
			       const UnaryLoadingDefinition::TokensIterator end)
  {
    using namespace std;
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "'finalTime' has not been defined.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    T res;
    istringstream converter(current->value);
    converter >> res;
    if(converter.fail()){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "cannot read value.\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(!converter.eof()){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "cannot read value.\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    ++(current);
    return res;
  } // end of UnaryLoadingDefinition::read
  
  const std::string& 
  UnaryLoadingDefinition::getTestType(void) const
  {
    return this->testType;
  }

  unsigned short 
  UnaryLoadingDefinition::getDimension(void) const
  {
    return this->dimension;
  }

  const std::string& 
  UnaryLoadingDefinition::getNumericType(void) const
  {
    return this->numericType;
  }

  bool
  UnaryLoadingDefinition::getUseQt(void) const
  {
    return this->useQt;
  }

  const std::vector<long double>&
  UnaryLoadingDefinition::getTimes(void) const
  {
    using namespace std;
    return this->times;
  } // end of UnaryLoadingDefinition::getTimes

  const std::map<std::string,long double>&
  UnaryLoadingDefinition::getCoefficientsValues(void) const
  {
    using namespace std;
    return this->coefValues;
  } // end of UnaryLoadingDefinition::getCoefficientsValues

  const std::map<std::string,long double>&
  UnaryLoadingDefinition::getInternalStateVariablesValues(void) const
  {
    using namespace std;
    return this->stateVarValues;
  } // end of UnaryLoadingDefinition::getInternalStateVariablesValues

  UnaryLoadingDefinition::TokensIterator
  UnaryLoadingDefinition::treatParticularInput(const std::string& name,
					       UnaryLoadingDefinition::TokensIterator current,
					       const UnaryLoadingDefinition::TokensIterator end)
  {
    using namespace std;
    map<long double,long double> *pm;
    InterpolationFlag *pi;
    bool *pb;
    if(this->times.empty()){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "'times' has not been defined.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if (name=="T"){
      pm=&this->temperatureValues;
      pi=&this->temperatureInterpolationType;
      pb=&this->isTemperatureInterpolationTypeDefined;
    } else if (name!=this->testVariable){
      pm=&this->loadingValues;
      pi=&this->loadingInterpolationType;
      pb=&this->isLoadingInterpolationTypeDefined;
    } else {
      string msg("UnaryLoadingDefinition::treatParticularInput : internal error.\n");
      msg += " unsupported input ";
      msg += name;
      throw(runtime_error(msg));
    }
    if(current->value=="."){
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="setInterpolationType"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "expected 'setInterpolationType', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="("){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "expected '(', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value=="\"ConstantPerInterval\""){
	if(*pb){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatParticularInput : ";
	  msg << "interpolation type has already been set for variable " 
	      << name << ".\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	*pb = true;
	*pi = ConstantPerInterval;
      } else if(current->value=="\"LinearPerInterval\""){
	if(*pb){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatParticularInput : ";
	  msg << "interpolation type has already been set for variable " 
	      << name << ".\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	*pb = true;
	*pi = LinearPerInterval;
      } else {
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "unsupported interpolation type (read "<< current->value << ").";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=")"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "expected ')', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=";"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatParticularInput : ";
	msg << "expected ')', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      return current;
    }
    if(!*pb){
      *pi = ConstantPerInterval;
    }
    if(current->value!="["){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "expected '[', (read " << current->value << ")\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    ++current;
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "unexpected end of file.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    istringstream iconverter(current->value);
    long double time;
    iconverter >> time;
    if(iconverter.fail()){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "cannot read time definition for input " << name << ".\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(!iconverter.eof()){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "cannot read time definition for input " << name
	  << " (parsing failed).\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    ++current;
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "unexpected end of file.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(current->value!="]"){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "expected ']', (read " << current->value << ")\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    ++current;
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "unexpected end of file.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(current->value!="="){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "expected '=', (read " << current->value << ")\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    ++current;
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "unexpected end of file.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    istringstream converter(current->value);
    long double value;
    converter >> value;
    if(converter.fail()){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "cannot read value for input " << name << ".\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(!converter.eof()){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "cannot read value for input " << name 
	  << " (parsing failed).\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(!(pm->insert(make_pair(time,value))).second){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "value for input " << name 
	  << " already defined at time " << time << ".\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));	
    }
    ++current;
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "unexpected end of file.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(current->value!=";"){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatParticularInput : ";
      msg << "expected ';', (read " << current->value << ")\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    ++current;
    return current;
  } // end of UnaryLoadingDefinition::treatParticularInput

  UnaryLoadingDefinition::TokensIterator
  UnaryLoadingDefinition::treatTimes(UnaryLoadingDefinition::TokensIterator current,
				     const UnaryLoadingDefinition::TokensIterator end)
  {
    using namespace std;
    long double t;
    long double dt;
    long double time;
    if(current==end){
      --(current);
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected end of line (expected '=').\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    if(current->value!="="){
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected token (expected '=', read "+current->value+").\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    ++(current);
    if(current==end){
      --(current);
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected end of line (expected '{').\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    if(current->value!="{"){
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected token (expected '{', read "+current->value+").\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    ++(current);
    if(current==end){
      --(current);
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected end of line (expected initial time value).\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    if(current->value=="}"){
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="Time is empty.\n";
      msg+="Error at line : ";
      msg+=toString(current->line);
      throw(runtime_error(msg));
    }
    this->times.push_back(this->read<double>(current,end));
    if(current==end){
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected end of file while reading Time definition.\n";
      throw(runtime_error(msg));
    }
    if(current->value!=","){
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected token (expected ',', read "+current->value+").\n";
      throw(runtime_error(msg));
    }
    ++(current);
    if(current==end){
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected end of file while reading Time definition.\n";
      throw(runtime_error(msg));
    }
    while(current->value!="}"){
      time = this->read<double>(current,end);
      if(time<this->times.back()+100*numeric_limits<double>::epsilon()){
	--(current);
	string msg("UnaryLoadingDefinition::treatTimes : ");
	msg+="time given is less than previous given time.\n";
	msg+="Error at line : ";
	msg+=toString(current->line);
	throw(runtime_error(msg));
      }
      if(current==end){
	string msg("UnaryLoadingDefinition::treatTimes : ");
	msg+="unexpected end of file while reading Time definition.\n";
	throw(runtime_error(msg));
      }
      if((current->value==":")||
	 current->value=="in"){
	// treating discretisation through a given number of intervals
	++(current);
	if(current==end){
	  string msg("UnaryLoadingDefinition::treatTimes : ");
	  msg+="unexpected end of file while reading Time definition.\n";
	  throw(runtime_error(msg));
	}
	unsigned short nb = this->read<unsigned short>(current,end);
	if(current==end){
	  string msg("UnaryLoadingDefinition::treatTimes : ");
	  msg+="unexpected end of file while reading Time definition.\n";
	  throw(runtime_error(msg));
	}
	t  = this->times.back();
	dt = (time-t)/nb;
	t = t+dt;
	for(;t<time-dt*0.5;t+=dt){
	  this->times.push_back(t);
	}
      } else if((current->value=="/")||
		(current->value=="by")){
	// treating discretisation through a given time step
	++(current);
	if(current==end){
	  string msg("UnaryLoadingDefinition::treatTimes : ");
	  msg+="unexpected end of file while reading Time definition.\n";
	  throw(runtime_error(msg));
	}
	dt = this->read<double>(current,end);
	if(current==end){
	  string msg("UnaryLoadingDefinition::treatTimes : ");
	  msg+="unexpected end of file while reading Time definition.\n";
	  throw(runtime_error(msg));
	}
	t  = this->times.back()+dt;
	for(;t<time-dt*0.5;t+=dt){
	  this->times.push_back(t);
	}
      }
      this->times.push_back(time);
      if(current->value==","){
	++(current);
	if(current==end){
	  string msg("UnaryLoadingDefinition::treatTimes : ");
	  msg+="unexpected end of file while reading Time definition.\n";
	  throw(runtime_error(msg));
	}
      } else if(current->value!="}"){
	string msg("UnaryLoadingDefinition::treatTimes : ");
	msg+="unexpected token (expected ',' or '}', read "+current->value+").\n";
	throw(runtime_error(msg));
      }
    }
    ++(current);
    if(current==end){
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected end of file (expected ';').\n";
      throw(runtime_error(msg));
    }
    if(current->value!=";"){
      string msg("UnaryLoadingDefinition::treatTimes : ");
      msg+="unexpected token (expected ';', read "+current->value+").\n";
      throw(runtime_error(msg));
    }
    ++(current);
    return current;
  } // end of UnaryLoadingDefinition::treatTimes
  
  UnaryLoadingDefinition::TokensIterator
  UnaryLoadingDefinition::treatCoef(const std::string& coefName,
				    const SupportedTypes::TypeFlag flag,
				    UnaryLoadingDefinition::TokensIterator current,
				    const UnaryLoadingDefinition::TokensIterator end)
  {
    using namespace std;
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatCoef : ";
      msg << "unexpected end of file.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(flag==SupportedTypes::Scalar){
      if(current->value!="="){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "expected '=', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      istringstream converter(current->value);
      long double tmp;
      converter >> tmp;
      if(converter.fail()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "cannot read value for coefficient " << coefName << ".\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!converter.eof()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "cannot read value for coefficient " << coefName 
	    << " (parsing failed).\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!coefValues.insert(make_pair(coefName,tmp)).second){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "value for coefficient " << coefName 
	    << " already defined.\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));	
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=";"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "expected ';', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
    } else if(flag==SupportedTypes::Stensor){
      bool allComponents = false;
      unsigned short componentNbr = 0u;
      long double value;
      if(current->value!="("){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "expected '(', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value=="*"){
	allComponents = true;
      } else {
	istringstream converter(current->value);
	converter >> componentNbr;
	if(converter.fail()){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatCoef : ";
	  msg << "cannot read value for component number\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(!converter.eof()){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatCoef : ";
	  msg << "cannot read value for component number (parsing failed).\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(componentNbr>5){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatCoef : ";
	  msg << "invalid index for stensor (read "<< componentNbr << ").\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(componentNbr==3){
	  if((dimension!=2)||(dimension!=3)){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatCoef : ";
	    msg << componentNbr << "is not a valid index for stensor in dimension "
		<< dimension << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));
	  }
	}
	if((componentNbr==4)||(componentNbr==5)){
	  if(dimension!=3){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatCoef : ";
	    msg << componentNbr << "is not a valid index for stensor in dimension "
		<< dimension << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));
	  }
	}
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=")"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "expected ')', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="="){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "expected '=', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      istringstream converter(current->value);
      ostringstream oconverter;
      converter >> value;
      if(converter.fail()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "cannot read value for stensor " << coefName;
	msg << "\nError at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!converter.eof()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "cannot read value for stensor " << coefName << " (parsing failed).\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=";"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatCoef : ";
	msg << "expected ';', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++(current);
      if(allComponents){
	unsigned short i;
	for(i=0;i<3u;++i){
	  oconverter.str("");
	  oconverter << "(" << i << ")";
	  string coefName2 = coefName + oconverter.str();
	  if(!coefValues.insert(make_pair(coefName2,value)).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatCoef : ";
	    msg << "value for coefficient " << coefName2 
		<< " already defined.\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	}
	if((this->dimension==2u)||(this->dimension==3u)){
	  oconverter.str("");
	  oconverter << "(" << 3u << ")";
	  string coefName2 = coefName + oconverter.str();
	  value*=UnaryLoadingDefinition::Cst_1_Sqrt2;
	  if(!coefValues.insert(make_pair(coefName2,value)).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatCoef : ";
	    msg << "value for coefficient " << coefName2 
		<< " already defined.\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	}
	if(this->dimension==3u){
	  oconverter.str("");
	  oconverter << "(" << 4u << ")";
	  string coefName2 = coefName + oconverter.str();
	  if(!coefValues.insert(make_pair(coefName2,value)).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatCoef : ";
	    msg << "value for coefficient " << coefName2 
		<< " already defined.\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	  oconverter.str("");
	  oconverter << "(" << 5u << ")";
	  coefName2 = coefName + oconverter.str();
	  if(!coefValues.insert(make_pair(coefName2,value)).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatCoef : ";
	    msg << "value for coefficient " << coefName2 
		<< " already defined.\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	}
      } else {
	oconverter.str("");
	oconverter << "(" << componentNbr << ")";
	string coefName2 = coefName + oconverter.str();
	if(!coefValues.insert(make_pair(coefName2,value)).second){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatCoef : ";
	  msg << "value for coefficient " << coefName2 
	      << " already defined.\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));	
	}
      }
    } else {
      ostringstream msg;
      --current;
      msg << "UnaryLoadingDefinition::treatCoef : ";
      msg << "Internal error, unsupported type.\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    return current;
  } // end of UnaryLoadingDefinition::treatCoef

  UnaryLoadingDefinition::TokensIterator
  UnaryLoadingDefinition::treatStateVar(const std::string& coefName,
					const SupportedTypes::TypeFlag flag,
					UnaryLoadingDefinition::TokensIterator current,
					const UnaryLoadingDefinition::TokensIterator end)
  {
    using namespace std;
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatStateVar : ";
      msg << "unexpected end of file.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(this->times.empty()){
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatStateVar : ";
      msg << "'times' has not been defined.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(flag==SupportedTypes::Scalar){
      if(current->value!="["){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected '[', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      long double tmp = this->read<double>(current,end);
      if(abs(times[0]-tmp)>10.*numeric_limits<long double>::min()){
	--(current);
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "state variable may only be defined at intial time, (read " 
	    << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="]"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected ']', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="="){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected '=', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      tmp = this->read<long double>(current,end);
      if(abs(times[0]-tmp)>10.*numeric_limits<long double>::min()){
	--(current);
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "state variable may only be defined at intial time, (read " 
	    << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!stateVarValues.insert(make_pair(coefName,tmp)).second){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "value for state variable " << coefName 
	    << " already defined.\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));	
      }
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=";"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected ';', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
    } else if(flag==SupportedTypes::Stensor){
      bool allComponents = false;
      unsigned short componentNbr = 0u;
      long double value;
      if(current->value!="("){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected '(', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value=="*"){
	allComponents = true;
      } else {
	istringstream converter(current->value);
	converter >> componentNbr;
	if(converter.fail()){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatStateVar : ";
	  msg << "cannot read value for component number\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(!converter.eof()){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatStateVar : ";
	  msg << "cannot read value for component number (parsing failed).\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(componentNbr>5){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatStateVar : ";
	  msg << "invalid index for stensor (read "<< componentNbr << ").\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(componentNbr==3){
	  if((dimension!=2)||(dimension!=3)){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatStateVar : ";
	    msg << componentNbr << "is not a valid index for stensor in dimension "
		<< dimension << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));
	  }
	}
	if((componentNbr==4)||(componentNbr==5)){
	  if(dimension!=3){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatStateVar : ";
	    msg << componentNbr << "is not a valid index for stensor in dimension "
		<< dimension << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));
	  }
	}
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=")"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected ')', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="["){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected '[', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      long double tmp = this->read<long double>(current,end);
      if(abs(times[0]-tmp)>10.*numeric_limits<long double>::min()){
	--(current);
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "state variable may only be defined at intial time, (read " 
	    << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="]"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected ']', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="="){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected '=', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      istringstream converter(current->value);
      ostringstream oconverter;
      converter >> value;
      if(converter.fail()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "cannot read value for stensor " << coefName;
	msg << "\nError at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!converter.eof()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "cannot read value for stensor " << coefName << " (parsing failed).\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=";"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatStateVar : ";
	msg << "expected ';', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++(current);
      if(allComponents){
	unsigned short i;
	for(i=0;i<3u;++i){
	  oconverter.str("");
	  oconverter << "(" << i << ")";
	  string coefName2 = coefName + oconverter.str();
	  if(!stateVarValues.insert(make_pair(coefName2,value)).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatStateVar : ";
	    msg << "value for state variable " << coefName2 
		<< " already defined.\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	}
	if((this->dimension==2u)||(this->dimension==3u)){
	  oconverter.str("");
	  oconverter << "(" << 3u << ")";
	  string coefName2 = coefName + oconverter.str();
	  value*=UnaryLoadingDefinition::Cst_1_Sqrt2;
	  if(!stateVarValues.insert(make_pair(coefName2,value)).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatStateVar : ";
	    msg << "value for state variable " << coefName2 
		<< " already defined.\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	}
	if(this->dimension==3u){
	  oconverter.str("");
	  oconverter << "(" << 4u << ")";
	  string coefName2 = coefName + oconverter.str();
	  if(!stateVarValues.insert(make_pair(coefName2,value)).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatStateVar : ";
	    msg << "value for state variable " << coefName2 
		<< " already defined.\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	  oconverter.str("");
	  oconverter << "(" << 5u << ")";
	  coefName2 = coefName + oconverter.str();
	  if(!stateVarValues.insert(make_pair(coefName2,value)).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatStateVar : ";
	    msg << "value for state variable " << coefName2 
		<< " already defined.\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	}
      } else {
	oconverter.str("");
	oconverter << "(" << componentNbr << ")";
	string coefName2 = coefName + oconverter.str();
	if(!stateVarValues.insert(make_pair(coefName2,value)).second){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatStateVar : ";
	  msg << "value for state variable " << coefName2 
	      << " already defined.\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));	
	}
      }
    } else {
      ostringstream msg;
      --current;
      msg << "UnaryLoadingDefinition::treatStateVar : ";
      msg << "Internal error, unsupported type.\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    return current;
  }

  UnaryLoadingDefinition::TokensIterator
  UnaryLoadingDefinition::treatExternalStateVar(const std::string& coefName,
						const SupportedTypes::TypeFlag flag,
						UnaryLoadingDefinition::TokensIterator current,
						const UnaryLoadingDefinition::TokensIterator end)
  {
    using namespace std;
    map<string,map<long double,long double> >::iterator p;
    map<string,InterpolationFlag>::const_iterator p2;
    if(this->times.empty()){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
      msg << "'times' has not been defined.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(current==end){
      --current;
      ostringstream msg;
      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
      msg << "unexpected end of file.";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    if(flag==SupportedTypes::Scalar){
      if(current->value=="."){
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value!="setInterpolationType"){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "expected 'setInterpolationType', (read " << current->value << ")\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value!="("){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "expected '(', (read " << current->value << ")\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value=="\"ConstantPerInterval\""){
	  pair<string,InterpolationFlag> tmp = make_pair(coefName,ConstantPerInterval);
	  if(!this->externalStateVarInterpolationType.insert(tmp).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	    msg << "interpolation type has already been set for variable " 
		<< coefName << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));
	  }
	} else if(current->value=="\"LinearPerInterval\""){
	  pair<string,InterpolationFlag> tmp = make_pair(coefName,LinearPerInterval);
	  if(!this->externalStateVarInterpolationType.insert(tmp).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	    msg << "interpolation type has already been set for variable " 
		<< coefName << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));
	  }
	} else {
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unsupported interpolation type (read "<< current->value << ").";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value!=")"){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "expected ')', (read " << current->value << ")\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value!=";"){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "expected ')', (read " << current->value << ")\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	return current;
      }
      p2=this->externalStateVarInterpolationType.find(coefName);
      if(p2==this->externalStateVarInterpolationType.end()){
	pair<string,InterpolationFlag> tmp = make_pair(coefName,ConstantPerInterval);
	this->externalStateVarInterpolationType.insert(tmp);
      }
      if(current->value!="["){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected '[', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      istringstream iconverter(current->value);
      long double time;
      iconverter >> time;
      if(iconverter.fail()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "cannot read time definition for external state variable " << coefName << ".\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!iconverter.eof()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "cannot read time definition for external state variable " << coefName
	    << " (parsing failed).\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="]"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected ']', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="="){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected '=', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      istringstream converter(current->value);
      long double value;
      converter >> value;
      if(converter.fail()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "cannot read value for external state variable " << coefName << ".\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!converter.eof()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "cannot read value for external state variable " << coefName 
	    << " (parsing failed).\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      p  = externalStateVarValues.find(coefName);
      if(p==this->externalStateVarValues.end()){
	map<long double,long double> tmp;
	tmp.insert(make_pair(time,value));
	this->externalStateVarValues.insert(make_pair(coefName,tmp));
      } else {
	if(!(p->second.insert(make_pair(time,value))).second){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "value for external state variable " << coefName 
	      << " already defined at time " << time << ".\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));	
	}
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=";"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected ';', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
    } else if(flag==SupportedTypes::Stensor){
      bool allComponents = false;
      unsigned short componentNbr = 0u;
      long double value;
      if(current->value!="("){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected '(', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value=="*"){
	allComponents = true;
      } else {
	istringstream converter(current->value);
	converter >> componentNbr;
	if(converter.fail()){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "cannot read value for component number\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(!converter.eof()){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "cannot read value for component number (parsing failed).\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(componentNbr>5){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "invalid index for stensor (read "<< componentNbr << ").\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(componentNbr==3){
	  if((dimension!=2)||(dimension!=3)){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	    msg << componentNbr << "is not a valid index for stensor in dimension "
		<< dimension << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));
	  }
	}
	if((componentNbr==4)||(componentNbr==5)){
	  if(dimension!=3){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	    msg << componentNbr << "is not a valid index for stensor in dimension "
		<< dimension << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));
	  }
	}
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=")"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected ')', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value=="."){
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value!="setInterpolationType"){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "expected '[', (read " << current->value << ")\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value!="("){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "expected '(', (read " << current->value << ")\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value=="\"ConstantPerInterval\""){
	  if(allComponents){
	    for(unsigned short i=0;i<3;++i){
	      ostringstream coefName2;
	      coefName2 << coefName << "(" << i << ")";
	      pair<string,InterpolationFlag> tmp=make_pair(coefName2.str(),ConstantPerInterval);
	      if(!this->externalStateVarInterpolationType.insert(tmp).second){
		ostringstream msg;
		msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
		msg << "interpolation type has already been set for variable " 
		    << coefName2.str() << ".\n";
		msg << "Error at line " << current->line;
		throw(runtime_error(msg.str()));
	      }
	    }
	    if((this->dimension==2u)||(this->dimension==3u)){
	      ostringstream coefName2;
	      coefName2 << coefName << "(" << 3u << ")";
	      pair<string,InterpolationFlag> tmp=make_pair(coefName2.str(),ConstantPerInterval);
	      if(!this->externalStateVarInterpolationType.insert(tmp).second){
		ostringstream msg;
		msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
		msg << "interpolation type has already been set for variable " 
		    << coefName2.str() << ".\n";
		msg << "Error at line " << current->line;
		throw(runtime_error(msg.str()));
	      }
	    }
	    if(this->dimension==3u){
	      ostringstream coefName2;
	      coefName2 << coefName << "(" << 4u << ")";
	      pair<string,InterpolationFlag> tmp=make_pair(coefName2.str(),ConstantPerInterval);
	      if(!this->externalStateVarInterpolationType.insert(tmp).second){
		ostringstream msg;
		msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
		msg << "interpolation type has already been set for variable " 
		    << coefName2.str() << ".\n";
		msg << "Error at line " << current->line;
		throw(runtime_error(msg.str()));
	      }	      
	      coefName2.str("");
	      coefName2 << coefName << "(" << 5u << ")";
	      tmp=make_pair(coefName2.str(),ConstantPerInterval);
	      if(!this->externalStateVarInterpolationType.insert(tmp).second){
		ostringstream msg;
		msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
		msg << "interpolation type has already been set for variable " 
		    << coefName2.str() << ".\n";
		msg << "Error at line " << current->line;
		throw(runtime_error(msg.str()));
	      }	      
	    }
	  } else {
	    ostringstream coefName2;
	    coefName2 << coefName << "(" << componentNbr << ")";
	    pair<string,InterpolationFlag> tmp=make_pair(coefName2.str(),ConstantPerInterval);
	    if(!this->externalStateVarInterpolationType.insert(tmp).second){
	      ostringstream msg;
	      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	      msg << "interpolation type has already been set for variable " 
		  << coefName2.str() << ".\n";
	      msg << "Error at line " << current->line;
	      throw(runtime_error(msg.str()));
	    }	      
	  }
	} else if(current->value=="\"LinearPerInterval\""){
	  if(allComponents){
	    for(unsigned short i=0;i<3;++i){
	      ostringstream coefName2;
	      coefName2 << coefName << "(" << i << ")";
	      pair<string,InterpolationFlag> tmp=make_pair(coefName2.str(),LinearPerInterval);
	      if(!this->externalStateVarInterpolationType.insert(tmp).second){
		ostringstream msg;
		msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
		msg << "interpolation type has already been set for variable " 
		    << coefName2.str() << ".\n";
		msg << "Error at line " << current->line;
		throw(runtime_error(msg.str()));
	      }
	    }
	    if((this->dimension==2u)||(this->dimension==3u)){
	      ostringstream coefName2;
	      coefName2 << coefName << "(" << 3u << ")";
	      pair<string,InterpolationFlag> tmp=make_pair(coefName2.str(),LinearPerInterval);
	      if(!this->externalStateVarInterpolationType.insert(tmp).second){
		ostringstream msg;
		msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
		msg << "interpolation type has already been set for variable " 
		    << coefName2.str() << ".\n";
		msg << "Error at line " << current->line;
		throw(runtime_error(msg.str()));
	      }
	    }
	    if(this->dimension==3u){
	      ostringstream coefName2;
	      coefName2 << coefName << "(" << 4u << ")";
	      pair<string,InterpolationFlag> tmp=make_pair(coefName2.str(),LinearPerInterval);
	      if(!this->externalStateVarInterpolationType.insert(tmp).second){
		ostringstream msg;
		msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
		msg << "interpolation type has already been set for variable " 
		    << coefName2.str() << ".\n";
		msg << "Error at line " << current->line;
		throw(runtime_error(msg.str()));
	      }	      
	      coefName2.str("");
	      coefName2 << coefName << "(" << 5u << ")";
	      tmp=make_pair(coefName2.str(),LinearPerInterval);
	      if(!this->externalStateVarInterpolationType.insert(tmp).second){
		ostringstream msg;
		msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
		msg << "interpolation type has already been set for variable " 
		    << coefName2.str() << ".\n";
		msg << "Error at line " << current->line;
		throw(runtime_error(msg.str()));
	      }	      
	    }
	  } else {
	    ostringstream coefName2;
	    coefName2 << coefName << "(" << componentNbr << ")";
	    pair<string,InterpolationFlag> tmp=make_pair(coefName2.str(),LinearPerInterval);
	    if(!this->externalStateVarInterpolationType.insert(tmp).second){
	      ostringstream msg;
	      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	      msg << "interpolation type has already been set for variable " 
		  << coefName2.str() << ".\n";
	      msg << "Error at line " << current->line;
	      throw(runtime_error(msg.str()));
	    }	      
	  }
	} else {
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unsupported interpolation type (read "<< current->value << ").";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value!=")"){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "expected ')', (read " << current->value << ")\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	if(current==end){
	  --current;
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "unexpected end of file.";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	if(current->value!=";"){
	  ostringstream msg;
	  msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	  msg << "expected ')', (read " << current->value << ")\n";
	  msg << "Error at line " << current->line;
	  throw(runtime_error(msg.str()));
	}
	++current;
	return current;
      }
      if(current->value!="["){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected '[', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      istringstream iconverter(current->value);
      long double time;
      iconverter >> time;
      if(iconverter.fail()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "cannot read time definition for external state variable " << coefName << ".\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!iconverter.eof()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "cannot read time definition for external state variable " << coefName
	    << " (parsing failed).\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="]"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected ']', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!="="){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected '=', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      istringstream converter(current->value);
      ostringstream oconverter;
      converter >> value;
      if(converter.fail()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "cannot read value for stensor " << coefName;
	msg << "\nError at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(!converter.eof()){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "cannot read value for stensor " << coefName << " (parsing failed).\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++current;
      if(current==end){
	--current;
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "unexpected end of file.";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      if(current->value!=";"){
	ostringstream msg;
	msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	msg << "expected ';', (read " << current->value << ")\n";
	msg << "Error at line " << current->line;
	throw(runtime_error(msg.str()));
      }
      ++(current);
      if(allComponents){
	unsigned short i;
	for(i=0;i<3u;++i){
	  oconverter.str("");
	  oconverter << "(" << i << ")";
	  string coefName2 = coefName + oconverter.str();
	  p2=this->externalStateVarInterpolationType.find(coefName2);
	  if(p2==this->externalStateVarInterpolationType.end()){
	    pair<string,InterpolationFlag> tmp = make_pair(coefName2,ConstantPerInterval);
	    this->externalStateVarInterpolationType.insert(tmp);
	  }
	  p  = externalStateVarValues.find(coefName2);
	  if(p==this->externalStateVarValues.end()){
	    map<long double,long double> tmp;
	    tmp.insert(make_pair(time,value));
	    this->externalStateVarValues.insert(make_pair(coefName2,tmp));
	  } else {
	    if(!(p->second.insert(make_pair(time,value))).second){
	      ostringstream msg;
	      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	      msg << "value for external state variable " << coefName2
		  << " already defined at time " << time << ".\n";
	      msg << "Error at line " << current->line;
	      throw(runtime_error(msg.str()));	
	    }
	  }
	}
	if((this->dimension==2u)||(this->dimension==3u)){
	  oconverter.str("");
	  oconverter << "(" << 3u << ")";
	  string coefName2 = coefName + oconverter.str();
	  value*=UnaryLoadingDefinition::Cst_1_Sqrt2;
	  p2=this->externalStateVarInterpolationType.find(coefName2);
	  if(p2==this->externalStateVarInterpolationType.end()){
	    pair<string,InterpolationFlag> tmp = make_pair(coefName2,ConstantPerInterval);
	    this->externalStateVarInterpolationType.insert(tmp);
	  }
	  p  = externalStateVarValues.find(coefName2);
	  if(p==this->externalStateVarValues.end()){
	    map<long double,long double> tmp;
	    tmp.insert(make_pair(time,value));
	    this->externalStateVarValues.insert(make_pair(coefName2,tmp));
	  } else {
	    if(!(p->second.insert(make_pair(time,value))).second){
	      ostringstream msg;
	      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	      msg << "value for external state variable " << coefName2 
		  << " already defined at time " << time << ".\n";
	      msg << "Error at line " << current->line;
	      throw(runtime_error(msg.str()));	
	    }
	  }
	}
	if(this->dimension==3u){
	  oconverter.str("");
	  oconverter << "(" << 4u << ")";
	  string coefName2 = coefName + oconverter.str();
	  p2=this->externalStateVarInterpolationType.find(coefName2);
	  if(p2==this->externalStateVarInterpolationType.end()){
	    pair<string,InterpolationFlag> tmp = make_pair(coefName2,ConstantPerInterval);
	    this->externalStateVarInterpolationType.insert(tmp);
	  }
	  p  = externalStateVarValues.find(coefName2);
	  if(p==this->externalStateVarValues.end()){
	    map<long double,long double> tmp;
	    tmp.insert(make_pair(time,value));
	    this->externalStateVarValues.insert(make_pair(coefName2,tmp));
	  } else {
	    if(!(p->second.insert(make_pair(time,value))).second){
	      ostringstream msg;
	      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	      msg << "value for external state variable " << coefName2 
		  << " already defined at time " << time << ".\n";
	      msg << "Error at line " << current->line;
	      throw(runtime_error(msg.str()));	
	    }
	  }
	  oconverter.str("");
	  oconverter << "(" << 5u << ")";
	  coefName2 = coefName + oconverter.str();
	  p2=this->externalStateVarInterpolationType.find(coefName2);
	  if(p2==this->externalStateVarInterpolationType.end()){
	    pair<string,InterpolationFlag> tmp = make_pair(coefName2,ConstantPerInterval);
	    this->externalStateVarInterpolationType.insert(tmp);
	  }
	  p  = externalStateVarValues.find(coefName2);
	  if(p==this->externalStateVarValues.end()){
	    map<long double,long double> tmp;
	    tmp.insert(make_pair(time,value));
	    this->externalStateVarValues.insert(make_pair(coefName2,tmp));
	  } else {
	    if(!(p->second.insert(make_pair(time,value))).second){
	      ostringstream msg;
	      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	      msg << "value for external state variable " << coefName2 
		  << " already defined at time " << time << ".\n";
	      msg << "Error at line " << current->line;
	      throw(runtime_error(msg.str()));	
	    }
	  }
	}
      } else {
	oconverter.str("");
	oconverter << "(" << componentNbr << ")";
	string coefName2 = coefName + oconverter.str();
	p2=this->externalStateVarInterpolationType.find(coefName2);
	if(p2==this->externalStateVarInterpolationType.end()){
	  pair<string,InterpolationFlag> tmp = make_pair(coefName2,ConstantPerInterval);
	  this->externalStateVarInterpolationType.insert(tmp);
	}
	p  = externalStateVarValues.find(coefName2);
	if(p==this->externalStateVarValues.end()){
	  map<long double,long double> tmp;
	  tmp.insert(make_pair(time,value));
	  this->externalStateVarValues.insert(make_pair(coefName2,tmp));
	} else {
	  if(!(p->second.insert(make_pair(time,value))).second){
	    ostringstream msg;
	    msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
	    msg << "value for external state variable " << coefName2 
		<< " already defined at time " << time << ".\n";
	    msg << "Error at line " << current->line;
	    throw(runtime_error(msg.str()));	
	  }
	}
      }
    } else {
      ostringstream msg;
      --current;
      msg << "UnaryLoadingDefinition::treatExternalStateVar : ";
      msg << "Internal error, unsupported type.\n";
      msg << "Error at line " << current->line;
      throw(runtime_error(msg.str()));
    }
    return current;
  } // end of UnaryLoadingDefinition::treatExternalStateVar

  void UnaryLoadingDefinition::writeTestData(std::ostream& os) const
  {
    using namespace std;
    map<string,map<long double,long double> >::const_iterator p1;
    map<long double,long double>::const_iterator p2;
    map<string,long double>::const_iterator p;
    map<string,InterpolationFlag>::const_iterator p3;
    os << "#Test type : " << this->testType  << endl;
    os << "#Dimension : " << this->dimension << endl;
    if(this->useQt){
      os << "#UseQt : true\n";
    } else {
      os << "#UseQt : false\n";
    }
    if(!this->times.empty()){
      os << "#times : ";
      copy(times.begin(),times.end(),ostream_iterator<long double>(os," "));
      os << endl;
    }
    if(!this->loadingValues.empty()){
      os << "#\n#Time increment ";
      if(this->loadingInterpolationType==ConstantPerInterval){
	os << " (constant per interval)\n"; 
      } else if (this->loadingInterpolationType==LinearPerInterval){
	os << " (linear per interval)\n"; 
      } else {
	string msg("UnaryLoadingDefinition::writeTestData : ");
	msg += "internal error ";
	throw(runtime_error(msg));
      }
      for(p2=this->loadingValues.begin();p2!=this->loadingValues.end();++p2){
	os << "(" << p2->first << "," << p2->second << ") ";
      }
      os << endl;
    }
    if(!this->coefValues.empty()){
      os << "#\n#Coef values\n#\n";
      for(p=this->coefValues.begin();p!=this->coefValues.end();++p){
	os << "#Coef " << p->first << " = " << p->second << endl; 
      }
    }
    if(!this->stateVarValues.empty()){
      os << "#\n#Initial State Variables\n#\n";
      for(p=this->stateVarValues.begin();p!=this->stateVarValues.end();++p){
	os << "#StateVar " << p->first << "[0] = " << p->second << endl; 
      }
    }
    if(!this->temperatureValues.empty()){
      os << "#\n#Time increment ";
      if(this->temperatureInterpolationType==ConstantPerInterval){
	os << " (constant per interval)\n"; 
      } else if (this->temperatureInterpolationType==LinearPerInterval){
	os << " (linear per interval)\n"; 
      } else {
	string msg("UnaryTemperatureDefinition::writeTestData : ");
	msg += "internal error ";
	throw(runtime_error(msg));
      }
      for(p2=this->temperatureValues.begin();p2!=this->temperatureValues.end();++p2){
	os << "(" << p2->first << "," << p2->second << ") ";
      }
      os << endl;
    }
    if(!this->externalStateVarValues.empty()){
      os << "#\n#External State Variables\n#\n";
      for(p1 =this->externalStateVarValues.begin();
	  p1!=this->externalStateVarValues.end();++p1){
	os << "#ExternalStateVar " << p1->first;
	p3=this->externalStateVarInterpolationType.find(p1->first);
	if(p3==this->externalStateVarInterpolationType.end()){
	  string msg("UnaryLoadingDefinition::writeTestData : ");
	  msg += "interpolation type was not defined for external variable ";
	  msg += p1->first;
	  throw(runtime_error(msg));
	}
	if(p3->second==ConstantPerInterval){
	  os << " (constant per interval)\n"; 
	} else if (p3->second==LinearPerInterval){
	  os << " (linear per interval)\n"; 
	} else {
	  string msg("UnaryLoadingDefinition::writeTestData : ");
	  msg += "internal error ";
	  throw(runtime_error(msg));
	}
	for(p2=p1->second.begin();p2!=p1->second.end();++p2){
	  os << "(" << p2->first << "," << p2->second << ") ";
	}
	os << endl;
      }
    }
  } // end of UnaryLoadingDefinition::writeTestData

} // end of namespace mfront
