/*!
 * \file   mfront/src/MFrontDefaultCZMParser.cxx
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
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->registerVariable("u",false);
    this->registerVariable("du",false);
    this->registerVariable("t",false);
    this->registerVariable("Dt_nn",false);
    this->registerVariable("Dt_tt",false);
    this->registerVariable("Dt_tn",false);
    this->registerVariable("Dt_nt",false);
    this->registerVariable("u_n",false);
    this->registerVariable("du_n",false);
    this->registerVariable("t_n",false);
    this->registerVariable("u_t",false);
    this->registerVariable("du_t",false);
    this->registerVariable("t_t",false);
    this->mb.declareAsACohesiveZoneModel();
    this->mb.addLocalVariable(h,VariableDescription("real&","u_n",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("real&","du_n",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("real&","t_n",1u,0u));    
    this->mb.addLocalVariable(h,VariableDescription("tfel::math::VectorExpr<tfel::math::tvector<N-1,real>,tfel::math::TinyVectorFromTinyVectorViewExpr<N-1,N,1,real,false> >","u_t",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("tfel::math::VectorExpr<tfel::math::tvector<N-1,real>,tfel::math::TinyVectorFromTinyVectorViewExpr<N-1,N,1,real,false> >","du_t",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("tfel::math::VectorExpr<tfel::math::tvector<N-1,real>,tfel::math::TinyVectorFromTinyVectorViewExpr<N-1,N,1,real,false> >","t_t",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("real&","Dt_nn",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("tfel::math::tmatrix_row_view<N,N,0,1,N-1,real>","Dt_nt",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("tfel::math::tmatrix_column_view<N,N,0,1,N-1,real>","Dt_tn",1u,0u));
    this->mb.addLocalVariable(h,VariableDescription("tfel::math::tmatrix_submatrix_view<N,N,1,1,N-1,N-1,real>","Dt_tt",1u,0u));
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
