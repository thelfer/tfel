/*! 
 * \file   mfront/src/UMATFiniteStrainMTestFileGenerator.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  10 juil. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<stdexcept>
#include<algorithm>

#include"MFront/UMATFiniteStrainMTestFileGenerator.hxx"

namespace mfront
{

  UMATFiniteStrainMTestFileGenerator::UMATFiniteStrainMTestFileGenerator(const std::string& i,
									 const std::string& l,
									 const std::string& b)
    : interface(i),
      library(l),
      behaviour(b)
  {
    using namespace std;
    fill(F0,F0+36,0.);
    fill(F1,F1+36,0.);
    fill(stress,stress+6,0.);
  } // end of UMATFiniteStrainMTestFileGenerator::UMATFiniteStrainMTestFileGenerator

  void
  UMATFiniteStrainMTestFileGenerator::writeBehaviourDeclaration(std::ostream& os) const
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    os << "@Behaviour<" << this->interface << "> '" << this->library << ".dll' '"<< this->behaviour << "';" << endl;
#else
    os << "@Behaviour<" << this->interface << "> '" << this->library << ".so' '"<< this->behaviour << "';" << endl;
#endif
  } // end of UMATFiniteStrainMTestFileGenerator::writeBehaviourDeclaration

  void
  UMATFiniteStrainMTestFileGenerator::setDeformationGradientAtTheBeginningOfTheStimeStep(const double* const F)
  {
    this->F0[0] = F[0];
    this->F0[1] = F[4];
    this->F0[2] = F[8];
    if(this->getTensorSize()>=5u){
      this->F0[3] = F[3];
      this->F0[4] = F[1];
      if(this->getTensorSize()==9u){
	this->F0[5] = F[6];
	this->F0[6] = F[2];
	this->F0[7] = F[7];
	this->F0[8] = F[5];
      }
    }
  } // end of UMATFiniteStrainMTestFileGenerator::setStrainTensor

  void
  UMATFiniteStrainMTestFileGenerator::setDeformationGradientAtTheEndOfTheStimeStep(const double* const F)
  {
    this->F1[0] = F[0];
    this->F1[1] = F[4];
    this->F1[2] = F[8];
    if(this->getTensorSize()>=5u){
      this->F1[3] = F[3];
      this->F1[4] = F[1];
      if(this->getTensorSize()==9u){
	this->F1[5] = F[6];
	this->F1[6] = F[2];
	this->F1[7] = F[7];
	this->F1[8] = F[5];
      }
    }
  } // end of UMATFiniteStrainMTestFileGenerator::setStrainTensorIncrement

  void
  UMATFiniteStrainMTestFileGenerator::setStressTensor(const double*const s)
  {
    using namespace std;
    copy(s,s+this->getStensorSize(),this->stress);
  } // end of UMATFiniteStrainMTestFileGenerator::setStressTensor

  void
  UMATFiniteStrainMTestFileGenerator::writeDrivingVariables(std::ostream& os) const
  {
    using namespace std;
    const auto& n = this->getDeformationGradientComponentsNames();
    vector<string>::const_iterator p;
    unsigned short i;
    if(this->times.size()!=2){
      string msg("UMATFiniteStrainMTestFileGenerator::writeDrivingVariables : "
		 "invalid number of times");
      throw(runtime_error(msg));
    }
    const real t0 = *(this->times.begin());
    const real t1 = *(this->times.rbegin());
    os << "@Stress {";
    os.precision(14);
    for(i=0;i!=this->getStensorSize();){
      if(i<3){
	os << this->stress[i];
      } else {
	os << (this->stress[i])*sqrt(2.);
      }
      if(++i!=this->getStensorSize()){
	os << ",";
      }
    }
    os << "};\n\n";
    for(p=n.begin(),i=0;p!=n.end();++p,++i){
      os.precision(14);
      os << "@ImposedDeformationGradient<evolution> '" << *p << "' {" 
	 << t0 << ":" << this->F0[i]<< ","
	 << t1 << ":" << this->F1[i]<< "};\n";
    }
    os << endl;
  } // end of UMATFiniteStrainMTestFileGenerator::writeDrivingVariables
  
  UMATFiniteStrainMTestFileGenerator::~UMATFiniteStrainMTestFileGenerator()
  {} // end of UMATFiniteStrainMTestFileGenerator::UMATFiniteStrainMTestFileGenerator

} // end of namespace mfront


