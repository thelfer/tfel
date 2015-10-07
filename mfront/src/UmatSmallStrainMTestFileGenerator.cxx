/*! 
 * \file   mfront/src/UmatSmallStrainMTestFileGenerator.cxx
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

#include"MFront/UmatSmallStrainMTestFileGenerator.hxx"

namespace mfront
{

  UmatSmallStrainMTestFileGenerator::UmatSmallStrainMTestFileGenerator(const std::string& i,
								       const std::string& l,
								       const std::string& b)
    : interface(i),
      library(l),
      behaviour(b)
  {
    using namespace std;
    fill(eto,eto+6,0.);
    fill(deto,deto+6,0.);
    fill(stress,stress+6,0.);
  } // end of UmatSmallStrainMTestFileGenerator::UmatSmallStrainMTestFileGenerator

  void
  UmatSmallStrainMTestFileGenerator::writeBehaviourDeclaration(std::ostream& os) const
  {
    using namespace std;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    os << "@Behaviour<" << this->interface << "> '" << this->library << ".dll' '"<< this->behaviour << "';" << endl;
#else
    os << "@Behaviour<" << this->interface << "> '" << this->library << ".so' '"<< this->behaviour << "';" << endl;
#endif
  } // end of UmatSmallStrainMTestFileGenerator::writeBehaviourDeclaration

  void
  UmatSmallStrainMTestFileGenerator::setStrainTensor(const double*const e)
  {
    using namespace std;
    copy(e,e+this->getStensorSize(),this->eto);
  } // end of UmatSmallStrainMTestFileGenerator::setStrainTensor

  void
  UmatSmallStrainMTestFileGenerator::setStrainTensorIncrement(const double*const de)
  {
    using namespace std;
    copy(de,de+this->getStensorSize(),this->deto);
  } // end of UmatSmallStrainMTestFileGenerator::setStrainTensorIncrement

  void
  UmatSmallStrainMTestFileGenerator::setStressTensor(const double*const s)
  {
    using namespace std;
    copy(s,s+this->getStensorSize(),this->stress);
  } // end of UmatSmallStrainMTestFileGenerator::setStressTensor

  void
  UmatSmallStrainMTestFileGenerator::writeDrivingVariables(std::ostream& os) const
  {
    using namespace std;
    using namespace tfel::material;
    const auto& n = this->getStrainComponentsNames();
    vector<string>::const_iterator p;
    unsigned short i;
    if(this->times.size()!=2){
      string msg("UmatSmallStrainMTestFileGenerator::writeDrivingVariables : "
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
      if(i<3){
	if(!((i==2)&&(this->hypothesis==ModellingHypothesis::PLANESTRAIN))){
	  os << "@ImposedStrain<evolution> '" << *p << "' {" 
	     << t0 << ":" << this->eto[i]<< ","
	     << t1 << ":" << this->eto[i]+this->deto[i]<< "};\n";
	}
      } else {
	os << "@ImposedStrain<evolution> '" << *p << "' {" 
	   << t0 << ":" << this->eto[i]/sqrt(2.) << ","
	   << t1 << ":" << (this->eto[i]+this->deto[i])/sqrt(2.) << "};\n";
      }
    }
    os << endl;
  } // end of UmatSmallStrainMTestFileGenerator::writeDrivingVariables
  
  UmatSmallStrainMTestFileGenerator::~UmatSmallStrainMTestFileGenerator()
  {} // end of UmatSmallStrainMTestFileGenerator::UmatSmallStrainMTestFileGenerator

} // end of namespace mfront


