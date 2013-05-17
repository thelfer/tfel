/*! 
 * \file  PolyCristalsSlidingSystems.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 16 mai 2013
 */

#ifndef TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_IXX
#define TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_IXX 1

#include"TFEL/Math/General/RandomRotationMatrix.hxx"

namespace tfel
{
  
  namespace material
  {
      
    template<unsigned short Np,
	     typename GS,
	     typename NumType>
    const PolyCristalsSlidingSystems<Np,GS,NumType>&
    PolyCristalsSlidingSystems<Np,GS,NumType>::getPolyCristalsSlidingSystems(){
      static PolyCristalsSlidingSystems gs;
      return gs;
    }
      
    template<unsigned short Np,
	     typename GS,
	     typename NumType>
    PolyCristalsSlidingSystems<Np,GS,NumType>::PolyCristalsSlidingSystems()
      : mus(Np)
    {
      using namespace tfel::math;
      // the sliding systems of one phase
      const GS& gs = GS::getSlidingSystems();
      for (unsigned short i=0;i!=Np;i++){
	// allocting memory
	this->mus[i].resize(Nss);
	// rotation matrix
	tmatrix<3u,3u,real> drot = getRandomRotationMatrix<real>();
	for (unsigned short j=0;j!=Nss;j++){
	  StrainStensor& mu = mus[i][j];
	  // local tensor of directional sense
	  mu = gs.mus[j];
	  // change to the global direction
	  mu.changeBasis(drot);
	}
      }
    }
  } // end of namespace material

} // end of namespace tfel

#endif /* TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_IXX */
