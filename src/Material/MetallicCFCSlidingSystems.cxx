/*! 
 * \file  MetallicCFCSlidingSystems.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mars 2013
 */

#include"TFEL/Material/MetallicCFCSlidingSystems.hxx"

namespace tfel
{

  namespace material
  {

    const MetallicCFCSlidingSystems&
    MetallicCFCSlidingSystems::getSlidingSystems(void)
    {
      static MetallicCFCSlidingSystems gs;
      return gs;
    } // end of MetallicCFCSlidingSystems::getSlidingSystems

    MetallicCFCSlidingSystems::MetallicCFCSlidingSystems()
    {
      using namespace tfel::math;
      using namespace tfel::fsalgo;
      const double coefm=1.0/sqrt(2.);
      const double coefn=1.0/sqrt(3.);
      // sliding systems
      const double nx[Ngs]={ 1.0,1.0,1.0, 1.0, 1.0, 1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
      const double ny[Ngs]={ 1.0,1.0,1.0,-1.0,-1.0,-1.0, 1.0, 1.0, 1.0,-1.0,-1.0,-1.0};
      const double nz[Ngs]={ 1.0,1.0,1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
      const double mx[Ngs]={-1.0, 0.0,-1.0,-1.0,0.0,1.0, 0.0,1.0,1.0,-1.0,1.0,0.0};
      const double my[Ngs]={ 0.0,-1.0, 1.0, 0.0,1.0,1.0,-1.0,1.0,0.0, 1.0,0.0,1.0};
      const double mz[Ngs]={ 1.0, 1.0, 0.0, 1.0,1.0,0.0, 1.0,0.0,1.0, 0.0,1.0,1.0};  
      for(unsigned short i=0;i!=Ngs;++i){
	tvector<3u,double> ns(double(0)); 
	tvector<3u,double> ms(double(0)); 
	ns[0]=nx[i]*coefn;
	ns[1]=ny[i]*coefn;
	ns[2]=nz[i]*coefn;
	ms[0]=mx[i]*coefm;
	ms[1]=my[i]*coefm;
	ms[2]=mz[i]*coefm;
	this->mus[i][0]=ns[0]*ms[0];
	this->mus[i][1]=ns[1]*ms[1];
	this->mus[i][2]=ns[2]*ms[2];
	this->mus[i][3]=(ns[0]*ms[1]+ns[1]*ms[0])*0.5*sqrt(2);
	this->mus[i][4]=(ns[0]*ms[2]+ns[2]*ms[0])*0.5*sqrt(2);
	this->mus[i][5]=(ns[1]*ms[2]+ns[2]*ms[1])*0.5*sqrt(2);
      }
    } // end of MetallicCFCSlidingSystems::MetallicCFCSlidingSystems

  }
}
