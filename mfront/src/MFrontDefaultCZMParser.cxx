/*!
 * \file   MFrontDefaultCZMParser.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#include<string>
#include<fstream>
#include<stdexcept>

#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"
#include"MFront/MFrontDefaultCZMParser.hxx"

namespace mfront{

  MFrontDefaultCZMParser::MFrontDefaultCZMParser()
    : MFrontDefaultParserBase()
  {
    this->registerVariable("u");
    this->registerVariable("du");
    this->registerVariable("t");
    this->mb.declareAsACohesiveZoneModel();
    this->mb.getLocalVariables().push_back(VarHandler("real&","Dt_nn",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("tfel::math::tmatrix_row_view<N,N,0,1,N-1,real>","Dt_nt",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("tfel::math::tmatrix_column_view<N,N,0,1,N-1,real>","Dt_tn",1u,0u));
    this->localVariablesInitializers += "Dt_nn(this->Dt(0,0)),\nDt_nt(this->Dt),\nDt_tn(this->Dt)";
  }

  std::string
  MFrontDefaultCZMParser::getDescription(void)
  {
    return "this parser is the most generic one as it does not make any restriction "
           "on the behaviour or the integration method that may be used";
  } // end of MFrontDefaultCZMParser::getDescription
  
  std::string
  MFrontDefaultCZMParser::getName(void)
  {
    return "DefaultCZMParser";
  }

  void MFrontDefaultCZMParser::writeBehaviourParserSpecificIncludes(void)
  {
    MFrontDefaultParserBase::writeBehaviourParserSpecificIncludes();
    this->behaviourFile << "#include\"TFEL/Math/tmatrix.hxx\"\n";
    this->behaviourFile << "#include\"TFEL/Math/tvector.hxx\"\n";
  } // end of MFrontDefaultCZMParser::writeBehaviourParserSpecificIncludes

  MFrontDefaultCZMParser::~MFrontDefaultCZMParser()
  {} // end of MFrontDefaultCZMParser::~MFrontDefaultCZMParser

} // end of namespace mfront  
