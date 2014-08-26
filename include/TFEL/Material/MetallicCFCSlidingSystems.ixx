/*! 
 * \file  MetallicCFCSlidingSystems.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mars 2013
 */

#ifndef _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_IXX_
#define _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_IXX_ 


namespace tfel
{

  namespace material
  {

    template<typename NumType>
    const MetallicCFCSlidingSystems<NumType>&
    MetallicCFCSlidingSystems<NumType>::getSlidingSystems(void)
    {
      static MetallicCFCSlidingSystems gs;
      return gs;
    } // end of MetallicCFCSlidingSystems::getSlidingSystems

    template<typename NumType>
    MetallicCFCSlidingSystems<NumType>::MetallicCFCSlidingSystems()
    {
      using namespace tfel::math;
      using namespace tfel::fsalgo;
      const real coefm=real(1)/sqrt(real(2));
      const real coefn=real(1)/sqrt(real(3));
      // sliding systems
      const real nx[Nss]={ 1.0,1.0,1.0, 1.0, 1.0, 1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
      const real ny[Nss]={ 1.0,1.0,1.0,-1.0,-1.0,-1.0, 1.0, 1.0, 1.0,-1.0,-1.0,-1.0};
      const real nz[Nss]={ 1.0,1.0,1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
      const real mx[Nss]={-1.0, 0.0,-1.0,-1.0,0.0,1.0, 0.0,1.0,1.0,-1.0,1.0,0.0};
      const real my[Nss]={ 0.0,-1.0, 1.0, 0.0,1.0,1.0,-1.0,1.0,0.0, 1.0,0.0,1.0};
      const real mz[Nss]={ 1.0, 1.0, 0.0, 1.0,1.0,0.0, 1.0,0.0,1.0, 0.0,1.0,1.0};  
      for(unsigned short i=0;i!=Nss;++i){
	tvector<3u,real> ns(real(0)); 
	tvector<3u,real> ms(real(0)); 
	ns[0]=nx[i]*coefn;
	ns[1]=ny[i]*coefn;
	ns[2]=nz[i]*coefn;
	ms[0]=mx[i]*coefm;
	ms[1]=my[i]*coefm;
	ms[2]=mz[i]*coefm;
	this->mu[i][0]=ns[0]*ms[0];
	this->mu[i][1]=ns[1]*ms[1];
	this->mu[i][2]=ns[2]*ms[2];
	this->mu[i][3]=ns[1]*ms[0];
	this->mu[i][4]=ns[0]*ms[1];
	this->mu[i][5]=ns[2]*ms[0];
	this->mu[i][6]=ns[0]*ms[2];
	this->mu[i][7]=ns[2]*ms[1];
	this->mu[i][8]=ns[1]*ms[2];
	this->mus[i] = syme(this->mu[i]);
      }
    } // end of MetallicCFCSlidingSystems::MetallicCFCSlidingSystems

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_IXX */

