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
    this->registerVariable("Dt_nn");
    this->registerVariable("Dt_tt");
    this->registerVariable("Dt_tn");
    this->registerVariable("Dt_nt");
    this->registerVariable("u_n");
    this->registerVariable("du_n");
    this->registerVariable("t_n");
    this->registerVariable("u_t");
    this->registerVariable("du_t");
    this->registerVariable("t_t");
    this->mb.declareAsACohesiveZoneModel();
    this->mb.getLocalVariables().push_back(VarHandler("real&","u_n",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("real&","du_n",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("real&","t_n",1u,0u));    
    this->mb.getLocalVariables().push_back(VarHandler("tfel::math::VectorExpr<tfel::math::tvector<N-1,real>,tfel::math::TinyVectorFromTinyVectorViewExpr<N-1,N,1,real,false> >","u_t",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("tfel::math::VectorExpr<tfel::math::tvector<N-1,real>,tfel::math::TinyVectorFromTinyVectorViewExpr<N-1,N,1,real,false> >","du_t",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("tfel::math::VectorExpr<tfel::math::tvector<N-1,real>,tfel::math::TinyVectorFromTinyVectorViewExpr<N-1,N,1,real,false> >","t_t",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("real&","Dt_nn",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("tfel::math::tmatrix_row_view<N,N,0,1,N-1,real>","Dt_nt",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("tfel::math::tmatrix_column_view<N,N,0,1,N-1,real>","Dt_tn",1u,0u));
    this->mb.getLocalVariables().push_back(VarHandler("tfel::math::tmatrix_submatrix_view<N,N,1,1,N-1,N-1,real>","Dt_tt",1u,0u));
    this->localVariablesInitializers += 
      "u_n(this->u(0)),\ndu_n(this->du(0)),\nt_n(this->t(0)),\n"
      "u_t(this->u),\ndu_t(this->du),\nt_t(this->t),\n"
      "Dt_nn(this->Dt(0,0)),\nDt_nt(this->Dt),\nDt_tn(this->Dt),\nDt_tt(this->Dt)";
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
