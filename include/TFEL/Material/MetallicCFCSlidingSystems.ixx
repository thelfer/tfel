/*! 
 * \file  MetallicCFCSlidingSystems.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mars 2013
 */

#ifndef _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_IXX_
#define _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_IXX_ 

#include"TFEL/FSAlgorithm/copy.hxx"

namespace tfel
{

  namespace material
  {

    template<unsigned short N>
    MetallicCFCSlidingSystemsBase<N>::MetallicCFCSlidingSystemsBase()
    {
      using namespace tfel::math;
      using namespace tfel::fsalgo;
      const double coefm=1.0/sqrt(2.);
      const double coefn=1.0/sqrt(3.);
      // sliding systems
      const double nx[12]={ 1.0,1.0,1.0, 1.0, 1.0, 1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
      const double ny[12]={ 1.0,1.0,1.0,-1.0,-1.0,-1.0, 1.0, 1.0, 1.0,-1.0,-1.0,-1.0};
      const double nz[12]={ 1.0,1.0,1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
      const double mx[12]={-1.0, 0.0,-1.0,-1.0,0.0,1.0, 0.0,1.0,1.0,-1.0,1.0,0.0};
      const double my[12]={ 0.0,-1.0, 1.0, 0.0,1.0,1.0,-1.0,1.0,0.0, 1.0,0.0,1.0};
      const double mz[12]={ 1.0, 1.0, 0.0, 1.0,1.0,0.0, 1.0,0.0,1.0, 0.0,1.0,1.0};  
      for(unsigned short i=0;i!=12;++i){
	tvector<3u,double> ns(double(0)); 
	tvector<3u,double> ms(double(0)); 
	stensor<3u,double> tmus(double(0)); 
	ns[0]=nx[i]*coefn;
	ns[1]=ny[i]*coefn;
	ns[2]=nz[i]*coefn;
	ms[0]=mx[i]*coefm;
	ms[1]=my[i]*coefm;
	ms[2]=mz[i]*coefm;
	tmus[0]=ns[0]*ms[0];
	tmus[1]=ns[1]*ms[1];
	tmus[2]=ns[2]*ms[2];
	tmus[3]=(ns[0]*ms[1]+ns[1]*ms[0])*0.5*sqrt(2);
	tmus[4]=(ns[0]*ms[2]+ns[2]*ms[0])*0.5*sqrt(2);
	tmus[5]=(ns[1]*ms[2]+ns[2]*ms[1])*0.5*sqrt(2);
	copy<StensorDimeToSize<N>::value>::exe(tmus.begin(),
					       this->mus[i].begin());
      }
    } // end of MetallicCFCSlidingSystemsBase::MetallicCFCSlidingSystemsBase

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_H */

