/*! 
 * \file   PolyCristalsSlidingSystems.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  16 mai 2013
 */

#ifndef TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_HXX
#define TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_HXX 1
  
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/General/RandomRotationMatrix.hxx"

namespace tfel
{
  
  namespace material
  {

    /*!
     * \param[in] Np      : number of phases
     * \param[in] GS      : class describing the sliding system of one phase
     * \param[in] NumType : numeric type used
     */
    template<unsigned short Ng,
	     typename GS,
	     typename NumType>
    struct PolyCristalsSlidingSystems
    {
      //! number of sliding systems of the mono cristal
      static const unsigned short Ngs = GS::Ngs;
      //! a simple alias
      typedef NumType real;
      //! a simple alias
      typedef tfel::math::stensor<3u> StrainStensor;
      //! return the uniq instance of the class
      static const PolyCristalsSlidingSystems&
      getPolyCristalsSlidingSystems();
      //! tensor of directional senses, sorted by phases
      tfel::math::vector<tfel::math::vector<StrainStensor> > mus;
    private:
      /*!
       * default constructor (declared private~: it can only be used
       * by the getPolyCristalsSlidingSystems method)
       */
      PolyCristalsSlidingSystems();
      /*!
       * copy constructor (declared private to disable it)
       */
      PolyCristalsSlidingSystems(const PolyCristalsSlidingSystems&);
      /*!
       * assignement operatore (declared private to disable it)
       */
      PolyCristalsSlidingSystems&
      operator = (const PolyCristalsSlidingSystems&);     
    };
      
  } // end of namespace material
  
} // end of namespace tfel

#endif /* TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_HXX */
