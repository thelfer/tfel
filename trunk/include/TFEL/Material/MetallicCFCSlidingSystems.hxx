/*! 
 * \file  MetallicCFCSlidingSystems.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mars 2013
 */

#ifndef _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_H_
#define _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel
{

  namespace material
  {

    /*!
     * An helper class which actually does the computation of the
     * tensor of directional sense for CFC cristals.
     */
    template<typename NumType>
    struct MetallicCFCSlidingSystems
    {
      //! a simple alias
      typedef NumType real;
      //! number of sliding systems
      static const unsigned short Nss = 12;
      //! tensor of directional sense
      tfel::math::tvector<Nss,tfel::math::stensor<3u,real> > mus;
      //! return the uniq instance of the class
      static const MetallicCFCSlidingSystems&
      getSlidingSystems(void);
    protected:
      /*!
       * constructor
       */
      MetallicCFCSlidingSystems();
    private:
      // copy constructor (disabled)
      MetallicCFCSlidingSystems(const MetallicCFCSlidingSystems&);
      // assignement operator (disabled)
      MetallicCFCSlidingSystems&
      operator=(const MetallicCFCSlidingSystems&);
    };

  } // end of namespace material

} // end of namespace tfel

#include"TFEL/Material/MetallicCFCSlidingSystems.ixx"

#endif /* _LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_H */

