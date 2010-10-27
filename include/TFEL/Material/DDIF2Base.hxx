/*! 
 * \file  DDIF2Base.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 mar 2010
 */

#ifndef   _LIB_TFEL_MATERIAL_DDIF2BASE_H_
#define   _LIB_TFEL_MATERIAL_DDIF2BASE_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace material
  {
    
    struct DDIF2Base
    {

      template<typename Stensor2Type1,
	       typename Stensor,
	       typename real>
      static TFEL_VISIBILITY_LOCAL void
      treatFracture(Stensor2Type1& dfe_ddeel,
		    real& dfe_dde,
		    real& fe,
		    const real em_1,
		    const real e,
		    const real de,
		    const Stensor& sig,
		    const Stensor& n,
		    const real sigr,
		    const real Rp,
		    const real young,
		    const real lambda,
		    const real mu,
		    const unsigned short i);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cart2cyl(tfel::math::stensor<1u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cart2cyl(tfel::math::stensor<2u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cart2cyl(tfel::math::stensor<3u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cyl2cart(tfel::math::stensor<1u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cyl2cart(tfel::math::stensor<2u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      cyl2cart(tfel::math::stensor<3u,real>&,
	       const real);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      rk(tfel::math::tmatrix<3,3,real>&,
	 tfel::math::tvector<3,real>&,
	 const real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const unsigned short,
	 const unsigned short,
	 const unsigned short);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      rkbm(tfel::math::tmatrix<3,3,real>&,
	 tfel::math::tvector<3,real>&,
	 const real,
	 const real,
	 const real,
	 real&,
	 real,
	 const real,
	 const real,
	 const real,
	 const real,
	 const unsigned short,
	 const unsigned short,
	 const unsigned short);

      template<typename real>
      static TFEL_VISIBILITY_LOCAL void
      rk2(real&,
	  const real,
	  const real,
	  const real,
	  const real,
	  const real,
	  const real,
	  const real,
	  const real,
	  const real,
	  const real);

    }; // end of struct DDIF2Base
    
  } // end of namespace material

} // end of namespace tfel

#include"TFEL/Material/DDIF2Base.ixx"

#endif /* _LIB_TFEL_MATERIAL_DDIF2BASE_H */

